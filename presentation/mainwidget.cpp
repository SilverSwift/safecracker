#include "mainwidget.h"
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

MainWidget::MainWidget(QWidget *parent) : QMainWindow(parent)
  , pFridgeItem (new fridge::Fridge())
  , pStats (new domain::Stats(this))
  , pModel (new domain::RatingModel(this))
  , pSettings(new SettingsDialog(this))
{  
    this->initComponents();
    this->statusBar()->addPermanentWidget(new QLabel(intro, this->statusBar()));
}

QSize MainWidget::sizeHint() const
{
    return QSize(800, 600);
}

void MainWidget::onGameStatsTriggered()
{
    RatingDialog(pModel).exec();
}

void MainWidget::onSettingsTriggered()
{
    pSettings->exec();
}

void MainWidget::onFinished()
{
    pStats->onFinished();
    pModel->addResult(pStats->name(), pStats->gameTime(), pStats->points());
    QMessageBox::information(this, tr("Completed!"), pStats->result());
    this->startNewGame();
}

void MainWidget::startNewGame()
{

    StartGameDialog dialog;
    if (dialog.exec() == QDialog::Rejected)
        return;

    pStats->setName(dialog.playerName());

    int size = dialog.fieldSize();

    pFridgeItem->initialize(size);

}

void MainWidget::onUpdatePoints()
{
    this->statusBar()->showMessage(pStats->statusInfo());
}

void MainWidget::initComponents()
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
            this, &MainWidget::startNewGame);

    connect(menu, &MenuBar::gameStatsTriggered,
            this, &MainWidget::onGameStatsTriggered);

    connect(menu, &MenuBar::settingsTriggered,
            this, &MainWidget::onSettingsTriggered);

    connect(pFridgeItem, &fridge::Fridge::finished,
            this, &MainWidget::onFinished);

    connect(pFridgeItem, &fridge::Fridge::started,
            pStats, &domain::Stats::onStarted);

    connect(pFridgeItem, &fridge::Fridge::pressed,
            pStats, &domain::Stats::onPressed);

    connect(pStats, &domain::Stats::updatePoints,
            this, &MainWidget::onUpdatePoints);

    connect(pSettings, &SettingsDialog::durationChanged,
            domain::SettingsHolder::instance(),
            &domain::SettingsHolder::onDurationChanged);

}
