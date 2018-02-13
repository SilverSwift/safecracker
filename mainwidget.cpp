#include "mainwidget.h"
#include "menubar.h"
#include "fridge.h"
#include "presentation_traits.h"
#include "stats.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QVBoxLayout>

using namespace presentation;

MainWidget::MainWidget(QWidget *parent) : QMainWindow(parent)
  , fridgeItem (new fridge::Fridge())
  , stats (new domain::Stats(this))
{  
    this->initComponents();
}

void MainWidget::onFinished()
{
    QMessageBox::information(this, tr("Completed!"), stats->result());
    this->startNewGame();
}

void MainWidget::startNewGame()
{
    bool ok;
    int size = QInputDialog::getInt(this, tr("Select field size"), tr("Field size:"),
                         minSize, minSize, maxSize, 1, &ok);
    if (!ok)
        return;

    fridgeItem->initialize(size);

}

void MainWidget::onUpdatePoints()
{
    this->statusBar()->showMessage(stats->statusInfo());
}

void MainWidget::initComponents()
{
    MenuBar* menu = new MenuBar();
    this->setMenuBar(menu);

    this->setCentralWidget(fridgeItem);

    this->setStatusBar(new QStatusBar());

    connect(fridgeItem, &fridge::Fridge::canRedoChanged,
            menu, &MenuBar::enableRedo);
    connect(fridgeItem, &fridge::Fridge::canUndoChanged,
            menu, &MenuBar::enableUndo);

    emit fridgeItem->canRedoChanged(fridgeItem->canRedo());
    emit fridgeItem->canUndoChanged(fridgeItem->canUndo());

    connect(menu, &MenuBar::redoTriggered,
            fridgeItem, &fridge::Fridge::redo);

    connect(menu, &MenuBar::undoTriggered,
            fridgeItem, &fridge::Fridge::undo);

    connect(menu, &MenuBar::newGameTriggered,
            this, &MainWidget::startNewGame);
    connect(fridgeItem, &fridge::Fridge::finished,
            this, &MainWidget::onFinished);

    connect(fridgeItem, &fridge::Fridge::started,
            stats, &domain::Stats::onStarted);
    connect(fridgeItem, &fridge::Fridge::finished,
            stats, &domain::Stats::onFinished);
    connect(fridgeItem, &fridge::Fridge::pressed,
            stats, &domain::Stats::onPressed);

    connect(stats, &domain::Stats::updatePoints,
            this, &MainWidget::onUpdatePoints);

}
