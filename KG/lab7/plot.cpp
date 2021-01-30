#include "plot.h"

Plot::Plot(QWidget *parent) : QWidget(parent)
{
    move_p = -1;
    step = 0.001f;
    k_param = 4;
    points = {{1.f/7, 1.f/2}, {2.f/7, 1.f/2}, {3.f/7, 1.f/2}, {4.f/7, 1.f/2}, {5.f/7, 1.f/2}, {6.f/7, 1.f/2}};
    for (size_t i = 0; i < points.size() + k_param; i++)
        T.push_back(i);
    setMouseTracking(true);
}

float Plot::N (const float &t, const vector <float> &x, const unsigned int &i, const unsigned int &k) {
    if (k == 1) {
        if ((float)x[i] <= t && t < (float)x[i + 1])
            return 1.f;
        return 0.f;
    }
    return (t - x[i])*N(t, x, i, k - 1)/(x[i + k - 1] - x[i]) + (x[i + k] - t)*N(t, x, i + 1, k - 1)/(x[i + k] - x[i + 1]);
}

void Plot::drawPlot(QPainter *qp) {
        for (float t = T[k_param - 1]; t < T[points.size()]; t += step) {
            x = 0.f;
            y = 0.f;
            for(size_t i = 0; i < points.size(); i++) {
                N_value = N(t, T, i, k_param);
                x += N_value*points[i].first;
                y += N_value*points[i].second;
                //cout << N_value << endl;
            }
            qp->drawPoint(QPointF(width()*x, height()*y));
        }
}

void Plot::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    QPainter qp(this);
    qp.setRenderHint(QPainter::Antialiasing);
    qp.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    drawPlot(&qp);
    qp.setPen(QPen(Qt::red, 2, Qt::SolidLine));
    float radius = 2.f;
    for (size_t i = 0; i < points.size(); i++)
        qp.drawEllipse(QPointF(width()*points[i].first, height()*points[i].second), radius, radius);
}

void Plot::mousePressEvent(QMouseEvent* e) {
    float error = 5.f;
    for (size_t i = 0; i < points.size(); i++)
        if (abs(width()*points[i].first - e->pos().x()) < error && abs(height()*points[i].second - e->pos().y()) < error) {
            move_p = i;
            return;
        }
}

void Plot::mouseMoveEvent(QMouseEvent* e) {
    if (move_p != -1) {
        points[move_p].first = (float)e->pos().x()/width();
        points[move_p].second = (float)e->pos().y()/height();
        if (points[move_p].first < 0.f)
            points[move_p].first = 0.f;
        if (points[move_p].first > 1.f)
            points[move_p].first = 1.f;
        if (points[move_p].second < 0.f)
            points[move_p].second = 0.f;
        if (points[move_p].second > 1.f)
            points[move_p].second = 1.f;
        repaint();
    }
}

void Plot::mouseReleaseEvent(QMouseEvent* e) {
    Q_UNUSED(e);
    move_p = -1;
}

void Plot::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_R) {
        cout << "input " << points.size() + k_param << " nums" << endl;
        for (size_t i = 0; i < points.size() + k_param; i++) {
            cin >> T[i];
            cout << endl << T[i];
        }
    }
}

Plot::~Plot()
{

}

