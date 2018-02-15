#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>

namespace domain{
    class RatingModel;
    class Stats;
}


namespace presentation{
namespace fridge{
    class Fridge;
}

class SettingsDialog;

class MainWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    QSize sizeHint() const override;
signals:

private slots:
    void onGameStatsTriggered();
    void onFinished();
    void onSettingsTriggered();
    void onUpdatePoints();
    void startNewGame();

private:
    void initComponents();

    fridge::Fridge* pFridgeItem;
    domain::Stats* pStats;
    domain::RatingModel* pModel;
    SettingsDialog* pSettings;

};

}
#endif // MAINWIDGET_H
