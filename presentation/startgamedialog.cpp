#include "startgamedialog.h"
#include "common_traits.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QVBoxLayout>

using namespace presentation;

StartGameDialog::StartGameDialog(QWidget *parent) : QDialog(parent)
  , pFieldSize (new QSpinBox(this))
  , pPlayerName (new QLineEdit(this))
{
    this->initComponents();
}

int StartGameDialog::fieldSize() const
{
    return pFieldSize->value();
}

QString StartGameDialog::playerName() const
{
    return pPlayerName->text();
}

void StartGameDialog::initComponents()
{
    this->setWindowTitle(tr("Start new game"));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Player:")));
    layout->addWidget(pPlayerName);
    layout->addWidget(new QLabel(tr("Field size:")));
    layout->addWidget(pFieldSize);

    QHBoxLayout* bottomRow = new QHBoxLayout();
    QPushButton* start = new QPushButton(tr("Start"));
    QPushButton* abort = new QPushButton(tr("Abort"));
    bottomRow->addWidget(start);
    bottomRow->addWidget(abort);

    layout->addLayout(bottomRow);

    pFieldSize->setMinimum(presentation::minSize);
    pFieldSize->setMaximum(presentation::maxSize);

    connect(pPlayerName, &QLineEdit::textChanged,
            this,[=](QString text){start->setEnabled(!text.isEmpty());});
    connect(start, &QPushButton::clicked, this, &StartGameDialog::accept);
    connect(abort, &QPushButton::clicked, this, &StartGameDialog::reject);

    connect(this, &StartGameDialog::accepted, this, [=](){
        QSettings().setValue(lastPlayerKey, pPlayerName->text());
    });

    QString player = QSettings().value(lastPlayerKey).toString();
    pPlayerName->setText(player);

    emit pPlayerName->textChanged(pPlayerName->text());

}

