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

private slots:
    void onAccepted();

private:
    void initComponents();

    QSpinBox* pFieldSize;
    QLineEdit* pUserName;

};

}

#endif // STARTGAMEDIALOG_H
