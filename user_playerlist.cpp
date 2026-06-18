#include "user_playerlist.h"
#include "ui_user_playerlist.h"

user_playerlist::user_playerlist(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::user_playerlist),
    net(new network(this))
{
    ui->setupUi(this);
}

void user_playerlist::setItem(QJsonObject& jsonData)
{
    setJsonData(jsonData);
    net->geturl_data(jsonData.value("coverImgUrl").toString(),
                     [this](const QByteArray & data)
    {
        QPixmap pixmap;
        pixmap.loadFromData(data);
        ui->song_img->setPixmap(pixmap.scaled(ui->song_img->size(), Qt::KeepAspectRatio));
    });

    ui->song_name->setText(jsonData.value("name").toString());
    setId(QString::number(static_cast<qint64>(jsonData.value("id").toDouble())));
}
void user_playerlist::rmSelected()
{
    ui->splitter->setStyleSheet("background-color: rgb(255, 255, 255,0);");
}
void user_playerlist::setSelected()
{
    ui->splitter->setStyleSheet("background-color: rgb(255, 0, 0,20);");
}
user_playerlist::~user_playerlist()
{
    delete ui;
}

QString user_playerlist::getId() const
{
    return id;
}

void user_playerlist::setId(const QString& value)
{
    id = value;
}

QJsonObject user_playerlist::getJsonData() const
{
    return jsonData;
}

void user_playerlist::setJsonData(const QJsonObject& value)
{
    jsonData = value;
}
