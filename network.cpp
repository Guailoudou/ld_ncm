#include "network.h"

network::network(QObject* parent) : QObject(parent)
{

}

void network::login_getkey(std::function<void(const QString& key)> callback)
{
    QString url = getApi_url() + "/login/qr/key" + "?timestamp=" + common::get_time();
    client.hget(url,
                [callback](const QByteArray & data)
    {
        qDebug() << "[GET Success]:" << data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject jsonObj = jsonDoc.object();
        QString key = jsonObj.value("data").toObject().value("unikey").toString();
        if(callback)
        {
            callback(key);
        }
    },
    [](const QString & err)
    {
        qDebug() << "[GET Error]:" << err;
    }
               );
}

void network::login_getQR(std::function<void(const QString& qrBase64)> callback)
{
    login_getkey([this, callback](const QString & key)
    {
        setLogin_key(key);
        QString url = getApi_url() + "/login/qr/create?key=" + getLogin_key() + "&timestamp=" + common::get_time() + "&qrimg=1";
        client.hget(url,
                    [callback](const QByteArray & data)
        {
            qDebug() << "[GET Success]:" << data;
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
            QJsonObject jsonObj = jsonDoc.object();
            QString qr = jsonObj.value("data").toObject().value("qrimg").toString();
            if(callback)
            {
                callback(qr);
            }
        },
        [](const QString & err)
        {
            qDebug() << "[GET Error]:" << err;
        }
                   );
    });
}

void network::login_check(std::function<void (const QJsonObject&)> callback)
{
    QString url = getApi_url() + "/login/qr/check?key=" + getLogin_key() + "&timestamp=" + common::get_time() + "&ua=pc";
    client.hget(url,
                [callback](const QByteArray & data)
    {
        qDebug() << "[GET Success]:" << data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject jsonObj = jsonDoc.object();
        if(callback)
        {
            callback(jsonObj);
        }
    },
    [](const QString & err)
    {
        qDebug() << "[GET Error]:" << err;
    }
               );
}

void network::geturl_Json(QString Url, std::function<void (const QJsonObject&)> callback)
{
    client.hget(Url,
                [callback](const QByteArray & data)
    {
        qDebug() << "[GET Success]:" << data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject jsonObj = jsonDoc.object();
        if(callback)
        {
            callback(jsonObj);
        }
    },
    [](const QString & err)
    {
        qDebug() << "[GET Error]:" << err;
    }
               );
}

void network::posturl_Json(QString Url, const QJsonObject& jsonData, std::function<void (const QJsonObject&)> callback)
{
    client.hpost(Url, jsonData,
                 [callback](const QByteArray & data)
    {
        qDebug() << "[POST Success]:" << data;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject jsonObj = jsonDoc.object();
        if(callback)
        {
            callback(jsonObj);
        }
    },
    [](const QString & err)
    {
        qDebug() << "[POST Error]:" << err;
    }
                );
}
void network::geturl_data(QString Url, std::function<void (const QByteArray&)> callback)
{
    client.hget(Url,
                [callback](const QByteArray & data)
    {
        if(callback)
        {
            callback(data);
        }
    },
    [](const QString & err)
    {
        qDebug() << "[GET Error]:" << err;
    }
               );
}

QString network::getApi_url() const
{
    return api_url;
}

void network::setApi_url(const QString& value)
{
    api_url = value;
}

QString network::getLogin_key() const
{
    return login_key;
}

void network::setLogin_key(const QString& value)
{
    login_key = value;
}

