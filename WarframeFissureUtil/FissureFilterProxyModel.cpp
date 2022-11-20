#include "FissureFilterProxyModel.h"

FissureFilterProxyModel::FissureFilterProxyModel(FissureTableModel* parent) : QSortFilterProxyModel(parent)
{
	this->fissureTable = parent;
}
void FissureFilterProxyModel::UpdateFilters(QString& tier, QString& mode, QString& faction, int steelPath , int voidStorm) {
	this->tier = tier;
	this->mode = mode;
	this->faction = faction;
	this->steelPath = steelPath;
	this->voidStorm = voidStorm;
	this->invalidateFilter();
}
bool FissureFilterProxyModel::filterAcceptsRow(int row, const QModelIndex& parent) const
{
	const Fissure& entry = fissureTable->fissureFromIndex(row);
	if (QString::compare(tier, tr("Tier")) != 0 && QString::compare(tier, entry.tier, Qt::CaseInsensitive) != 0) {
		return false;
	}
	if (QString::compare(mode, tr("Gamemode")) != 0 && QString::compare(mode, entry.type, Qt::CaseInsensitive) != 0) {
		return false;
	}
	if (QString::compare(faction, tr("Faction")) != 0 && QString::compare(faction, entry.enemy, Qt::CaseInsensitive) != 0) {
		return false;
	}
	if (steelPath == Qt::PartiallyChecked && entry.steelPath || steelPath == Qt::Checked && !entry.steelPath)
		return false;
	if (voidStorm == Qt::PartiallyChecked && entry.voidStorm || voidStorm == Qt::Checked && !entry.voidStorm)
		return false;
	return true;
}
