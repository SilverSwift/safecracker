#ifndef STARTGAMEDIALOG_H
#define STARTGAMEDIALOG_H

#include <QDialog>

class QLineEdit;
class QSpinBox;

namespace presentation{

class StartGameDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartGameDialog(QWidget *parent = 0);
    int fieldSize() const;
    QString playerName() const;

private:
    void initComponents();

    QSpinBox* pFieldSize;
    QLineEdit* pPlayerName;

};

}

#endif // STARTGAMEDIALOG_H
