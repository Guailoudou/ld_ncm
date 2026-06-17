#ifndef USER_PLAYERLIST_H
#define USER_PLAYERLIST_H

#include <QWidget>
#include <QJsonObject>
#include "network.h"

namespace Ui
{
class user_playerlist;
}

class user_playerlist : public QWidget
{
    Q_OBJECT

public:
    explicit user_playerlist(QWidget* parent = nullptr);
    ~user_playerlist();

    void setItem(QJsonObject& jsonData);

    QString getId() const;
    void setId(const QString& value);


    QJsonObject getJsonData() const;
    void setJsonData(const QJsonObject& value);

private:
    Ui::user_playerlist* ui;
    network* net;
    QString id;
    QJsonObject jsonData;
};

#endif // USER_PLAYERLIST_H
