#include "FissureTableModel.h"

FissureTableModel::FissureTableModel(QObject* parent)
{
}

FissureTableModel::FissureTableModel(const QList<Fissure>& fissureList, QObject* parent): QAbstractTableModel(parent)
{
	SetData(fissureList);
}

Q_INVOKABLE int FissureTableModel::rowCount(const QModelIndex& parent) const
{
	return fissures.size();
}

Q_INVOKABLE int FissureTableModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent)
	return 7;
}

Q_INVOKABLE QVariant FissureTableModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (index.row() >= fissures.size() || index.row() < 0)
		return QVariant();

	if (role == Qt::DisplayRole) {
		const Fissure& fissure = fissures.at(index.row());
		switch (index.column()) {
		case 0:
			return fissure.node;
		case 1:
			return fissure.type;
		case 2:
			return fissure.tier;
		case 3:
			return fissure.enemy;
		case 4:
			return fissure.time;
		case 5:
			return fissure.steelPath;
		case 6:
			return fissure.voidStorm;
		default:
			return QVariant();
		}
	}
	else if (role == Qt::TextAlignmentRole) {
		if (index.column() == 0)
			return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
		else
			return Qt::AlignCenter;
	}
	return QVariant();
}

QVariant FissureTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return tr("Node");
		case 1:
			return tr("Mission Type");
		case 2:
			return tr("Tier");
		case 3:
			return tr("Faction");
		case 4:
			return tr("Time Remaining");
		case 5:
			return tr("Steel Path");
		case 6:
			return tr("Void Storm");
		default:
			return QVariant();
		}
	}
	return section + 1;
}

void FissureTableModel::Clear()
{
	fissures.clear();

}

void FissureTableModel::SetData(const QList<Fissure> fissureList)
{
	this->fissures = fissureList;
	std::sort(fissures.begin(), fissures.end(), [&](const Fissure& a, const Fissure& b)->int{
		int res = a.node.compare(b.node);
		if (res < 0)
			return 1;
		return 0;
	});
	emit dataChanged(index(0, 0), index(rowCount(), columnCount()));
}

Fissure FissureTableModel::fissureFromIndex(int row) const
{
	if (row >= fissures.size() || row < 0)
		return Fissure("error", "error", "error", "error", "error", false, false);
	return fissures.at(row);
}
