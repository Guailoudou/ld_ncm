#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <string>
#include "httpclient.h"
#include "common.h"
class network : public QObject
{
    Q_OBJECT
public:
    explicit network(QObject* parent = nullptr);
    void login_getkey(std::function<void(const QString& key)> callback);
    void login_getQR(std::function<void(const QString& qrBase64)> callback);
    void login_check(std::function<void(const QJsonObject& data)> callback);
    void geturl_data(QString Url, std::function<void(const QByteArray& data)> callback);
    QString getApi_url() const;
    void setApi_url(const QString& value);

    QString getLogin_key() const;
    void setLogin_key(const QString& value);


private:
    HttpClient client;
    QString api_url = "https://ncm-api.prod.gbclstudio.cn";
    QString login_key;
signals:

public slots:
};

#endif // NETWORK_H
