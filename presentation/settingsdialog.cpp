#include "common_traits.h"
#include "settingsdialog.h"
#include "settingsholder.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

using namespace presentation;

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent)
  , pSpeed (new QSlider(this))
  , pSize (new QSlider(this))
{
    this->setWindowTitle(tr("Settings"));
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    this->initComponents();
}

QSize SettingsDialog::sizeHint() const
{
    return QSize(350, 250);
}

void SettingsDialog::onAccepted()
{
    int duration = mMaxDuaration/pSpeed->value();
    settings->setDuration(duration);
    settings->setFieldSize(pSize->value());
}

void SettingsDialog::initComponents()
{
    pSpeed->setMinimum(mMinDivider);
    pSpeed->setMaximum(mMaxDivider);
    pSpeed->setValue(mMaxDuaration/settings->duration());

    pSize->setMinimum(presentation::minFieldSize);
    pSize->setMaximum(presentation::maxFieldSize);
    pSize->setValue(settings->fieldSize());


    QPushButton *ok = new QPushButton(tr("OK"), this);
    QPushButton *cancel = new QPushButton(tr("Cancel"), this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(this->sliderBox(pSpeed, tr("Animation speed:")));
    layout->addWidget(this->sliderBox(pSize, tr("Field size:")));

    connect(ok, &QPushButton::clicked, this, &SettingsDialog::accept);
    connect(cancel, &QPushButton::clicked, this, &SettingsDialog::reject);
    connect(this, &SettingsDialog::accepted, this, &SettingsDialog::onAccepted);

    layout->addStretch();

    QHBoxLayout *bottomRow = new QHBoxLayout();
    bottomRow->setAlignment(Qt::AlignHCenter);
    bottomRow->addWidget(ok);
    bottomRow->addWidget(cancel);
    layout->addLayout(bottomRow);

}

QGroupBox*SettingsDialog::sliderBox(QSlider* slider, QString title)
{
    slider->setOrientation(Qt::Horizontal);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(qMax((slider->maximum() - slider->minimum())/5, 1) );

    QSpinBox* spinBox = new QSpinBox();
    spinBox->setMinimum(slider->minimum());
    spinBox->setMaximum(slider->maximum());
    spinBox->setValue(slider->value());

    connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            slider, &QSlider::setValue);
    connect(slider, &QSlider::valueChanged, spinBox, &QSpinBox::setValue);

    QHBoxLayout* spinBoxLayout = new QHBoxLayout();
    spinBoxLayout->addStretch();
    spinBoxLayout->addWidget(new QLabel(tr("Value:")));
    spinBoxLayout->addWidget(spinBox);

    QHBoxLayout* sliderBuddy = new QHBoxLayout();
    sliderBuddy->addWidget(new QLabel(tr("Min")));
    sliderBuddy->addStretch();
    sliderBuddy->addWidget(new QLabel(tr("Max")));

    QGroupBox* box = new QGroupBox(title, this);
    QVBoxLayout* boxLayout = new QVBoxLayout(box);
    boxLayout->addLayout(spinBoxLayout);
    boxLayout->addWidget(slider);
    boxLayout->addLayout(sliderBuddy);

    return box;
}
