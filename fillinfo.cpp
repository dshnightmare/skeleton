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
    if (ui->knee1->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ڷ���");
    if (ui->knee2->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽڹ��Թؽ��ף��������˱��ͣ���");
    if (ui->knee3->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽڹ��Թؽ��ף����˺󣩣�");
    if (ui->knee4->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ⷭ��");
    if (ui->knee5->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽ��ڲ���ҹ��Թؽ��ף�");
    if (ui->knee6->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("�ƹǼ��ҹ��Թؽ��ף�");
    if (ui->knee7->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("�ƹ�����");
    if (ui->knee8->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽ������ҹ��Թؽ��ף�");
    if (ui->knee9->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽڻ�Ĥ�ף�");
    if (ui->knee10->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽڻ�Һ��");
    if (ui->knee11->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽڹ��Թؽ��ף��¾����󣩣�");
    if (ui->knee12->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽں��ף�");
    if (ui->knee13->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("��֫���Σ��ɹǣ���");
    if (ui->knee14->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("��֫���Σ��ֹǣ���");
    if (ui->knee15->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("���ʪ�Թؽ��ף�ϥ����");
    if (ui->knee16->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ǿֱ�Լ����ף�ϥ����");
    if (ui->knee17->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽڸ�Ⱦ��");
    if (ui->knee18->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽ���ʹ������ԭ�򣩣�");
    if (ui->knee19->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("�ֹǽ�ڹ����ף�");
    if (ui->knee20->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽڰ��°����ˣ�");
    if (ui->knee21->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("ϥ�ؽڹǹ�����");
    if (ui->knee22->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("�˹�ϥ�ؽ��û���������ɶ���");
    if (ui->knee23->isChecked()) kneediagnosis=kneediagnosis+QString::fromLocal8Bit("�˹�ϥ�ؽ��û����󸴲飬");
    if (!kneediagnosis.isEmpty()) kneediagnosis.chop(1);
}
void fillinfo::fillhipdiagnosis()
{
    if (ui->hip1->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("ԭ�����Źؽڹ��Թؽ��ף�");
    if (ui->hip2->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�žʷ���������");
    if (ui->hip3->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�žʷ��������̷����Թؽ��ף�");
    if (ui->hip4->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�Źؽڹ��Թؽ��ף������ԣ���");
    if (ui->hip5->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�Źؽڹ��Թؽ��ף��¾����󣩣�");
    if (ui->hip6->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�ɹ�ͷȱѪ������0�ڣ���");
    if (ui->hip7->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�ɹ�ͷȱѪ������1�ڣ���");
    if (ui->hip8->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�ɹ�ͷȱѪ������2�ڣ���");
    if (ui->hip9->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�ɹ�ͷȱѪ������3�ڣ���");
    if (ui->hip10->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�ɹ�ͷȱѪ������4�ڣ���");
    if (ui->hip11->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�Źؽڹ��Թؽ��ף��¾ɸ�Ⱦ����");
    if (ui->hip12->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�Źؽ���ʹ������ԭ�򣩣�");
    if (ui->hip13->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�Źؽڻ�Һ��");
    if (ui->hip14->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�Źؽ��۴����ˣ�");
    if (ui->hip15->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�Źؽ�ײ���ۺ�����");
    if (ui->hip16->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�Źؽڸ�Ⱦ��");
    if (ui->hip17->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("ǿֱ�Լ����ף��ţ���");
    if (ui->hip18->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("���ʪ�Թؽ��ף��ţ���");
    if (ui->hip19->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�˹��Źؽ��û���������ɶ���");
    if (ui->hip20->isChecked()) hipdiagnosis=hipdiagnosis+QString::fromLocal8Bit("�˹��Źؽ��û����󸴲飬");
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

    if (ui->knee->isChecked()) diagonasis=diagonasis+"ϥ�ؽڣ�";
    if (ui->hip->isChecked())  diagonasis=diagonasis+"�Źؽڣ�";
    if (!diagonasis.isEmpty()) diagonasis.chop(1);

    fillkneediagnosis();
    fillhipdiagnosis();



    this->close();
}
