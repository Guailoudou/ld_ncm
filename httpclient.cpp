#include "httpclient.h"
#include <QNetworkCookieJar>

HttpClient::HttpClient(QObject* parent) : QNetworkAccessManager(parent) {}

void HttpClient::hget(const QString& url,
                      std::function<void(const QByteArray&)> onSuccess,
                      std::function<void(const QString&)> onError)
{
    QNetworkRequest request{QUrl(url)};

    // 发起 GET 请求
    QNetworkReply* reply = QNetworkAccessManager::get(request);

    connect(reply, &QNetworkReply::finished, [reply, onSuccess, onError]()
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            if(onSuccess)
            {
                onSuccess(reply->readAll());
            }
        }
        else
        {
            if(onError)
            {
                onError(reply->errorString());
            }
        }
        reply->deleteLater(); // 释放内存
    });
}

void HttpClient::hpost(const QString& url, const QJsonObject& jsonData,
                       std::function<void(const QByteArray&)> onSuccess,
                       std::function<void(const QString&)> onError)
{
    QNetworkRequest request{QUrl(url)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QByteArray data = QJsonDocument(jsonData).toJson(QJsonDocument::Compact);

    // 发起 POST 请求
    QNetworkReply* reply = QNetworkAccessManager::post(request, data);

    connect(reply, &QNetworkReply::finished, [reply, onSuccess, onError]()
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            if(onSuccess)
            {
                onSuccess(reply->readAll());
            }
        }
        else
        {
            if(onError)
            {
                onError(reply->errorString());
            }
        }
        reply->deleteLater(); // 释放内存
    });
}

void HttpClient::setCookies(const QList<QNetworkCookie>& cookies)
{
    // 获取默认的 Cookie Jar 并设置 Cookie
    QNetworkCookieJar* jar = this->cookieJar();
    for(const auto& cookie : cookies)
    {
        jar->insertCookie(cookie);
    }
}
