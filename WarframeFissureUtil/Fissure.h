#pragma once
#include <QString>
#include <QList>
#include "JSONReciever.h"
class Fissure
{
public:
	QString node;
	QString type;
	QString tier;
	QString enemy;
	QString time;
	bool steelPath = false;
	bool voidStorm = false;
	bool active = true;

	//todo time stuff

	static QList<Fissure> currentFissures();
	Fissure(QString name, QString type, QString relicTier, QString faction, QString time, bool sp, bool vs);
};