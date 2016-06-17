#include "fillinfo.h"
#include "ui_fillinfo.h"

fillinfo::fillinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::fillinfo)
{
    ui->setupUi(this);
}

fillinfo::~fillinfo()
{
    delete ui;
}

void fillinfo::on_cancel_button_clicked()
{
    this->close();
}

void fillinfo::fillkneediagnosis()
{
    if (ui->knee1->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝内翻，");
    if (ui->knee2->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节骨性关节炎（三间室退变型），");
    if (ui->knee3->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节骨性关节炎（创伤后），");
    if (ui->knee4->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝外翻，");
    if (ui->knee5->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节内侧间室骨性关节炎，");
    if (ui->knee6->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("髌骨间室骨性关节炎，");
    if (ui->knee7->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("髌骨软化，");
    if (ui->knee8->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节外侧间室骨性关节炎，");
    if (ui->knee9->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节滑膜炎，");
    if (ui->knee10->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节积液，");
    if (ui->knee11->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节骨性关节炎（陈旧术后），");
    if (ui->knee12->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节红肿，");
    if (ui->knee13->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("下肢畸形（股骨），");
    if (ui->knee14->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("下肢畸形（胫骨），");
    if (ui->knee15->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("类风湿性关节炎（膝），");
    if (ui->knee16->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("强直性脊柱炎（膝），");
    if (ui->knee17->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节感染，");
    if (ui->knee18->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节疼痛（不明原因），");
    if (ui->knee19->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("胫骨结节骨骺炎，");
    if (ui->knee20->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节半月板损伤，");
    if (ui->knee21->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("膝关节骨梗死，");
    if (ui->knee22->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("人工膝关节置换术后假体松动，");
    if (ui->knee23->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("人工膝关节置换术后复查，");
    if (!kneediagnosis.isEmpty()) kneediagnosis.chop(1);
}
void fillinfo::fillhipdiagnosis()
{
    if (ui->hip1->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("原发性髋关节骨性关节炎，");
    if (ui->hip2->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("髋臼发育不良，");
    if (ui->hip3->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("髋臼发育不良继发骨性关节炎，");
    if (ui->hip4->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("髋关节骨性关节炎（创伤性），");
    if (ui->hip5->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("髋关节骨性关节炎（陈旧术后），");
    if (ui->hip6->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("股骨头缺血坏死（0期），");
    if (ui->hip7->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("股骨头缺血坏死（1期），");
    if (ui->hip8->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("股骨头缺血坏死（2期），");
    if (ui->hip9->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("股骨头缺血坏死（3期），");
    if (ui->hip10->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("股骨头缺血坏死（4期），");
    if (ui->hip11->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("髋关节骨性关节炎（陈旧感染），");
    if (ui->hip12->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("髋关节疼痛（不明原因），");
    if (ui->hip13->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("髋关节积液，");
    if (ui->hip14->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("髋关节盂唇损伤，");
    if (ui->hip15->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("髋关节撞击综合征，");
    if (ui->hip16->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("髋关节感染，");
    if (ui->hip17->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("强直性脊柱炎（髋），");
    if (ui->hip18->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("类风湿性关节炎（髋），");
    if (ui->hip19->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("人工髋关节置换术后假体松动，");
    if (ui->hip20->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("人工髋关节置换术后复查，");
    if (!hipdiagnosis.isEmpty()) hipdiagnosis.chop(1);

}

void fillinfo::on_ok_button_clicked()
{

    name=ui->name->text();
    gender=ui->gender->currentText();
    age=ui->age->text();
    height=ui->height->text();
    weight=ui->weight->text();
    medicaltype=ui->medicaltype->currentText();
    side=ui->side->currentText();
    year=ui->year->currentText();
    month=ui->month->currentText();
    day=ui->day->currentText();
    otherdiagnosis=ui->otherdiagonasis->toPlainText().simplified();

    if (ui->knee->isChecked()) diagonasis=diagonasis+"膝关节，";
    if (ui->hip->isChecked())  diagonasis=diagonasis+"髋关节，";
    if (!diagonasis.isEmpty()) diagonasis.chop(1);

    fillkneediagnosis();
    fillhipdiagnosis();



    this->close();
}
