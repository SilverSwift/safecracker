#include "ratingmodel.h"

#include <QDateTime>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <algorithm>

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
    return mProperties.count();
}

QVariant RatingModel::data(const QModelIndex& index, int role) const
{
    if (!this->hasIndex(index.row(), index.column()))
        return QVariant();

    switch(role){
        case Qt::DisplayRole:
            return mItems[index.row()]->property(mProperties[index.column()]);

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
        return mHeaderProperties[section];
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

void RatingModel::addResult(QString userName, int time, int points)
{
    QObject* object = new QObject(this);
    object->setProperty(mProperties[Columns::UserName], userName);
    object->setProperty(mProperties[Columns::RoundTime], QString::number(time));
    object->setProperty(mProperties[Columns::Points], QString::number(points));
    QString timeStr = QDateTime::currentDateTime().toString("dd MMM yyyy hh:mm");
    object->setProperty(mProperties[Columns::DateTime], timeStr);

    emit layoutAboutToBeChanged();
    mItems<<object;
    this->sortUsers();
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

        mItems<<userObject;

    }

    this->sortUsers();

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

void RatingModel::sortUsers()
{
    std::sort(mItems.begin(), mItems.end(), [=](QObject* left, QObject* right){

        bool leftOk, rightOk;
        int lValue = left->property(mProperties[Columns::RoundTime]).toString().toInt(&leftOk);
        int rValue = right->property(mProperties[Columns::RoundTime]).toString().toInt(&rightOk);

        if (!leftOk || !rightOk)
            return false;

        return lValue < rValue;

    });
    if (mItems.count() > 10)
        mItems.resize(10);
}

QJsonArray RatingModel::usersArray() const
{
    QJsonArray users;

    foreach (QObject* item, mItems) {
        QVariantMap itemObject;
        foreach (const char* property, mProperties.values())
            itemObject[QString(property)] = item->property(property);
        users.append(QJsonObject::fromVariantMap(itemObject));
    }

    return users;
}
