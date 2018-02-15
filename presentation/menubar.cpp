#include "menubar.h"
#include <QApplication>

using namespace presentation;

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    this->initComponents();
}

void MenuBar::enableRedo(bool enabled)
{
    pRedo->setEnabled(enabled);
}

void MenuBar::enableUndo(bool enabled)
{
    pUndo->setEnabled(enabled);
}

QMenu* MenuBar::fileMenu()
{
    QMenu* file = new QMenu(tr("File"), this);
    file->addAction(tr("Settings"), this, SIGNAL(settingsTriggered()));
    file->addAction(tr("Quit"), qApp, SLOT(quit()), QKeySequence::Quit);

    return file;
}

QMenu* MenuBar::gameMenu()
{
    QMenu* game = new QMenu(tr("Game"), this);

    game->addAction(tr("New game"), this, SIGNAL(newGameTriggered()));
    pRedo = game->addAction(tr("Redo"), this, SIGNAL(redoTriggered()), QKeySequence::Redo);
    pUndo = game->addAction(tr("Undo"), this, SIGNAL(undoTriggered()), QKeySequence::Undo);
    game->addAction(tr("Game stats"), this, SIGNAL(gameStatsTriggered()));

    return game;
}

QMenu* MenuBar::helpMenu()
{
    QMenu* help = new QMenu(tr("Help"), this);

    help->addAction(tr("Help"), this, SIGNAL(helpTriggered()), QKeySequence::HelpContents);
    help->addAction(tr("About"), this, SIGNAL(aboutTriggered()));

    return help;
}

void MenuBar::initComponents()
{
    this->addMenu(this->fileMenu());
    this->addMenu(this->gameMenu());
    this->addMenu(this->helpMenu());
}

