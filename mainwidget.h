#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>

namespace domain{
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
    void onFinished();
    void startNewGame();
    void onUpdatePoints();

private:
    void initComponents();

    fridge::Fridge* fridgeItem;
    domain::Stats* stats;

};

}
#endif // MAINWIDGET_H
