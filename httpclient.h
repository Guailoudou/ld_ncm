#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookie>
#include <QList>
#include <QTimer>
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

private slots:
    void retryGet();
    void retryPost();

private:
    // GET 重试参数
    QString m_retryGetUrl;
    std::function<void(const QByteArray&)> m_retryGetOnSuccess;
    std::function<void(const QString&)> m_retryGetOnError;
    int m_retryGetCount = 0;

    // POST 重试参数
    QString m_retryPostUrl;
    QJsonObject m_retryPostData;
    std::function<void(const QByteArray&)> m_retryPostOnSuccess;
    std::function<void(const QString&)> m_retryPostOnError;
    int m_retryPostCount = 0;

    static constexpr int MAX_RETRY = 3;
    static constexpr int RETRY_DELAY_MS = 500;
};

#endif // HTTPCLIENT_H
