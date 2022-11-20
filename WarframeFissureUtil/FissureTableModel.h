#pragma once
#include "Fissure.h"
#include <QAbstractTableModel>
#include <QString>
#include <QList>
class FissureTableModel :
    public QAbstractTableModel
{
    Q_OBJECT
public:
    FissureTableModel(QObject* parent = NULL);
    FissureTableModel(const QList<Fissure>&, QObject* parent = 0);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex&,int role = Qt::DisplayRole) const;
    QVariant headerData(int, Qt::Orientation,int role = Qt::DisplayRole) const;
    void Clear();
    void SetData(const QList<Fissure>);
    Fissure fissureFromIndex(int row) const;
private:
    QList<Fissure> fissures;
};

