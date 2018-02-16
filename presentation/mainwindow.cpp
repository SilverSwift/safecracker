#include "mainwindow.h"
#include "menubar.h"
#include "fridge.h"
#include "common_traits.h"
#include "ratingdialog.h"
#include "ratingmodel.h"
#include "settingsdialog.h"
#include "settingsholder.h"
#include "stats.h"
#include "startgamedialog.h"

#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QVBoxLayout>

using namespace presentation;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
  , pFridgeItem (new fridge::Fridge())
  , pStats (new domain::Stats(this))
  , pModel (new domain::RatingModel(this))
{  
    this->setWindowTitle(tr("Safecracker Game"));
    this->setMinimumSize(480, 320);
    this->initComponents();
    this->statusBar()->addPermanentWidget(new QLabel(tr("Press Game->New game to start"), this->statusBar()));
}

QSize MainWindow::sizeHint() const
{
    return QSize(800, 600);
}

void MainWindow::onAboutTriggered()
{
    QMessageBox::information(this, tr("About"), tr("Made just for fun by Albert Perekhodchenko."));
}

void MainWindow::onGameStatsTriggered()
{
    RatingDialog(pModel).exec();
}

void MainWindow::onSettingsTriggered()
{
    SettingsDialog().exec();
}

void MainWindow::onFinished()
{
    pStats->onFinished();
    pModel->addResult(settings->userName(), pStats->gameTime(), pStats->points());
    QMessageBox::information(this, tr("Completed!"), pStats->result());
    this->startNewGame();
}

void MainWindow::startNewGame()
{
    if (StartGameDialog().exec()  != QDialog::Rejected)
        pFridgeItem->initialize();
}

void MainWindow::onUpdatePoints()
{
    this->statusBar()->showMessage(pStats->statusInfo());
}

void MainWindow::initComponents()
{
    MenuBar* menu = new MenuBar();
    this->setMenuBar(menu);

    this->setCentralWidget(pFridgeItem);

    this->setStatusBar(new QStatusBar());

    connect(pFridgeItem, &fridge::Fridge::canRedoChanged,
            menu, &MenuBar::enableRedo);
    connect(pFridgeItem, &fridge::Fridge::canUndoChanged,
            menu, &MenuBar::enableUndo);

    emit pFridgeItem->canRedoChanged(pFridgeItem->canRedo());
    emit pFridgeItem->canUndoChanged(pFridgeItem->canUndo());

    connect(menu, &MenuBar::redoTriggered,
            pFridgeItem, &fridge::Fridge::redo);

    connect(menu, &MenuBar::undoTriggered,
            pFridgeItem, &fridge::Fridge::undo);

    connect(menu, &MenuBar::newGameTriggered,
            this, &MainWindow::startNewGame);

    connect(menu, &MenuBar::gameStatsTriggered,
            this, &MainWindow::onGameStatsTriggered);

    connect(menu, &MenuBar::settingsTriggered,
            this, &MainWindow::onSettingsTriggered);

    connect(menu, &MenuBar::aboutTriggered,
            this, &MainWindow::onAboutTriggered);

    connect(pFridgeItem, &fridge::Fridge::finished,
            this, &MainWindow::onFinished);

    connect(pFridgeItem, &fridge::Fridge::started,
            pStats, &domain::Stats::onStarted);

    connect(pFridgeItem, &fridge::Fridge::pressed,
            pStats, &domain::Stats::onPressed);

    connect(pStats, &domain::Stats::updatePoints,
            this, &MainWindow::onUpdatePoints);

}
