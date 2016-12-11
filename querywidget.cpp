#include "querywidget.h"
#include "ui_querywidget.h"

QueryWidget::QueryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryWidget)
{
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(on_query_clicked()));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(view_detail(QModelIndex)));
//    QIdentityProxyModel *proxymodel = new QIdentityProxyModel(this);
//    proxymodel->setSourceModel(model);
    model = new MySqlTableModel(this, Database::getInstance()->db);
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->setTable("infolist");
    model->setSort(Database::Info_Id, Qt::AscendingOrder);
    model->setHeaderData(Database::Info_Name, Qt::Horizontal, QString::fromLocal8Bit("姓名"));
    model->setHeaderData(Database::Info_Age, Qt::Horizontal, QString::fromLocal8Bit("年龄"));
    model->setHeaderData(Database::Info_Sex, Qt::Horizontal, QString::fromLocal8Bit("性别"));
    model->setHeaderData(Database::Info_Complain, Qt::Horizontal, QString::fromLocal8Bit("主诉"));
    model->setHeaderData(Database::Info_Date, Qt::Horizontal, QString::fromLocal8Bit("诊断时间"));

//    int col = model->columnCount();
//    model->insertColumn(col);
//    model->setHeaderData(col, Qt::Horizontal, QString::fromLocal8Bit("操作"));

    ui->tableView->addAction(ui->deleteRowAction);
    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setColumnHidden(Database::Info_Id, true);
    ui->tableView->setColumnHidden(Database::Info_Height, true);
    ui->tableView->setColumnHidden(Database::Info_Weight, true);
    ui->tableView->setColumnHidden(Database::Info_BMI, true);
    ui->tableView->setColumnHidden(Database::Info_Medicinetype, true);
    ui->tableView->setColumnHidden(Database::Info_Cardid, true);
    ui->tableView->setColumnHidden(Database::Info_Score, true);
    ui->tableView->setColumnHidden(Database::Info_Diagnosis, true);
    //ui->tableView->setColumnHidden(col, false);
    //ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnWidth(Database::Info_Name, 80);
    ui->tableView->setColumnWidth(Database::Info_Age, 50);
    ui->tableView->setColumnWidth(Database::Info_Sex, 50);
    ui->tableView->setColumnWidth(Database::Info_Complain, 200);
    ui->tableView->setColumnWidth(Database::Info_Date, 150);
//    ui->tableView->setColumnWidth(Database::Info_Date + 1, 150);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(Database::Info_Name, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(Database::Info_Age, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(Database::Info_Sex, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(Database::Info_Complain, QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(Database::Info_Date, QHeaderView::Fixed);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableView->setItemDelegateForColumn(Database::Info_Date + 1, new MySqlTableButtonDelegate);


    menu = new QMenu(ui->tableView);
    menu->addAction(ui->deleteRowAction);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(show_menu(QPoint)));

    update_data();
    //emit model->layoutChanged();
}

QueryWidget::~QueryWidget()
{
    delete ui;
    delete model;
    delete menu;
}

void QueryWidget::on_query_clicked()
{
    QString field, value;
    switch (ui->comboBox->currentIndex()){
    case 0:field="name";break;
    case 1:field="complain";break;
    default:field="name";
    }
    value = QString::fromLocal8Bit(ui->lineEdit->text().toLocal8Bit().data());
//    query->prepare(ORIGINAL_QUERY + " where " + field + " == " + value + " or " + field + " like " + value);
//    query->exec();
    QString strFilter;
    if (value == "")
        strFilter = "1";
    else
        strFilter = QString(" %1 like '\%%2\%' ").arg(field).arg(value);
    model->setFilter(strFilter);
    update_data();
}

void QueryWidget::update_data(){
    model->select();
}

void QueryWidget::view_detail(const QModelIndex &index){
    int row = index.row();
    InfoDetailView view;
    view.setDate(model->record(row).value("date").toString());
    view.setName(model->record(row).value("name").toString());
    view.setAge(model->record(row).value("age").toString() + QString::fromLocal8Bit("岁"));
    view.setSex(model->record(row).value("sex").toString());
    view.setCardid(model->record(row).value("cardid").toString());
    view.setComplain(model->record(row).value("complain").toString());
    view.setScore(model->record(row).value("score").toString() + QString::fromLocal8Bit("分"));
    view.setDiagnose(model->record(row).value("diagnosis").toString());
    view.exec();
}

void QueryWidget::show_menu(const QPoint &pos){
    if (ui->tableView->indexAt(pos).isValid()){
        menu->move(ui->tableView->cursor().pos());
        menu->show();
    }
}

void QueryWidget::on_deleteRowAction_triggered()
{
    QModelIndexList currentSelection = ui->tableView->selectionModel()->selectedIndexes();
    for (int i = 0; i < currentSelection.count(); ++i) {
        if (currentSelection.at(i).column() != 0)
            continue;
        model->removeRow(currentSelection.at(i).row());
    }
    update_data();
}
