#include "userdata.h"

userData::userData()
{

}

QString userData::getCookie() const
{
    return cookie;
}

void userData::setCookie(const QString& value)
{
    cookie = value;
}

QJsonObject userData::getData() const
{
    return data;
}

void userData::setData(const QJsonObject& value)
{
    data = value;
}
