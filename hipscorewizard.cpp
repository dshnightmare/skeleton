#include "QtWidgets"
#include "hipscorewizard.h"

HipScoreWizard::HipScoreWizard(QWidget *parent, QString side)
    : QWizard(parent)
{
    addPage(new HipIntroPage);
    addPage(new HipPainLevelPage);
    addPage(new HipTreadPage);
    addPage(new HipFunctionPage);
    addPage(new HipPutOnShoesPage);
    addPage(new HipScoresPage);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setWindowFlags(windowFlags() & (~(Qt::WindowContextHelpButtonHint)) | Qt::CustomizeWindowHint);
    setWindowTitle(QString::fromLocal8Bit("�Źؽڣ�") + side + QString::fromLocal8Bit("��"));
    setButtonText(QWizard::NextButton, QString::fromLocal8Bit("��һ��"));
    setButtonText(QWizard::FinishButton, QString::fromLocal8Bit("���"));
    setButtonText(QWizard::CancelButton, QString::fromLocal8Bit("ȡ��"));
}

void HipScoreWizard::accept() {
    emit scoreChanged(field("sum").toString());
    QDialog::accept();
}

HipIntroPage::HipIntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("˵��"));

    introLabel = new QLabel(QString::fromLocal8Bit("�����ֱ��Ϊ�����ֵ�һ�������˹���д���ڶ������ɻ����Զ��������ۺϵõ����ս����"));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(introLabel);
    setLayout(layout);
}

HipPainLevelPage::HipPainLevelPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("��ʹ�̶�"));

    topLabel = new QLabel(QString::fromLocal8Bit("��ѡ��"));
    painComboBox = new QComboBox;
    painComboBox->addItem(QString::fromLocal8Bit("��/�ɺ���"));
    painComboBox->addItem(QString::fromLocal8Bit("��΢��ż�����������"));
    painComboBox->addItem(QString::fromLocal8Bit("��ȣ��ճ������Ӱ��"));
    painComboBox->addItem(QString::fromLocal8Bit("һ������ʹ������NSAID"));
    painComboBox->addItem(QString::fromLocal8Bit("�жȣ������ܣ������������"));
    painComboBox->addItem(QString::fromLocal8Bit("ż���ɴ�����ҩ��"));
    painComboBox->addItem(QString::fromLocal8Bit("��ʹ���ԣ�������������"));
    painComboBox->addItem(QString::fromLocal8Bit("������ȫɥʧ"));
    painComboBox->setCurrentIndex(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(painComboBox);
    setLayout(layout);
}

bool HipPainLevelPage::validatePage(){
    switch (painComboBox->currentIndex()){
    case 0:setField("first", 44);break;
    case 1:setField("first", 40);break;
    case 2:
    case 3:setField("first", 30);break;
    case 4:
    case 5:setField("first", 20);break;
    case 6:setField("first", 10);break;
    case 7:setField("first", 0);break;
    default:setField("first", 0);
    }
    return true;
}

HipTreadPage::HipTreadPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("��̬"));

    supportLabel = new QLabel(QString::fromLocal8Bit("֧�ߣ���ѡ�񣩣�"));
    supportComboBox = new QComboBox;
    supportComboBox->addItem(QString::fromLocal8Bit("��"));
    supportComboBox->addItem(QString::fromLocal8Bit("���ȳ�����Ҫ"));
    supportComboBox->addItem(QString::fromLocal8Bit("���ȶ̾���Ҫ"));
    supportComboBox->addItem(QString::fromLocal8Bit("������"));
    supportComboBox->addItem(QString::fromLocal8Bit("˫����"));
    supportComboBox->addItem(QString::fromLocal8Bit("��˫��/��������"));
    supportComboBox->setCurrentIndex(0);
    walkLabel = new QLabel(QString::fromLocal8Bit("���������ߣ���ѡ�񣩣�"));
    walkComboBox = new QComboBox;
    walkComboBox->addItem(QString::fromLocal8Bit("������"));
    walkComboBox->addItem(QString::fromLocal8Bit(">1000��"));
    walkComboBox->addItem(QString::fromLocal8Bit("500��"));
    walkComboBox->addItem(QString::fromLocal8Bit("���ڻ"));
    walkComboBox->addItem(QString::fromLocal8Bit("�Դ���������"));
    walkComboBox->setCurrentIndex(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(supportLabel);
    layout->addWidget(supportComboBox);
    layout->addWidget(walkLabel);
    layout->addWidget(walkComboBox);
    setLayout(layout);
}

bool HipTreadPage::validatePage(){
    switch (supportComboBox->currentIndex()){
    case 0:setField("third", 11);break;
    case 1:setField("third", 7);break;
    case 2:setField("third", 5);break;
    case 3:setField("third", 4);break;
    case 4:setField("third", 2);break;
    case 5:setField("third", 0);break;
    default:setField("fifth", 0);
    }
    switch (walkComboBox->currentIndex()){
    case 0:setField("forth", 11);break;
    case 1:setField("forth", 8);break;
    case 2:setField("forth", 5);break;
    case 3:setField("forth", 2);break;
    case 4:setField("forth", 0);break;
    default:setField("forth", 0);
    }
    return true;
}

