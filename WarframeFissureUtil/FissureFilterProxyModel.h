#pragma once
#include <qsortfilterproxymodel.h>
#include "FissureTableModel.h"
class FissureFilterProxyModel :
    public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FissureFilterProxyModel(FissureTableModel* parent = nullptr);
    void UpdateFilters(QString& tier, QString& mode, QString& faction, int steelPath = 0, int voidStorm = 0);
protected:
    bool filterAcceptsRow(int, const QModelIndex&) const override;
private:
    FissureTableModel* fissureTable;
    QString tier;
    QString mode;
    QString faction;
    int steelPath;
    int voidStorm;
};

