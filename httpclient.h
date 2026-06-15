#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookie>
#include <QList>
#include <functional>

class HttpClient : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit HttpClient(QObject* parent = nullptr);

    // 发送 GET 请求
    void hget(const QString& url,
              std::function<void(const QByteArray&)> onSuccess = nullptr,
              std::function<void(const QString&)> onError = nullptr);

    // 发送 POST 请求 (JSON格式)
    void hpost(const QString& url, const QJsonObject& jsonData,
               std::function<void(const QByteArray&)> onSuccess = nullptr,
               std::function<void(const QString&)> onError = nullptr);

    // 手动设置 Cookie
    void setCookies(const QList<QNetworkCookie>& cookies);
};

#endif // HTTPCLIENT_H
