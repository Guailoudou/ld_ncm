#include "music_list_item.h"
#include "ui_music_list_item.h"

music_list_item::music_list_item(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::music_list_item),
    net(new network(this))
{
    ui->setupUi(this);
}

void music_list_item::setItem(QJsonObject& jsonData)
{
    setJsonData(jsonData);
    net->geturl_data(jsonData.value("al").toObject().value("picUrl").toString(),
                     [this](const QByteArray & data)
    {
        QPixmap pixmap;
        pixmap.loadFromData(data);
        ui->song_img->setPixmap(pixmap.scaled(ui->song_img->size(), Qt::KeepAspectRatio));
        setImgdata(data);
    });
    QJsonArray ars = jsonData.value("ar").toArray();
    QString ars_str = "";
    for(const QJsonValue& arv : ars)
    {
        if(arv.isObject())
        {
            QJsonObject ar = arv.toObject();
            ars_str += ar.value("name").toString();
            ars_str += " ";
        }

    }
    QJsonArray alias = jsonData.value("alia").toArray();
    for(const QJsonValue& arv : alias)
    {
        if(arv.isString())
        {
            QString ar = arv.toString();
            ars_str += ar;
            ars_str += " ";
        }

    }
    ui->song_artist->setText(ars_str);
    QString name = "";
    if(jsonData.value("fee").toInt() == 1)
    {
        name += "[vip] ";
    }
    name += jsonData.value("name").toString();
    ui->song_name->setText(name);
    setMid(QString::number(static_cast<qint64>(jsonData.value("id").toDouble())));
}
void music_list_item::rmSelected()
{
    ui->splitter_2->setStyleSheet("background-color: rgb(255, 255, 255,0);");
}
void music_list_item::setSelected()
{
    ui->splitter_2->setStyleSheet("background-color: rgb(255, 0, 0,20);");
}
music_list_item::~music_list_item()
{
    delete ui;
}

QString music_list_item::getMid() const
{
    return mid;
}

void music_list_item::setMid(const QString& value)
{
    mid = value;
}

QByteArray music_list_item::getImgdata() const
{
    return Imgdata;
}

void music_list_item::setImgdata(const QByteArray& value)
{
    Imgdata = value;
}

QJsonObject music_list_item::getJsonData() const
{
    return jsonData;
}

void music_list_item::setJsonData(const QJsonObject& value)
{
    jsonData = value;
}
