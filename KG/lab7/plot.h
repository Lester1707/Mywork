#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QApplication>
#include <QPainter>
#include <QMenuBar>
#include <QLabel>
#include <QFrame>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <iostream>
#include <QSlider>
#include <math.h>
#include <string>
#include <QVector>
#include <vector>
using namespace std;
class Plot : public QWidget
{
    int move_p;
    float N_value;
    vector <float> T;
    vector <pair <float, float>> points;
    unsigned int k_param;
    float x, y;
    float step;
    Q_OBJECT
    QPen pen;
public:
    Plot(QWidget *parent = nullptr);
    ~Plot();
    float N(const float &t, const vector <float> &x, const unsigned int &i, const unsigned int &k);
    void drawPlot(QPainter *qp);
    void paintEvent(QPaintEvent *e);
private:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void keyPressEvent(QKeyEvent *event);
};
#endif // PLOT_H
