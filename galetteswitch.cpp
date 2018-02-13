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

void GaletteSwitch::lock(bool locked)
{
    mLocked = locked;
}

void GaletteSwitch::paintEvent(QPaintEvent* )
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing |
                            QPainter::TextAntialiasing |
                            QPainter::SmoothPixmapTransform);

    QRect itemRect (this->x()*0.1, this->y()*0.4,
                this->width()*0.8, this->height()*0.2);

    painter.translate(this->rect().center());
    painter.rotate(mAngle);
    painter.translate(-itemRect.center());

    painter.setBrush(Qt::black);
    painter.setPen(Qt::black);
    QPoint center (itemRect.center());
    int radius = 0.5*itemRect.height();

    painter.drawEllipse(center, 2*radius, 2*radius);
    center.setX(center.x() - radius);
    painter.drawEllipse(center, 3*radius, radius);
    center.setX(center.x() + 2*radius);
    painter.drawEllipse(center, 3*radius, radius);

    painter.setBrush(Qt::Dense4Pattern);
    painter.setPen(Qt::transparent);

    center = QPoint(itemRect.center().x() + 0.2*radius,
                    itemRect.center().y() + 0.45*radius);

    painter.drawEllipse(center, 2*radius, 2*radius);
    center.setX(center.x() - radius);
    painter.drawEllipse(center, 3*radius, radius);
    center.setX(center.x() + 2*radius);
    painter.drawEllipse(center, 3*radius, radius);

}

void GaletteSwitch::mousePressEvent(QMouseEvent* event)
{
    event->accept();
    if (mLocked)
        return;

    if (event->button() == Qt::LeftButton)
        emit pressed();
}
