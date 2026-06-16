#ifndef MUSIC_LIST_ITEM_H
#define MUSIC_LIST_ITEM_H

#include <QWidget>
#include "network.h"

namespace Ui
{
class music_list_item;
}

class music_list_item : public QWidget
{
    Q_OBJECT

public:
    explicit music_list_item(QWidget* parent = nullptr);
    void setItem(QJsonObject jsonData);
    ~music_list_item();

    QString getMid() const;
    void setMid(const QString& value);

    QByteArray getImgdata() const;
    void setImgdata(const QByteArray& value);

    QJsonObject getJsonData() const;
    void setJsonData(const QJsonObject& value);

private:
    Ui::music_list_item* ui;
    network* net;
    QString mid;
    QByteArray Imgdata;
    QJsonObject jsonData;
};

#endif // MUSIC_LIST_ITEM_H