HipFunctionPage::HipFunctionPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("���ܻ"));

    upstairLabel = new QLabel(QString::fromLocal8Bit("��¥�ݣ���ѡ�񣩣�"));
    upstairComboBox = new QComboBox;
    upstairComboBox->addItem(QString::fromLocal8Bit("����"));
    upstairComboBox->addItem(QString::fromLocal8Bit("ʹ�÷��ֿ�������¥"));
    upstairComboBox->addItem(QString::fromLocal8Bit("ʹ�������κη���������¥"));
    upstairComboBox->addItem(QString::fromLocal8Bit("������¥"));
    upstairComboBox->setCurrentIndex(0);
    sittingLabel = new QLabel(QString::fromLocal8Bit("������ѡ�񣩣�"));
    sittingComboBox = new QComboBox;
    sittingComboBox->addItem(QString::fromLocal8Bit("�κ����ӣ�1Сʱ"));
    sittingComboBox->addItem(QString::fromLocal8Bit("�����ӣ�0.5Сʱ"));
    sittingComboBox->addItem(QString::fromLocal8Bit("����"));
    sittingComboBox->setCurrentIndex(0);
    vehicleLabel = new QLabel(QString::fromLocal8Bit("��ͨ����ѡ�񣩣�"));
    vehicleComboBox = new QComboBox;
    vehicleComboBox->addItem(QString::fromLocal8Bit("�˹�����ͨ����/С����"));
    vehicleComboBox->addItem(QString::fromLocal8Bit("����ʹ�ý�ͨ����"));
    vehicleComboBox->setCurrentIndex(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(upstairLabel);
    layout->addWidget(upstairComboBox);
    layout->addWidget(sittingLabel);
    layout->addWidget(sittingComboBox);
    layout->addWidget(vehicleLabel);
    layout->addWidget(vehicleComboBox);
    setLayout(layout);
}

bool HipFunctionPage::validatePage(){
    switch (upstairComboBox->currentIndex()){
    case 0:setField("fifth", 4);break;
    case 1:setField("fifth", 2);break;
    case 2:setField("fifth", 1);break;
    case 3:setField("fifth", 0);break;
    default:setField("fifth", 0);
    }

    switch (sittingComboBox->currentIndex()){
    case 0:setField("seventh", 5);break;
    case 1:setField("seventh", 3);break;
    case 2:setField("seventh", 0);break;
    default:setField("seventh", 0);
    }

    switch (vehicleComboBox->currentIndex()){
    case 0:setField("eighth", 1);break;
    case 1:setField("eighth", 0);break;
    default:setField("eighth", 0);
    }
    return true;
}

HipPutOnShoesPage::HipPutOnShoesPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("������/ϵЬ��"));

    topLabel = new QLabel(QString::fromLocal8Bit("�볢����ɴ����Ӷ���"));
    startButton = new QPushButton;
    startButton->setText(QString::fromLocal8Bit("�����ʼ��ʱ"));
    startButton->setFixedWidth(150);
    connect(startButton, SIGNAL(clicked()), this, SLOT(on_click_start()));
    giveupButton = new QPushButton;
    giveupButton->setText(QString::fromLocal8Bit("�޷����/����"));
    giveupButton->setFixedWidth(150);
    //giveupButton->setEnabled(false);
    connect(giveupButton, SIGNAL(clicked()), this, SLOT(on_click_giveup()));
    progressBar = new QProgressBar;
    connect(progressBar, SIGNAL(valueChanged(int)), this, SLOT(validateNext(int)));
    progressBar->setRange(0, 40);
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(progressChanged()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(startButton);
    layout->addWidget(giveupButton);
    layout->addWidget(progressBar);
    setLayout(layout);
}

bool HipPutOnShoesPage::isComplete() const {
    return progressBar->value() == 40;
}

bool HipPutOnShoesPage::validatePage() {
    if (count2 < 20)
        setField("sixth", 0);
    else if(count1 < 20)
        setField("sixth", 3);
    else
        setField("sixth", 5);
    return true;
}

void HipPutOnShoesPage::progressChanged(){
    progressBar->setValue(progressBar->value() + 1);
    if(curShoes > 40)
        count1++;
    if(curShoes > 30)
        count2++;
    if(progressBar->value() == 40){
        timer->stop();
        emit stop_Kinect();
        QTime tt;
        tt.start();
        while(tt.elapsed() < 100)
        startButton->setEnabled(true);
    }
}

void HipPutOnShoesPage::on_click_start(){
    emit start_Kinect();
    count1 = 0;
    count2 = 0;
    startButton->setEnabled(false);
    giveupButton->setEnabled(true);
    progressBar->setValue(0);
    timer->start(1000);
}

