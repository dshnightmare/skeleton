#include "QtWidgets"
#include "kneescorewizard.h"

KneeScoreWizard::KneeScoreWizard(QWidget *parent, QString side)
    : QWizard(parent)
{
    addPage(new KneeIntroPage);
    addPage(new KneePainLevelPage);
    addPage(new KneeStabilityPage);
    addPage(new KneeValgusPage);
    addPage(new KneeScoresPage);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setWindowFlags(windowFlags() & (~(Qt::WindowContextHelpButtonHint)) | Qt::CustomizeWindowHint);
    setWindowTitle(QString::fromLocal8Bit("ϥ�ؽڣ�") + side + QString::fromLocal8Bit("��"));
    setButtonText(QWizard::NextButton, QString::fromLocal8Bit("��һ��"));
    setButtonText(QWizard::FinishButton, QString::fromLocal8Bit("���"));
    setButtonText(QWizard::CancelButton, QString::fromLocal8Bit("ȡ��"));
}

void KneeScoreWizard::accept() {
    emit scoreChanged(field("sum").toString());
    QDialog::accept();
}

KneeIntroPage::KneeIntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("˵��"));

    introLabel = new QLabel(QString::fromLocal8Bit("�����ֱ��Ϊ�����ֵ�һ�������˹���д���ڶ������ɻ����Զ��������ۺϵõ����ս����"));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(introLabel);
    setLayout(layout);
}

KneePainLevelPage::KneePainLevelPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("��ʹ�̶�"));

    topLabel = new QLabel(QString::fromLocal8Bit("��ѡ��"));
    painComboBox = new QComboBox;
    painComboBox->addItem(QString::fromLocal8Bit("��"));
    painComboBox->addItem(QString::fromLocal8Bit("��΢��ż��"));
    painComboBox->addItem(QString::fromLocal8Bit("����¥��ʱ��ʹ"));
    painComboBox->addItem(QString::fromLocal8Bit("���ߺ�����¥��ʱ��ʹ"));
    painComboBox->addItem(QString::fromLocal8Bit("�е���ʹ��ż��"));
    painComboBox->addItem(QString::fromLocal8Bit("�еȣ�����"));
    painComboBox->addItem(QString::fromLocal8Bit("����"));
    painComboBox->setCurrentIndex(0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(painComboBox);
    setLayout(layout);
}

bool KneePainLevelPage::validatePage() {
    switch (painComboBox->currentIndex()){
    case 0:setField("first", 50);break;
    case 1:setField("first", 45);break;
    case 2:setField("first", 40);break;
    case 3:setField("first", 30);break;
    case 4:setField("first", 20);break;
    case 5:setField("first", 10);break;
    case 6:setField("first", 0);break;
    default:setField("first", 0);
    }
    return true;
}

