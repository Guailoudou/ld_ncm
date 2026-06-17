#include "httpclient.h"
#include <QNetworkCookieJar>

HttpClient::HttpClient(QObject* parent) : QNetworkAccessManager(parent) {}

void HttpClient::hget(const QString& url,
                      std::function<void(const QByteArray&)> onSuccess,
                      std::function<void(const QString&)> onError)
{
    QNetworkRequest request{QUrl(url)};

    m_retryGetUrl = url;
    m_retryGetOnSuccess = onSuccess;
    m_retryGetOnError = onError;
    m_retryGetCount = 0;

    // 发起 GET 请求
    QNetworkReply* reply = QNetworkAccessManager::get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            if(m_retryGetOnSuccess)
            {
                m_retryGetOnSuccess(reply->readAll());
            }
        }
        else
        {
            m_retryGetCount++;
            if(m_retryGetCount < MAX_RETRY)
            {
                QTimer::singleShot(RETRY_DELAY_MS, this, &HttpClient::retryGet);
                return;
            }
            else
            {
                if(m_retryGetOnError)
                {
                    m_retryGetOnError(reply->errorString());
                }
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

    m_retryPostUrl = url;
    m_retryPostData = jsonData;
    m_retryPostOnSuccess = onSuccess;
    m_retryPostOnError = onError;
    m_retryPostCount = 0;

    // 发起 POST 请求
    QNetworkReply* reply = QNetworkAccessManager::post(request, data);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            if(m_retryPostOnSuccess)
            {
                m_retryPostOnSuccess(reply->readAll());
            }
        }
        else
        {
            m_retryPostCount++;
            if(m_retryPostCount < MAX_RETRY)
            {
                QTimer::singleShot(RETRY_DELAY_MS, this, &HttpClient::retryPost);
                return;
            }
            else
            {
                if(m_retryPostOnError)
                {
                    m_retryPostOnError(reply->errorString());
                }
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

void HttpClient::retryGet()
{
    QNetworkRequest request{QUrl(m_retryGetUrl)};
    QNetworkReply* reply = QNetworkAccessManager::get(request);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            if(m_retryGetOnSuccess)
            {
                m_retryGetOnSuccess(reply->readAll());
            }
        }
        else
        {
            m_retryGetCount++;
            if(m_retryGetCount < MAX_RETRY)
            {
                QTimer::singleShot(RETRY_DELAY_MS, this, &HttpClient::retryGet);
                return;
            }
            else
            {
                if(m_retryGetOnError)
                {
                    m_retryGetOnError(reply->errorString());
                }
            }
        }
        reply->deleteLater();
    });
}

void HttpClient::retryPost()
{
    QNetworkRequest request{QUrl(m_retryPostUrl)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QByteArray data = QJsonDocument(m_retryPostData).toJson(QJsonDocument::Compact);

    QNetworkReply* reply = QNetworkAccessManager::post(request, data);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            if(m_retryPostOnSuccess)
            {
                m_retryPostOnSuccess(reply->readAll());
            }
        }
        else
        {
            m_retryPostCount++;
            if(m_retryPostCount < MAX_RETRY)
            {
                QTimer::singleShot(RETRY_DELAY_MS, this, &HttpClient::retryPost);
                return;
            }
            else
            {
                if(m_retryPostOnError)
                {
                    m_retryPostOnError(reply->errorString());
                }
            }
        }
        reply->deleteLater();
    });
}
