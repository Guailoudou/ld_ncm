#ifndef USERDATA_H
#define USERDATA_H
#include <QJsonObject>
#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QDebug>
class userData
{
public:
    userData();
    QString getCookie() const;
    void setCookie(const QString& value);

    QJsonObject getData() const;
    void setData(const QJsonObject& value);

    void getfile();
    void savefile();

private:
    QString cookie;
    QJsonObject data;
};

#endif // USERDATA_H
