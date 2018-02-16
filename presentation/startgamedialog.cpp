#include "startgamedialog.h"
#include "settingsholder.h"
#include "common_traits.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

using namespace presentation;

StartGameDialog::StartGameDialog(QWidget *parent) : QDialog(parent)
  , pFieldSize (new QSpinBox(this))
  , pUserName (new QLineEdit(this))
{
    this->initComponents();
}

void StartGameDialog::onAccepted()
{
    settings->setUserName(pUserName->text());
    settings->setFieldSize(pFieldSize->value());
}

void StartGameDialog::initComponents()
{
    this->setWindowTitle(tr("Start new game"));
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Player:")));
    layout->addWidget(pUserName);
    layout->addWidget(new QLabel(tr("Field size:")));
    layout->addWidget(pFieldSize);
    layout->addStretch();

    QHBoxLayout* bottomRow = new QHBoxLayout();
    bottomRow->setAlignment(Qt::AlignHCenter);
    QPushButton* start = new QPushButton(tr("Start"));
    QPushButton* abort = new QPushButton(tr("Abort"));
    bottomRow->addWidget(start);
    bottomRow->addWidget(abort);

    layout->addLayout(bottomRow);

    pFieldSize->setMinimum(presentation::minFieldSize);
    pFieldSize->setMaximum(presentation::maxFieldSize);
    pFieldSize->setValue(settings->fieldSize());

    connect(pUserName, &QLineEdit::textChanged,
            this,[=](QString text){start->setEnabled(!text.isEmpty());});
    connect(start, &QPushButton::clicked, this, &StartGameDialog::accept);
    connect(abort, &QPushButton::clicked, this, &StartGameDialog::reject);
    connect(this, &StartGameDialog::accepted, this, &StartGameDialog::onAccepted);

    pUserName->setText(settings->userName());

    emit pUserName->textChanged(pUserName->text());

}

