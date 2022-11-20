#include "Fissure.h"

QList<Fissure> Fissure::currentFissures()
{
	QList<Fissure> output;
	QJsonDocument fissureData = JSONReciever::receive(QUrl("https://api.warframestat.us/PC/fissures"));
	QJsonArray fissures = fissureData.array();
	for (int i = 0; i < fissures.count(); i++) {
		QJsonObject val = fissures.at(i).toObject();

		QString node = val.value("node").toString();
		QString missionType = val.value("missionType").toString();
		QString tier = val.value("tier").toString();
		QString faction = val.value("enemy").toString();
		QString timeLeft = val.value("eta").toString();

		bool steelPath = val.value("isHard").toBool();
		bool voidStorm = val.value("isStorm").toBool();
		Fissure f(node, missionType, tier, faction, timeLeft, steelPath, voidStorm);
		output.push_back(f);
	}
	return output;
}

Fissure::Fissure(QString name, QString type, QString relicTier, QString faction, QString time, bool sp, bool vs)
{
	this->node = name;
	this->tier = relicTier;
	this->enemy = faction;
	this->type = type;
	this->time = time;
	this->steelPath = sp;
	this->voidStorm = vs;
}
