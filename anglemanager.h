#ifndef ANGLEMANAGER_H
#define ANGLEMANAGER_H

#include "qwt.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_legend.h"
#include <QPen>
#include "qwt_plot_canvas.h"
#include <set>
#include <deque>

class AngleManager:public QObject
{
Q_OBJECT

public:
    explicit AngleManager(QwtPlot *p);
    ~AngleManager();
    void refresh_angle(float *angles);
    void setcheck(int i, int j, int k, bool flag);
    void clear();
private:
    void makepen();
    inline int fromCategoryToIndex(int i, int j, int k);
    void refresh_one_angle(int index, float value);

public slots:
    void r_angles(float *angles);
signals:
    void s_stable_angle(QString, int);
private:
    QwtPlot *plot;
    int Time_cycle;
    int last_stable_angle;
    double xplot[30];
    std::set<int> check;
    std::deque<int> values[22];
};

#endif // ANGLEMANAGER_H
