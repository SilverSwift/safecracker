#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

class QAction;

namespace presentation {

class MenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = 0);

signals:
    void aboutTriggered();
    void gameStatsTriggered();
    void helpTriggered();
    void newGameTriggered();
    void redoTriggered();
    void settingsTriggered();
    void undoTriggered();

public slots:
    void enableRedo(bool enabled = true);
    void enableUndo(bool enabled = true);

private:
    QMenu* fileMenu();
    QMenu* gameMenu();
    QMenu* helpMenu();

    QAction* pRedo;
    QAction* pUndo;

    void initComponents();
};

}
#endif // MENUBAR_H
