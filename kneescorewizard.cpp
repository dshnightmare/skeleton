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
    setWindowTitle(QString::fromLocal8Bit("膝关节（") + side + QString::fromLocal8Bit("）"));
    setButtonText(QWizard::NextButton, QString::fromLocal8Bit("下一步"));
    setButtonText(QWizard::FinishButton, QString::fromLocal8Bit("完成"));
    setButtonText(QWizard::CancelButton, QString::fromLocal8Bit("取消"));
}

void KneeScoreWizard::accept() {
    emit scoreChanged(field("sum").toString());
    QDialog::accept();
}

KneeIntroPage::KneeIntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("说明"));

    introLabel = new QLabel(QString::fromLocal8Bit("本评分表分为两部分第一部分由人工填写，第二部分由机器自动给出，综合得到最终结果。"));
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(introLabel);
    setLayout(layout);
}

KneePainLevelPage::KneePainLevelPage(QWidget *parent)
    : QWizardPage(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    setTitle(QString::fromLocal8Bit("疼痛程度"));

    topLabel = new QLabel(QString::fromLocal8Bit("请选择："));
    painComboBox = new QComboBox;
    painComboBox->addItem(QString::fromLocal8Bit("无"));
    painComboBox->addItem(QString::fromLocal8Bit("轻微或偶尔"));
    painComboBox->addItem(QString::fromLocal8Bit("上下楼梯时疼痛"));
    painComboBox->addItem(QString::fromLocal8Bit("行走和上下楼梯时疼痛"));
    painComboBox->addItem(QString::fromLocal8Bit("中等疼痛，偶尔"));
    painComboBox->addItem(QString::fromLocal8Bit("中等，持续"));
    painComboBox->addItem(QString::fromLocal8Bit("剧烈"));
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
    setTitle(QString::fromLocal8Bit("稳定性"));

    fbLabel = new QLabel(QString::fromLocal8Bit("前后稳定性："));
    fbGroup = new QButtonGroup;
    fbStableButton = new QRadioButton(QString::fromLocal8Bit("稳定"));
    fbGroup->addButton(fbStableButton);
    fbUnstableButton = new QRadioButton(QString::fromLocal8Bit("不稳定"));
    fbGroup->addButton(fbUnstableButton);
    fbStableComboBox = new QComboBox;
    fbStableComboBox->addItem(QString::fromLocal8Bit("请选择稳定性程度"));
    fbStableComboBox->addItem("<5mm");
    fbStableComboBox->addItem("5-10mm");
    fbStableComboBox->addItem(">10mm");
    fbStableComboBox->setCurrentIndex(0);
    connect(fbStableButton, SIGNAL(toggled(bool)), fbStableComboBox, SLOT(setHidden(bool)));
    fbStableButton->setChecked(true);
    connect(fbStableButton, SIGNAL(toggled(bool)), this, SLOT(completeChangedFromButton(bool)));
    connect(fbStableComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(completeChangedFromComboBox(int)));

    ioLabel = new QLabel(QString::fromLocal8Bit("内外侧稳定性："));
    ioGroup = new QButtonGroup;
    ioStableButton = new QRadioButton(QString::fromLocal8Bit("稳定"));
    ioGroup->addButton(ioStableButton);
    ioUnstableButton = new QRadioButton(QString::fromLocal8Bit("不稳定"));
    ioGroup->addButton(ioUnstableButton);
    ioStableComboBox = new QComboBox;
    ioStableComboBox->addItem(QString::fromLocal8Bit("请选择稳定性程度"));
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
    setTitle(QString::fromLocal8Bit("外翻力线"));

    topLabel = new QLabel(QString::fromLocal8Bit("请保持竖直站立10秒"));
    startButton = new QPushButton;
    startButton->setText(QString::fromLocal8Bit("点击开始计时"));
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
    setTitle(QString::fromLocal8Bit("评分汇总"));

    firstLabel = new QLabel(QString::fromLocal8Bit("疼痛程度："));
    firstScore = new QLineEdit;
    firstScore->setReadOnly(true);
    secondLabel = new QLabel(QString::fromLocal8Bit("活动范围："));
    secondScore = new QLineEdit;
    secondScore->setReadOnly(true);
    thirdLabel = new QLabel(QString::fromLocal8Bit("前后稳定性："));
    thirdScore = new QLineEdit;
    thirdScore->setReadOnly(true);
    forthLabel = new QLabel(QString::fromLocal8Bit("内外侧稳定性："));
    forthScore = new QLineEdit;
    forthScore->setReadOnly(true);
    fifthLabel = new QLabel(QString::fromLocal8Bit("屈曲挛缩畸形："));
    fifthScore = new QLineEdit;
    fifthScore->setReadOnly(true);
    sixthLabel = new QLabel(QString::fromLocal8Bit("外翻力线："));
    sixthScore = new QLineEdit;
    sixthScore->setReadOnly(true);
    sumLabel = new QLabel(QString::fromLocal8Bit("总评分："));
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
