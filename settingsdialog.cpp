#include "settingsdialog.h"
#include "settingsholder.h"
using namespace presentation;

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
  , pSpinBox (new QSpinBox(this))
{

    this->setWindowTitle(tr("Settings"));

    pSpinBox->setMinimum(mMinDivider);
    pSpinBox->setMaximum(mMaxDivider);
    pSpinBox->setValue(mMaxDuaration/domain::SettingsHolder::instance()->duration());
    pSpinBox->setMaximumWidth(100);

    QPushButton *ok = new QPushButton(tr("OK"), this);
    QPushButton *cancel = new QPushButton(tr("Cancel"), this);

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel(tr("Animation speed:")));
    layout->addWidget(pSpinBox);
    layout->addStretch();

    QHBoxLayout *bottomRow = new QHBoxLayout();
    bottomRow->setAlignment(Qt::AlignHCenter);
    bottomRow->addWidget(ok);
    bottomRow->addWidget(cancel);
    layout->addLayout(bottomRow);

    connect(ok, &QPushButton::clicked, this, &SettingsDialog::accept);
    connect(cancel, &QPushButton::clicked, this, &SettingsDialog::reject);
    connect(this, &SettingsDialog::accepted, this, &SettingsDialog::onAccepted);
}

QSize SettingsDialog::sizeHint() const
{
    return QSize(350, 250);
}

void SettingsDialog::onAccepted()
{
    int duration = mMaxDuaration/pSpinBox->value();
    emit durationChanged(duration);
}

