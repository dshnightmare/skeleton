#ifndef INFODETAILVIEW_H
#define INFODETAILVIEW_H

#include <QDialog>

namespace Ui {
class InfoDetailView;
}

class InfoDetailView : public QDialog
{
    Q_OBJECT

public:
    explicit InfoDetailView(QWidget *parent = 0);
    ~InfoDetailView();
    void setDate(QString);
    void setName(QString);
    void setAge(QString);
    void setSex(QString);
    void setCardid(QString);
    void setComplain(QString);
    void setScore(QString);
    void setDiagnose(QString);
    void setPicture1(QString);
    void setPicture2(QString);
private:
    Ui::InfoDetailView *ui;
};

#endif // INFODETAILVIEW_H
