#include "ratingdialog.h"

#include <QHeaderView>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

using namespace presentation;

RatingDialog::RatingDialog(QAbstractItemModel* model, QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle(tr("Top players"));

    QTableView *view = new QTableView(this);
    view->setModel(model);
    QHeaderView* header = view->horizontalHeader();
    header->setSectionResizeMode(header->count() - 1, QHeaderView::Stretch);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(view);

    QPushButton* ok = new QPushButton(tr("OK"));
    layout->addWidget(ok, 0, Qt::AlignHCenter);

    connect(ok, &QPushButton::clicked, this, &RatingDialog::accept);
}

QSize RatingDialog::sizeHint() const
{
    return QSize(400, 200);
}
