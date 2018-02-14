#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>

namespace domain{
    class RatingModel;
    class Stats;
}

namespace fridge{
    class Fridge;
}

namespace presentation{
class MainWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);

signals:

private slots:
    void onGameStatsTriggered();
    void onFinished();
    void startNewGame();
    void onUpdatePoints();

private:
    void initComponents();

    fridge::Fridge* pFridgeItem;
    domain::Stats* pStats;
    domain::RatingModel* pModel;

};

}
#endif // MAINWIDGET_H
