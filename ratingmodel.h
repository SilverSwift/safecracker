#ifndef RATINGMODEL_H
#define RATINGMODEL_H

#include <QAbstractTableModel>
#include <QHash>

namespace domain{

class RatingModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum Columns{
        id,
        UserName,
        RoundTime,
        Points,
        DateTime
    };

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
    bool addResult(QString userName, int time, int points);

private:
    bool loadData();
    bool saveData();

    int getId() const;

    QJsonArray usersArray() const;

    QHash <int, QObject* > mItems;

    const QHash <int, const char*> mProperties = {
        {id, "id"},
        {UserName, "UserName"},
        {RoundTime, "RoundTime"},
        {Points, "Points"},
        {DateTime, "DateTime"}
    };

    bool mLoaded = false;
};

}
#endif // RATINGMODEL_H
