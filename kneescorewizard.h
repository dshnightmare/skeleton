#ifndef KNEESCOREWIZARD_H
#define KNEESCOREWIZARD_H

#include <QWizard>
#include <vector>
#include <iostream>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QComboBox;
class QButtonGroup;
class QRadioButton;
class QProgressBar;
class QTimer;
QT_END_NAMESPACE
/*
QWizard::WatermarkPixmap    0   The tall pixmap on the left side of a ClassicStyle or ModernStyle page
QWizard::LogoPixmap         1   The small pixmap on the right side of a ClassicStyle or ModernStyle page header
QWizard::BannerPixmap       2   The pixmap that occupies the background of a ModernStyle page header
QWizard::BackgroundPixmap   3   The pixmap that occupies the background of a MacStyle wizard
*/
class KneeScoreWizard : public QWizard
{
    Q_OBJECT

public:
    KneeScoreWizard(QWidget *parent = 0, QString side = "");
    void accept() Q_DECL_OVERRIDE;
signals:
    void scoreChanged(QString);
};

class KneeIntroPage : public QWizardPage{
    Q_OBJECT

public:
    KneeIntroPage(QWidget *parent = 0);
private:
    QLabel *introLabel;
};

class KneePainLevelPage : public QWizardPage{
    Q_OBJECT

public:
    KneePainLevelPage(QWidget *parent = 0);
    bool validatePage() Q_DECL_OVERRIDE;
private:
    QLabel *topLabel;
    QComboBox *painComboBox;
};

class KneeStabilityPage : public QWizardPage{
    Q_OBJECT

public:
    KneeStabilityPage(QWidget *parent = 0);
    bool isComplete() const Q_DECL_OVERRIDE;
    bool validatePage() Q_DECL_OVERRIDE;
private slots:
    void completeChangedFromButton(bool);
    void completeChangedFromComboBox(int);
private:
    QLabel *fbLabel;
    QButtonGroup *fbGroup;
    QRadioButton *fbStableButton;
    QRadioButton *fbUnstableButton;
    QComboBox *fbStableComboBox;
    QLabel *ioLabel;
    QButtonGroup *ioGroup;
    QRadioButton *ioStableButton;
    QRadioButton *ioUnstableButton;
    QComboBox *ioStableComboBox;
};

class KneeValgusPage : public QWizardPage {
    Q_OBJECT

public:
    KneeValgusPage(QWidget *parent = 0);
    bool isComplete() const Q_DECL_OVERRIDE;
    bool validatePage() Q_DECL_OVERRIDE;
private slots:
    void progressChanged();
    void on_click_start();
    void setValgus(int);
    void validateNext(int);
signals:
    void start_Kinect();
    void stop_Kinect();
private:
    QLabel *topLabel;
    QPushButton *startButton;
    QProgressBar *cutdownBar;
    QTimer *timer;
    std::vector<int> allValgus;
};

class KneeScoresPage : public QWizardPage{
    Q_OBJECT

public:
    KneeScoresPage(QWidget *parent = 0);

    void cleanupPage() Q_DECL_OVERRIDE;
    void initializePage() Q_DECL_OVERRIDE;
    bool validatePage() Q_DECL_OVERRIDE;

private:
    QLabel *firstLabel;
    QLineEdit *firstScore;
    QLabel *secondLabel;
    QLineEdit *secondScore;
    QLabel *thirdLabel;
    QLineEdit *thirdScore;
    QLabel *forthLabel;
    QLineEdit *forthScore;
    QLabel *fifthLabel;
    QLineEdit *fifthScore;
    QLabel *sixthLabel;
    QLineEdit *sixthScore;
    QLabel *sumLabel;
    QLineEdit *sumScore;
};



#endif // KNEESCOREWIZARD_H
