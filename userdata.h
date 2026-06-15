#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>
class userData
{
public:
    userData();
    QString getCookie() const;
    void setCookie(const QString& value);

private:
    QString cookie;
};

#endif // USERDATA_H
