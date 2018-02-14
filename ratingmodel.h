#ifndef RATINGMODEL_H
#define RATINGMODEL_H

#include "common_traits.h"

#include <QAbstractTableModel>
#include <QHash>
#include <QVector>

namespace domain{

class RatingModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    explicit RatingModel(QObject *parent = 0);
    ~RatingModel();

    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    virtual QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent) const override;

public slots:
    void addResult(QString userName, int time, int points);

private:
    bool loadData();
    bool saveData();
    void sortUsers();

    QJsonArray usersArray() const;

    QVector <QObject*> mItems;

    const QHash <int, const char*> mProperties = {
        {Columns::UserName, "UserName"},
        {Columns::RoundTime, "RoundTime"},
        {Columns::Points, "Points"},
        {Columns::DateTime, "DateTime"}
    };

    bool mLoaded = false;
};

}
#endif // RATINGMODEL_H
