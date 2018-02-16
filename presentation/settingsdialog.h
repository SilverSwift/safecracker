#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QGroupBox;
class QSlider;

namespace presentation{

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

    QSize sizeHint() const override;

signals:
    void durationChanged(int);

private slots:
    void onAccepted();

private:
    void initComponents();
    QGroupBox* sliderBox(QSlider* slider, QString title);


    const int mMaxDuaration = 1500;
    const int mMaxDivider = 10;
    const int mMinDivider = 1;

    int mDuration;

    QSlider* pSpeed;
    QSlider* pSize;


};

}
#endif // SETTINGSDIALOG_H
