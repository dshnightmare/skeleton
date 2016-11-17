#include "ui_hipscore.h"
#include "ui_ui_hipscore.h"

ui_hipscore::ui_hipscore(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ui_hipscore)
{
    ui->setupUi(this);
}

ui_hipscore::~ui_hipscore()
{
    delete ui;
}
