#include "galetteswitch.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

using namespace fridge;

GaletteSwitch::GaletteSwitch(QWidget *parent) : QWidget(parent)
{
    connect(this, &GaletteSwitch::rotationChanged,
            this, [this](){this->update();});
}

int GaletteSwitch::rotation() const
{
    return mAngle;
}

void GaletteSwitch::setRotation(int angle)
{
    mAngle = angle;
    emit rotationChanged();
}

void GaletteSwitch::paintEvent(QPaintEvent* )
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform);

    painter.translate(this->rect().center());
    painter.rotate(mAngle);
    painter.translate(-this->rect().center());

    QPixmap pixmap(":/icons/switch2.png");
    painter.drawPixmap(this->rect(), pixmap);
}

void GaletteSwitch::mousePressEvent(QMouseEvent* event)
{
    event->accept();

    if (event->button() == Qt::LeftButton)
        emit pressed();
}
