#include "ratingmodel.h"
#include "presentation_traits.h"

#include <QAbstractItemModel>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedPointer>

using namespace domain;

RatingModel::RatingModel(QObject *parent) : QAbstractTableModel(parent)
{
    this->loadData();

}

RatingModel::~RatingModel()
{
    this->saveData();
}

int RatingModel::columnCount(const QModelIndex& ) const
{
    return mProperties.count() - 1;
}

QVariant RatingModel::data(const QModelIndex& index, int role) const
{
    if (!this->hasIndex(index.row(), index.column()))
        return QVariant();

    switch(role){
        case Qt::DisplayRole:
            return mItems[index.row()]->property(mProperties[index.column() + 1]);

        default:
            return QVariant();
    }

    return QVariant();
}

Qt::ItemFlags RatingModel::flags(const QModelIndex& ) const
{
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

QVariant RatingModel::headerData(int section,
                                 Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return mProperties[section + 1];
    else
        return (section + 1);

    return QVariant();
}

QModelIndex RatingModel::parent(const QModelIndex& ) const
{
    return QModelIndex();
}

int RatingModel::rowCount(const QModelIndex& ) const
{
    return mItems.count();
}

bool RatingModel::addResult(QString userName, int time, int points)
{
    QObject* object = new QObject(this);
    int id = this->getId();
    object->setProperty(mProperties[Columns::id], id);
    object->setProperty(mProperties[UserName], userName);
    object->setProperty(mProperties[RoundTime], QString::number(time));
    object->setProperty(mProperties[Points], QString::number(points));
    QString timeStr = QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm");
    object->setProperty(mProperties[DateTime], timeStr);

    emit layoutAboutToBeChanged();
    mItems[id] = object;
    emit layoutChanged();


}

bool RatingModel::loadData()
{
    QFile file (ratingFilePath);

    if (!file.open(QFile::ReadOnly))
        return false;

    QJsonObject root = QJsonDocument::fromJson(file.readAll()).object();

    QJsonArray users = root.value(usersArrayKey).toArray();

    for (QJsonArray::iterator i = users.begin(); i != users.end(); i++){
        QJsonObject user = (*i).toObject();
        if (user.isEmpty())
            continue;
        QObject* userObject = new QObject(this);
        foreach (const char* property, mProperties.values())
            userObject->setProperty(property, user.value(property));

        int id =  userObject->property(mProperties[Columns::id]).toInt();

        mItems[id] = userObject;

    }

    return true;
}

bool RatingModel::saveData()
{
    QFile file(ratingFilePath);
    if (!file.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    QJsonArray users = this->usersArray();

    QJsonObject root {{usersArrayKey, users}};

    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();
    return true;
}

int RatingModel::getId() const
{
    int id = 0;
    while (mItems.contains(id))
        id++;

    return id;
}

QJsonArray RatingModel::usersArray() const
{
    QJsonArray users;

    foreach (QObject* item, mItems.values()) {
        QVariantMap itemObject;
        foreach (const char* property, mProperties.values())
            itemObject[QString(property)] = item->property(property);
        users.append(QJsonObject::fromVariantMap(itemObject));
    }

    return users;
}
