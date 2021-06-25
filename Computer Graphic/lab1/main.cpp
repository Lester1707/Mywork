#include <QWidget>
#include <QApplication>
#include <QPainter>
#include <QMenuBar>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <iostream>
#include <QSlider>
#include <math.h>
#include <string>
#include <QVector>
#include <vector>
using namespace std;

class Plot : public QWidget {
public:
    Plot(QWidget *par = 0) : QWidget(par), pen_graph(Qt::black, 1, Qt::SolidLine), pen_lines(Qt::red, 1, Qt::SolidLine), point(6) {
        dfi = 0.001f;
        scale = 200.f;
        Z = 700;
        x0 = (float)width()/2;
        y0 = (float)height()/2;
        resolution = width()/height();
        slider = new QSlider(Qt::Horizontal, this);
        slider->setGeometry(100, 100, 100, 100);
        slider->setValue(1);
        label = new QLabel("1", this);
        x_label = new QLabel("X", this);
        y_label = new QLabel("Y", this);
        for (int i = 0; i < point.size(); i++)
            point[i] = new QLabel(this);
        connect(slider, &QSlider::valueChanged, label,
                static_cast<void (QLabel::*)(int)>(&QLabel::setNum));
        connect(slider, &QSlider::valueChanged, this, [=] () {repaint();});
    }

private:
    //Q_OBJECT
    QPen pen_graph;
    QPen pen_lines;
    float dfi;
    float x0, y0;
    int Z;
    float scale;
    QSlider* slider;
    QLabel* label;
    QVector <QLabel*> point;
    QLabel* x_label;
    QLabel* y_label;
    float resolution;

    float Function (const int a, const float &ang) {
        return a*(1 - cos(ang));
    }

    void drawFunction(QPainter *qp) {
        slider->setGeometry(5, 2*y0 - 2*y0/10, 2*x0/4, 2*y0/20);
        label->setGeometry(5 + 2*x0/4, 2*y0 - 2*y0/10, 15, 15);
        float x1 = x0;
        float y1 = y0;
        float x2, y2;
        for (float step = 0; step < 2*M_PI; step += dfi) {
            x2 = x0 + Function(slider->value(), step)*cos(step)*min(x0, y0)/scale;
            y2 = y0 + Function(slider->value(), step)*sin(step)*min(x0, y0)/scale;
            qp->drawLine(QLineF(x1, y1, x2, y2));
            x1 = x2;
            y1 = y2;
        }
    }
    void paintEvent(QPaintEvent *e) {
        x0 = (float)width()/2;
        y0 = (float)height()/2;
        Z = min(width(), height());
        Q_UNUSED(e);
        QPainter qp(this);
        qp.setRenderHint(QPainter::Antialiasing);
        qp.setPen(pen_lines);
        qp.drawLine(0, y0, width(), y0);
        qp.drawLine(width() - x0/40, y0 - x0/40, width(), y0);
        qp.drawLine(width() - x0/40, y0 + x0/40, width(), y0);
        x_label->setGeometry(width() - width()/30, y0 + 5, 15, 15);
        qp.drawLine(x0, 0, x0, height());
        qp.drawLine(x0, 0, x0 - y0/40, y0/40);
        qp.drawLine(x0, 0, x0 + y0/40, y0/40);
        y_label->setGeometry(x0+5, y0/30, 15, 15);
        point[0]->setNum(2*x0/Z*scale/4);
        point[1]->setNum(2*x0/Z*scale/2);
        point[2]->setNum(2*x0/Z*3*scale/4);
        point[3]->setNum(2*y0/Z*scale/4);
        point[4]->setNum(2*y0/Z*scale/2);
        point[5]->setNum(2*y0/Z*3*scale/4);
        point[0]->setGeometry(x0 + x0/4, y0, 30, 15);
        point[1]->setGeometry(x0 + x0/2, y0, 30, 15);
        point[2]->setGeometry(x0 + 3*x0/4, y0, 30, 15);
        point[3]->setGeometry(x0, y0 - y0/4, 30, 15);
        point[4]->setGeometry(x0 + 2, y0 - y0/2, 30, 15);
        point[5]->setGeometry(x0 + 2, y0 - 3*y0/4, 30, 15);
        qp.setPen(pen_graph);
        drawFunction(&qp);
    }
};

int main(int argc, char *argv[]) {

  QApplication app(argc, argv);
  Plot window;

  window.resize(700, 700);
  window.move(0, 0);
  window.setWindowTitle("ToolTip"); // устанавливаем заголовок для окна
  window.setToolTip("QWidget"); // устанавливаем всплывающую подсказку для виджета
  window.show();

  return app.exec();
}
