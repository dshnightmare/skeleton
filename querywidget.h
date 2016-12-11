#ifndef QUERYWIDGET_H
#define QUERYWIDGET_H

#include <QWidget>
#include <QDialog>
//#include <QIdentityProxyModel>
#include <QMenu>
#include "MySqlTableModel.h"
#include "database.h"
#include "infodetailview.h"

namespace Ui {
class QueryWidget;
}

class QueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QueryWidget(QWidget *parent = 0);
    ~QueryWidget();
    void update_data();
signals:
private slots:
    void on_query_clicked();
    void view_detail(const QModelIndex &index);
    void show_menu(const QPoint &pos);
    void on_deleteRowAction_triggered();

private:
    Ui::QueryWidget *ui;
    MySqlTableModel *model;
    QMenu *menu;
};

#endif // QUERYWIDGET_H
