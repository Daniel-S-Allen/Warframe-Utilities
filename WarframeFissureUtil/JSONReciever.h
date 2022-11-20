#pragma once
#include "QtCore"
#include <QtNetwork>
class JSONReciever
{
	//https://api.warframestat.us/pc/fissures/?language=en
	QJsonObject data;
public:
	static QJsonDocument receive(QUrl url);
};