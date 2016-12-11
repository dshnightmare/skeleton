#ifndef ANGLEMANAGER_H
#define ANGLEMANAGER_H

#include "qwt.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_legend.h"
#include "qwt_plot_grid.h"
#include <QPen>
#include "qwt_plot_canvas.h"
#include "qwt_symbol.h"
#include <set>
#include <deque>
#include "bodyangle.h"

class AngleManager:public QObject
{
Q_OBJECT

public:
    explicit AngleManager(QwtPlot *p);
    ~AngleManager();
    void refresh_angle(float *angles);
    void setcheck(int i, int j, int k, bool flag);
    void clear();
    int get_max_angles(int index) const;
    int get_min_angles(int index) const;
    int get_limpDiff() const;
    int get_limbDiff() const;
private:
    void makepen();
    inline int fromCategoryToIndex(int i, int j, int k);
    void refresh_one_angle(int index, float value);

public slots:
    void r_angles(float *angles);
    void r_joints(bodyangle joints);
signals:
    void s_stable_angle(QString, int);
private:
    QwtPlot *plot;
    int Time_cycle;
    int last_stable_angles[28];
    int max_angles[28];
    int min_angles[28];
    double xplot[30];
    std::set<int> check;
    std::deque<int> values[28];
    int limpDiff;
    int limbDiff;
};

#endif // ANGLEMANAGER_H
