#pragma once
#include <QtWidgets>
#include "FissureTableModel.h"
#include "FissureFilterProxyModel.h"
#include "src/qgoodwindow.h"
#include "ui_WarframeFissureUtil.h"

class WarframeFissureUtil : public QGoodWindow
{
    Q_OBJECT;
public:
    WarframeFissureUtil(QWidget *parent = nullptr, bool frameless = false);
    void setGlass(bool useGlass = true);
    void clearWidgets(QLayout* layout);
    
    ~WarframeFissureUtil();    
    
public slots:
    void UpdateFissureTable();
    void FilterFissures(QString type, QString tier, bool sp, bool vs);
    void UpdateFissureTableFilters();
private:
    FissureTableModel* model = nullptr;
    FissureFilterProxyModel* proxyModel = nullptr;
    Ui::UtilWindow ui;
};
