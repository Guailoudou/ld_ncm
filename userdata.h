#ifndef USERDATA_H
#define USERDATA_H
#include <QJsonObject>
#include <QObject>
class userData
{
public:
    userData();
    QString getCookie() const;
    void setCookie(const QString& value);

    QJsonObject getData() const;
    void setData(const QJsonObject& value);

private:
    QString cookie;
    QJsonObject data;
};

#endif // USERDATA_H