KneeStabilityPage::KneeStabilityPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("�ȶ���"));

    fbLabel = new QLabel(QString::fromLocal8Bit("ǰ���ȶ��ԣ�"));
    fbGroup = new QButtonGroup;
    fbStableButton = new QRadioButton(QString::fromLocal8Bit("�ȶ�"));
    fbGroup->addButton(fbStableButton);
    fbUnstableButton = new QRadioButton(QString::fromLocal8Bit("���ȶ�"));
    fbGroup->addButton(fbUnstableButton);
    fbStableComboBox = new QComboBox;
    fbStableComboBox->addItem(QString::fromLocal8Bit("��ѡ���ȶ��Գ̶�"));
    fbStableComboBox->addItem("<5mm");
    fbStableComboBox->addItem("5-10mm");
    fbStableComboBox->addItem(">10mm");
    fbStableComboBox->setCurrentIndex(0);
    connect(fbStableButton, SIGNAL(toggled(bool)), fbStableComboBox, SLOT(setHidden(bool)));
    fbStableButton->setChecked(true);
    connect(fbStableButton, SIGNAL(toggled(bool)), this, SLOT(completeChangedFromButton(bool)));
    connect(fbStableComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(completeChangedFromComboBox(int)));

    ioLabel = new QLabel(QString::fromLocal8Bit("������ȶ��ԣ�"));
    ioGroup = new QButtonGroup;
    ioStableButton = new QRadioButton(QString::fromLocal8Bit("�ȶ�"));
    ioGroup->addButton(ioStableButton);
    ioUnstableButton = new QRadioButton(QString::fromLocal8Bit("���ȶ�"));
    ioGroup->addButton(ioUnstableButton);
    ioStableComboBox = new QComboBox;
    ioStableComboBox->addItem(QString::fromLocal8Bit("��ѡ���ȶ��Գ̶�"));
    ioStableComboBox->addItem("<5mm");
    ioStableComboBox->addItem("6-9mm");
    ioStableComboBox->addItem("10-14mm");
    ioStableComboBox->addItem(">15mm");
    ioStableComboBox->setCurrentIndex(0);
    connect(ioStableButton, SIGNAL(toggled(bool)), ioStableComboBox, SLOT(setHidden(bool)));
    ioStableButton->setChecked(true);
    connect(ioStableButton, SIGNAL(toggled(bool)), this, SLOT(completeChangedFromButton(bool)));
    connect(ioStableComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(completeChangedFromComboBox(int)));


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(fbLabel);
    layout->addWidget(fbStableButton);
    layout->addWidget(fbUnstableButton);
    layout->addWidget(fbStableComboBox);
    layout->addWidget(ioLabel);
    layout->addWidget(ioStableButton);
    layout->addWidget(ioUnstableButton);
    layout->addWidget(ioStableComboBox);
    setLayout(layout);
}

void KneeStabilityPage::completeChangedFromButton(bool checked) {
    emit completeChanged();
}

void KneeStabilityPage::completeChangedFromComboBox(int index) {
    emit completeChanged();
}

bool KneeStabilityPage::isComplete() const {
    return (fbStableButton->isChecked() || fbStableComboBox->currentIndex() > 0) &&
            (ioStableButton->isChecked() || ioStableComboBox->currentIndex() > 0);
}

bool KneeStabilityPage::validatePage() {
    switch (fbStableComboBox->currentIndex()){
    case 0:
    case 1:setField("third", 10);break;
    case 2:setField("third", 5);break;
    case 3:setField("third", 0);break;
    default:setField("third", 0);
    }

    switch (ioStableComboBox->currentIndex()){
    case 0:
    case 1:setField("forth", 15);break;
    case 2:setField("forth", 10);break;
    case 3:setField("forth", 5);break;
    case 4:setField("forth", 0);break;
    default:setField("forth", 0);
    }
    return true;
}

KneeValgusPage::KneeValgusPage(QWidget* parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("�ⷭ����"));

    topLabel = new QLabel(QString::fromLocal8Bit("�뱣����ֱվ��10��"));
    startButton = new QPushButton;
    startButton->setText(QString::fromLocal8Bit("�����ʼ��ʱ"));
    startButton->setFixedWidth(150);
    connect(startButton, SIGNAL(clicked()), this, SLOT(on_click_start()));
    cutdownBar = new QProgressBar;
    connect(cutdownBar, SIGNAL(valueChanged(int)), this, SLOT(validateNext(int)));
    cutdownBar->setRange(0, 10);
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(progressChanged()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(topLabel);
    layout->addWidget(startButton);
    layout->addWidget(cutdownBar);
    setLayout(layout);
}

bool KneeValgusPage::isComplete() const {
    return cutdownBar->value() == 10;
}

