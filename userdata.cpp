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
