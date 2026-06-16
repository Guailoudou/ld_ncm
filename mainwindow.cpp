#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    net(new network(this)),
    player(new QMediaPlayer(this)),
    playerlist(new QMediaPlaylist(this))
{
    ui->setupUi(this);
    connect(&timer, &QTimer::timeout, [this]
    {
        ui->mpbar->setValue(player->position());
        ui->mpbar->setMaximum(player->duration());
        if(player->duration() == 0)timer.stop();
        qDebug() << "maxtime=" << player->duration() << "time= " << player->position();
    });
    onLoginWindowClosed();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_searchButton_clicked()
{
    QString search_text = ui->searchEdit->text();
    ui->listWidget->clear();
    QString url = net->getApi_url() + "/cloudsearch?keywords=" + search_text;
    net->geturl_Json(url,
                     [this](const QJsonObject & Jsondata)
    {
        int code = Jsondata.value("code").toInt();
        if(code == 200)
        {
            QJsonArray songs = Jsondata.value("result").toObject().value("songs").toArray();
            for(const QJsonValue& songv : songs)
            {
                if(songv.isObject())
                {
                    QJsonObject song = songv.toObject();
                    QListWidgetItem* item = new QListWidgetItem;
                    ui->listWidget->addItem(item);
                    music_list_item* from = new music_list_item(this);
                    from->setItem(song);
                    item->setSizeHint(from->size());
                    ui->listWidget->setItemWidget(item, from);
                }
            }
        }


    });
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem* item)
{
    if(item)
    {
        music_list_item* from = dynamic_cast<music_list_item*>(ui->listWidget->itemWidget(item));
        QString mid = from->getMid();
        QString url = net->getApi_url() + "/song/url/v1?id=" + mid + "&ua=pc&level=exhigh&timestamp=" + common::get_time();
        qDebug() << url;
        QJsonObject postDataObj;
        if(user_data->getCookie() != "")
        {
            postDataObj.insert("cookie", user_data->getCookie());
            net->posturl_Json(url, postDataObj, [this](const QJsonObject & Jsondata)
            {
                int code = Jsondata.value("code").toInt();
                if(code == 200)
                {
                    QString murl = Jsondata.value("data").toArray()[0].toObject().value("url").toString();
                    qDebug() << "[播放]:" << murl;
                    //播放音乐
                    playerlist->clear();
                    playerlist->addMedia(QUrl(murl));                     //添加一音乐到播放列表中
                    player->setMedia(playerlist);                        //将列表设置到播放器中
                    playerlist->setCurrentIndex(0);
                    player->play();                                  //播放
                    timer.start(1000);
                }
            });
        }
        else
        {
            net->geturl_Json(url, [this](const QJsonObject & Jsondata)
            {
                int code = Jsondata.value("code").toInt();
                if(code == 200)
                {
                    QString murl = Jsondata.value("data").toArray()[0].toObject().value("url").toString();
                    qDebug() << "[player]:url=" << murl;
                    //播放音乐
                    playerlist->clear();
                    playerlist->addMedia(QUrl(murl));                     //添加一音乐到播放列表中
                    player->setMedia(playerlist);                        //将列表设置到播放器中
                    playerlist->setCurrentIndex(0);
                    player->play();                                  //播放
                    timer.start(1000);
                }
            });
        }

    }
}

void MainWindow::on_mpbar_sliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::on_stopButton_clicked()
{
    player->stop();
    timer.stop();
}


void MainWindow::on_loginButton_clicked()
{
    if(user_data->getCookie() == "")
    {
        ncm_login* logins = new ncm_login(this);
        logins->setAttribute(Qt::WA_DeleteOnClose);
        connect(logins, &ncm_login::windowClosed, this, &MainWindow::onLoginWindowClosed);
        logins->show();
    }

}
void MainWindow::onLoginWindowClosed()
{
    if(user_data->getCookie() != "")
    {
        qDebug() << "登录成功...";
        QString url = net->getApi_url() + "/login/status?timestamp=" + common::get_time() + "&ua=pc";
        QJsonObject postDataObj;
        postDataObj.insert("cookie", user_data->getCookie());
        net->posturl_Json(url, postDataObj, [this](const QJsonObject & Jsondata)
        {
            QJsonObject userdata = Jsondata.value("data").toObject();
            int code = userdata.value("code").toInt();
            if(code == 200)
            {
                user_data->setData(userdata);
                net->geturl_data(userdata.value("profile").toObject().value("avatarUrl").toString(),
                                 [this](const QByteArray & data)
                {
                    QPixmap pixmap;
                    pixmap.loadFromData(data);
                    ui->user_avatar->setPixmap(pixmap.scaled(ui->user_avatar->size(), Qt::KeepAspectRatio));
                });
                user_data->savefile();
                ui->loginButton->setText("退出登录");
            }
            else
            {
                user_data->setCookie("");
                user_data->savefile();
            }
        });
    }
}
