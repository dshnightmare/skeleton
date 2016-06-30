#ifndef SEEANGLE_H
#define SEEANGLE_H

#include <QDialog>
#include "qwt.h"
#include "qwt_plot_curve.h"
#include "qwt_legend.h"
#include "QPen"
#include "qwt_plot_canvas.h"

namespace Ui {
class seeangle;
}

class seeangle : public QDialog
{
    Q_OBJECT

public:
    explicit seeangle(QWidget *parent = 0);
    ~seeangle();
    QString saveangle[22];
    double xplot[30];
    int Time_cycle;


signals:
    void s_end();

private slots:
    void on_ok_button_clicked();

    void on_cancel_button_clicked();

    void r_angles(float *angles);

private:
    Ui::seeangle *ui;
    void refresh_angle(float *angles);

    void refresh_one_angle(float *angles, int ptr);
};

#endif // SEEANGLE_H
