#include "ratingdialog.h"
#include "common_traits.h"

#include <QHeaderView>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>

using namespace presentation;

RatingDialog::RatingDialog(QAbstractItemModel* model, QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle(tr("Top players"));
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    QTableView *view = new QTableView(this);
    view->setModel(model);
    QHeaderView* header = view->horizontalHeader();
    header->setSectionResizeMode(header->count() - 1, QHeaderView::Stretch);
    view->sortByColumn(domain::Columns::RoundTime, Qt::DescendingOrder);
    view->resizeColumnsToContents();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(view);

    QPushButton* ok = new QPushButton(tr("OK"));
    layout->addWidget(ok, 0, Qt::AlignHCenter);

    connect(ok, &QPushButton::clicked, this, &RatingDialog::accept);
}

QSize RatingDialog::sizeHint() const
{
    return QSize(500, 200);
}
