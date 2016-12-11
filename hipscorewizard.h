#ifndef HIPSCOREWIZARD_H
#define HIPSCOREWIZARD_H

#include <QWizard>
#include <vector>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QComboBox;
class QProgressBar;
class QTimer;
QT_END_NAMESPACE
/*
QWizard::WatermarkPixmap    0   The tall pixmap on the left side of a ClassicStyle or ModernStyle page
QWizard::LogoPixmap         1   The small pixmap on the right side of a ClassicStyle or ModernStyle page header
QWizard::BannerPixmap       2   The pixmap that occupies the background of a ModernStyle page header
QWizard::BackgroundPixmap   3   The pixmap that occupies the background of a MacStyle wizard
*/
class HipScoreWizard : public QWizard
{
    Q_OBJECT

public:    
    HipScoreWizard(QWidget *parent = 0, QString side = "");
    void accept() Q_DECL_OVERRIDE;
signals:
    void scoreChanged(QString);
private:
};

class HipIntroPage : public QWizardPage {
    Q_OBJECT

public:
    HipIntroPage(QWidget *parent = 0);
private:
    QLabel *introLabel;
};

class HipPainLevelPage : public QWizardPage {
    Q_OBJECT

public:
    HipPainLevelPage(QWidget* parent = 0);
    bool validatePage() Q_DECL_OVERRIDE;

private:
    QLabel *topLabel;
    QComboBox *painComboBox;
};

class HipTreadPage : public QWizardPage {
    Q_OBJECT
public:
    HipTreadPage(QWidget *parent = 0);
    bool validatePage() Q_DECL_OVERRIDE;

private:
    QLabel *supportLabel;
    QComboBox *supportComboBox;
    QLabel *walkLabel;
    QComboBox *walkComboBox;
};

class HipFunctionPage : public QWizardPage{
    Q_OBJECT
public:
    HipFunctionPage(QWidget *parent = 0);
    bool validatePage() Q_DECL_OVERRIDE;

private:
    QLabel *upstairLabel;
    QComboBox *upstairComboBox;
    QLabel *sittingLabel;
    QComboBox *sittingComboBox;
    QLabel *vehicleLabel;
    QComboBox *vehicleComboBox;
};

class HipPutOnShoesPage : public QWizardPage {
    Q_OBJECT
public:
    HipPutOnShoesPage(QWidget *parent = 0);
    bool isComplete() const Q_DECL_OVERRIDE;
    bool validatePage() Q_DECL_OVERRIDE;
private slots:
    void progressChanged();
    void on_click_start();
    void on_click_giveup();
    void validateNext(int);
    void setShoes(int);
signals:
    void start_Kinect();
    void stop_Kinect();
private:
    QLabel *topLabel;
    QPushButton *startButton;
    QPushButton *giveupButton;
    QProgressBar *progressBar;
    QTimer *timer;
    int count1;
    int count2;
    int curShoes;

};

class HipScoresPage : public QWizardPage{
    Q_OBJECT
public:
    HipScoresPage(QWidget *parent = 0);

    void cleanupPage() Q_DECL_OVERRIDE;
    void initializePage() Q_DECL_OVERRIDE;
    bool validatePage() Q_DECL_OVERRIDE;

private:
    QLabel *Group1Label;
    QLineEdit *firstScore;
    QLabel *Group2Label;
    QLabel *Group21Label;
    QLineEdit *secondScore;
    QLabel *Group22Label;
    QLineEdit *thirdScore;
    QLabel *Group23Label;
    QLineEdit *forthScore;
    QLabel *Group3Label;
    QLabel *Group31Label;
    QLineEdit *fifthScore;
    QLabel *Group32Label;
    QLineEdit *sixthScore;
    QLabel *Group33Label;
    QLineEdit *seventhScore;
    QLabel *Group34Label;
    QLineEdit *eighthScore;
    QLabel *Group4Label;
    QLineEdit *ninthScore;
    QLabel *Group5Label;
    QLineEdit *tenthScore;
    QLabel *sumLabel;
    QLineEdit *sumScore;
};




#endif // HIPSCOREWIZARD_H
