#include "JSONReciever.h"

QJsonDocument JSONReciever::receive(QUrl url)
{
    QNetworkAccessManager nam;
    QNetworkReply* response = nam.get(QNetworkRequest(url));
    QEventLoop event;
    QObject::connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QString content = response->readAll();
    return QJsonDocument::fromJson(content.toUtf8());
}