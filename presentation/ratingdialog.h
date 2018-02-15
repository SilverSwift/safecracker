#ifndef RATINGDIALOG_H
#define RATINGDIALOG_H

#include <QDialog>
class QAbstractItemModel;

namespace presentation{

class RatingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RatingDialog(QAbstractItemModel* model, QWidget *parent = 0);
    QSize sizeHint() const override;

};

}
#endif // RATINGDIALOG_H