void HipPutOnShoesPage::on_click_giveup(){
    if (progressBar->value() < 40){
        progressBar->setValue(40);
        timer->stop();
        emit stop_Kinect();
        QTime tt;
        tt.start();
        while(tt.elapsed() < 100)
        startButton->setEnabled(true);
        giveupButton->setEnabled(false);
    }
    count1 = 0;
    count2 = 0;
}

void HipPutOnShoesPage::validateNext(int){
    emit completeChanged();
}

void HipPutOnShoesPage::setShoes(int){

}

HipScoresPage::HipScoresPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("���ֻ���"));

    Group1Label = new QLabel(QString::fromLocal8Bit("��ʹ�̶ȣ�"));
    firstScore = new QLineEdit;
    firstScore->setReadOnly(true);
    Group2Label = new QLabel(QString::fromLocal8Bit("��̬��"));
    Group21Label = new QLabel(QString::fromLocal8Bit("���У�"));
    secondScore = new QLineEdit;
    secondScore->setReadOnly(true);;
    Group22Label = new QLabel(QString::fromLocal8Bit("֧�ߣ�"));
    thirdScore = new QLineEdit;
    thirdScore->setReadOnly(true);;
    Group23Label = new QLabel(QString::fromLocal8Bit("���������ߣ�"));
    forthScore = new QLineEdit;
    forthScore->setReadOnly(true);;
    Group3Label = new QLabel(QString::fromLocal8Bit("���ܻ��"));
    Group31Label = new QLabel(QString::fromLocal8Bit("��¥�ݣ�"));
    fifthScore = new QLineEdit;
    fifthScore->setReadOnly(true);;
    Group32Label = new QLabel(QString::fromLocal8Bit("������/ϵЬ����"));
    sixthScore = new QLineEdit;
    sixthScore->setReadOnly(true);;
    Group33Label = new QLabel(QString::fromLocal8Bit("����"));
    seventhScore = new QLineEdit;
    seventhScore->setReadOnly(true);;
    Group34Label = new QLabel(QString::fromLocal8Bit("��ͨ��"));
    eighthScore = new QLineEdit;
    eighthScore->setReadOnly(true);;
    Group4Label = new QLabel(QString::fromLocal8Bit("���α��֣�"));
    ninthScore = new QLineEdit;
    ninthScore->setReadOnly(true);;
    Group5Label = new QLabel(QString::fromLocal8Bit("���Χ��"));
    tenthScore = new QLineEdit;
    tenthScore->setReadOnly(true);
    sumLabel = new QLabel(QString::fromLocal8Bit("�����֣�"));
    sumScore = new QLineEdit;
    sumScore->setReadOnly(true);;

    registerField("first", firstScore);
    registerField("second", secondScore);
    registerField("third", thirdScore);
    registerField("forth", forthScore);
    registerField("fifth", fifthScore);
    registerField("sixth", sixthScore);
    registerField("seventh", seventhScore);
    registerField("eighth", eighthScore);
    registerField("ninth", ninthScore);
    registerField("tenth", tenthScore);
    registerField("sum", sumScore);

    QGridLayout *layout = new QGridLayout;
    layout->setColumnStretch(1, 1);
    layout->addWidget(Group1Label, 0, 0);
    layout->addWidget(firstScore, 0, 2);
    layout->addWidget(Group2Label, 1, 0);
    layout->addWidget(Group21Label, 2, 1);
    layout->addWidget(secondScore, 2, 2);
    layout->addWidget(Group22Label, 3, 1);
    layout->addWidget(thirdScore, 3, 2);
    layout->addWidget(Group23Label, 4, 1);
    layout->addWidget(forthScore, 4, 2);
    layout->addWidget(Group3Label, 5, 0);
    layout->addWidget(Group31Label, 6, 1);
    layout->addWidget(fifthScore, 6, 2);
    layout->addWidget(Group32Label, 7, 1);
    layout->addWidget(sixthScore, 7, 2);
    layout->addWidget(Group33Label, 8, 1);
    layout->addWidget(seventhScore, 8, 2);
    layout->addWidget(Group34Label, 9, 1);
    layout->addWidget(eighthScore, 9, 2);
    layout->addWidget(Group4Label, 10, 0);
    layout->addWidget(ninthScore, 10, 2);
    layout->addWidget(Group5Label, 11, 0);
    layout->addWidget(tenthScore, 11, 2);
    layout->addWidget(sumLabel, 12, 0);
    layout->addWidget(sumScore, 12, 2);
    setLayout(layout);
}

void HipScoresPage::cleanupPage(){

}

void HipScoresPage::initializePage(){
    int sum = 0;
    sum = firstScore->text().toInt() + secondScore->text().toInt() + thirdScore->text().toInt()
            + forthScore->text().toInt() + fifthScore->text().toInt() + sixthScore->text().toInt() + seventhScore->text().toInt()
            + eighthScore->text().toInt() + ninthScore->text().toInt() + tenthScore->text().toInt();
    sumScore->setText(QString::number(sum));
}

bool HipScoresPage::validatePage(){
    return true;
}
