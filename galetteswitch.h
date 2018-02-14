#ifndef GALETTESWITCH_H
#define GALETTESWITCH_H

#include <QWidget>

namespace fridge{

class GaletteSwitch : public QWidget
{
    Q_OBJECT
public:
    explicit GaletteSwitch(QWidget *parent = 0);

    Q_PROPERTY(int rotation READ rotation
                            WRITE setRotation
                            NOTIFY rotationChanged)

    int rotation() const;

public slots:
    void setRotation(int angle);

signals:
    void pressed();
    void rotationChanged();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;

private:
    int mAngle = 0;

};

}
#endif // GALETTESWITCH_H