bool KneeValgusPage::validatePage(){
    std::sort(allValgus.begin(), allValgus.end());
    int size = allValgus.size();
    if (size == 0)
        return false;
    switch(allValgus[size / 2]){
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:setField("sixth", 0);break;
    case 6:
    case 7:setField("sixth", -3);break;
    case 8:
    case 9:setField("sixth", -6);break;
    case 10:
    case 11:setField("sixth", -9);break;
    case 12:
    case 13:setField("sixth", -12);break;
    case 14:
    case 15:setField("sixth", -15);break;
    default:setField("sixth", -20);
    }
    return true;
}

void KneeValgusPage::on_click_start(){
    emit start_Kinect();
    startButton->setEnabled(false);
    allValgus.clear();
    cutdownBar->setValue(0);
    timer->start(1000);
}

void KneeValgusPage::progressChanged(){
    cutdownBar->setValue(cutdownBar->value()+1);
    if(cutdownBar->value() == 10){
        timer->stop();
        emit stop_Kinect();
        QTime tt;
        tt.start();
        while(tt.elapsed()<100)
        startButton->setEnabled(true);
    }
}

void KneeValgusPage::setValgus(int valgus) {
    allValgus.push_back(valgus);
}

void KneeValgusPage::validateNext(int value){
    emit completeChanged();
}

KneeScoresPage::KneeScoresPage(QWidget* parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("���ֻ���"));

    firstLabel = new QLabel(QString::fromLocal8Bit("��ʹ�̶ȣ�"));
    firstScore = new QLineEdit;
    firstScore->setReadOnly(true);
    secondLabel = new QLabel(QString::fromLocal8Bit("���Χ��"));
    secondScore = new QLineEdit;
    secondScore->setReadOnly(true);
    thirdLabel = new QLabel(QString::fromLocal8Bit("ǰ���ȶ��ԣ�"));
    thirdScore = new QLineEdit;
    thirdScore->setReadOnly(true);
    forthLabel = new QLabel(QString::fromLocal8Bit("������ȶ��ԣ�"));
    forthScore = new QLineEdit;
    forthScore->setReadOnly(true);
    fifthLabel = new QLabel(QString::fromLocal8Bit("�����������Σ�"));
    fifthScore = new QLineEdit;
    fifthScore->setReadOnly(true);
    sixthLabel = new QLabel(QString::fromLocal8Bit("�ⷭ���ߣ�"));
    sixthScore = new QLineEdit;
    sixthScore->setReadOnly(true);
    sumLabel = new QLabel(QString::fromLocal8Bit("�����֣�"));
    sumScore = new QLineEdit;

    registerField("first", firstScore);
    registerField("second", secondScore);
    registerField("third", thirdScore);
    registerField("forth", forthScore);
    registerField("fifth", fifthScore);
    registerField("sixth", sixthScore);
    registerField("sum", sumScore);

    QGridLayout *layout = new QGridLayout;
    layout->setColumnStretch(0, 1);
    layout->addWidget(firstLabel, 0, 0);
    layout->addWidget(firstScore, 0, 1);
    layout->addWidget(secondLabel, 1, 0);
    layout->addWidget(secondScore, 1, 1);
    layout->addWidget(thirdLabel, 2, 0);
    layout->addWidget(thirdScore, 2, 1);
    layout->addWidget(forthLabel, 3, 0);
    layout->addWidget(forthScore, 3, 1);
    layout->addWidget(fifthLabel, 4, 0);
    layout->addWidget(fifthScore, 4, 1);
    layout->addWidget(sixthLabel, 5, 0);
    layout->addWidget(sixthScore, 5, 1);
    layout->addWidget(sumLabel, 6, 0);
    layout->addWidget(sumScore, 6, 1);
    setLayout(layout);
}

void KneeScoresPage::cleanupPage(){
}

void KneeScoresPage::initializePage(){
    int sum = 0;
    sum = firstScore->text().toInt() + secondScore->text().toInt() + thirdScore->text().toInt()
            + forthScore->text().toInt() + fifthScore->text().toInt() + sixthScore->text().toInt();
    setField("sum", sum);
}

bool KneeScoresPage::validatePage() {
    return true;
}
