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

        if(player->duration() == 0)
        {
            timer.stop();
            for(int i = 0; i < ui->listWidget->count(); ++i)
            {
                QListWidgetItem* item = ui->listWidget->item(i);
                if(item)
                {
                    music_list_item* from = dynamic_cast<music_list_item*>(ui->listWidget->itemWidget(item));
                    if(from->getMid() == getNewmid())
                    {
                        if(i < ui->listWidget->count() - 1)
                        {
                            on_listWidget_itemClicked(ui->listWidget->item(++i));
                        }
                        break;
                    }
                }
            }
        }
        ui->mpbar->setValue(player->position());
        ui->mpbar->setMaximum(player->duration());
        //qDebug() << "maxtime=" << player->duration() << "time= " << player->position();
        ui->lyric_label->setText(getLyricAtMs(player->position()));
        QString time_str = QDateTime::fromMSecsSinceEpoch(player->position(), Qt::UTC).toString("mm:ss");
        time_str += "/" + QDateTime::fromMSecsSinceEpoch(player->duration(), Qt::UTC).toString("mm:ss");
        ui->time_label->setText(time_str);
    });
    onLoginWindowClosed();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//搜索
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
//点击列表元素
void MainWindow::on_listWidget_itemClicked(QListWidgetItem* item)
{
    if(item)
    {
        music_list_item* from = dynamic_cast<music_list_item*>(ui->listWidget->itemWidget(item));
        if(from)
        {
            QString mid = from->getMid();
            setNewmid(mid);
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
                        openplayer(murl);
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
                        openplayer(murl);
                    }
                });
            }
            getLyric(mid);
            QPixmap pixmap;
            pixmap.loadFromData(from->getImgdata());
            ui->img_new->setPixmap(pixmap.scaled(ui->img_new->size(), Qt::KeepAspectRatio));
        }
        else
        {
            user_playerlist* from2 = dynamic_cast<user_playerlist*>(ui->listWidget->itemWidget(item));
            if(from2)
            {
                ui->listWidget->clear();
                QString url = net->getApi_url() + "/playlist/detail?id=" + from2->getId();
                net->geturl_Json(url,
                                 [this](const QJsonObject & Jsondata)
                {
                    int code = Jsondata.value("code").toInt();
                    if(code == 200)
                    {
                        //playlist.tracks[]
                        QJsonArray songs = Jsondata.value("playlist").toObject().value("tracks").toArray();
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
        }


    }
}


void MainWindow::openplayer(const QString murl)
{
    qDebug() << "[player]:url=" << murl;
    //播放音乐
    playerlist->clear();
    playerlist->addMedia(QUrl(murl));                     //添加一音乐到播放列表中
    player->setMedia(playerlist);                        //将列表设置到播放器中
    playerlist->setCurrentIndex(0);
    player->play();                                  //播放
    timer.start(1000);
}
void MainWindow::getLyric(const QString mid)
{
    QString url = net->getApi_url() + "/lyric?id=" + mid + "&ua=pc&timestamp=" + common::get_time();
    net->geturl_Json(url, [this](const QJsonObject & Jsondata)
    {
        int code = Jsondata.value("code").toInt();
        if(code == 200)
        {
            lrcMap.clear();
            QString lrcString = Jsondata.value("lrc").toObject().value("lyric").toString();
            QRegularExpression regex(R"(\[\s*(\d+)\s*:\s*(\d+)\s*\.\s*(\d+)\s*\]\s*(.*))");
            QRegularExpressionMatchIterator it = regex.globalMatch(lrcString);

            while(it.hasNext())
            {
                QRegularExpressionMatch match = it.next();
                int minutes = match.captured(1).toInt();
                int seconds = match.captured(2).toInt();
                int milliseconds = match.captured(3).toInt();
                QString text = match.captured(4).trimmed();

                // 3. 将时间统一转换为毫秒
                qint64 totalMs = minutes * 60000 + seconds * 1000 + milliseconds;
                lrcMap.insert(totalMs, text);
            }
        }
    });
}
QString MainWindow::getLyricAtMs(qint64 currentMs)
{
    if(lrcMap.isEmpty())
    {
        return QString("当前音乐无歌词，请欣赏"); // 返回空字符串，防止崩溃
    }
    auto upperIt = lrcMap.lowerBound(currentMs);

    // 6. 如果找到的迭代器指向Map开头，说明当前时间早于第一句歌词
    if(upperIt == lrcMap.begin())
    {
        return upperIt.value();
    }

    // 7. 否则，回退一步，即为当前时间所属的歌词区间
    --upperIt;
    return upperIt.value();
}
void MainWindow::on_mpbar_sliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::on_stopButton_clicked()
{
    if(ui->stopButton->text() == "暂停")
    {
        player->pause();
        timer.stop();
        ui->stopButton->setText("播放");
    }
    else
    {
        player->play();
        timer.start(1000);
        ui->stopButton->setText("暂停");
    }

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
    else
    {
        onLoginWindowClosed();
        if(ui->loginButton->text() != "登录")
        {
            return;
        }
        //退出登录
        QString url = net->getApi_url() + "/logout?timestamp=" + common::get_time();
        QJsonObject postDataObj;
        postDataObj.insert("cookie", user_data->getCookie());
        net->posturl_Json(url, postDataObj,
                          [this](const QJsonObject & Jsondata)
        {
            int code = Jsondata.value("code").toInt();
            if(code == 200)
            {
                qDebug() << "退出登录成功";
                user_data->setCookie("");
                user_data->savefile();
                ui->loginButton->setText("登录");
            }


        });
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
                if(!userdata.value("profile").isObject())
                {
                    user_data->setCookie("");
                    user_data->savefile();
                    return;
                }
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
        });
    }
}

void MainWindow::on_recButton_clicked()
{
    if(user_data->getCookie() == "")
    {
        QMessageBox::information(this, "提示", "登录后才可以获取每日推荐。");
        return;
    }
    ui->listWidget->clear();
    QString url = net->getApi_url() + "/recommend/songs";
    QJsonObject postDataObj;
    postDataObj.insert("cookie", user_data->getCookie());
    qDebug() << "开始获取每日推荐";
    net->posturl_Json(url, postDataObj,
                      [this](const QJsonObject & Jsondata)
    {
        int code = Jsondata.value("code").toInt();
        if(code == 200)
        {
            QJsonArray songs = Jsondata.value("data").toObject().value("dailySongs").toArray();
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

QString MainWindow::getNewmid() const
{
    return newmid;
}

void MainWindow::setNewmid(QString value)
{
    newmid = value;
}

void MainWindow::on_playlistButton_clicked()
{
    QString url = net->getApi_url() + "/user/playlist?uid=" + QString::number(static_cast<qint64>(user_data->getData().value("account").toObject().value("id").toDouble()));
    qDebug() << "url:" << url ;

    net->geturl_Json(url,
                     [this](const QJsonObject & Jsondata)
    {
        int code = Jsondata.value("code").toInt();
        if(code == 200)
        {
            ui->listWidget->clear();
            QJsonArray playerlists = Jsondata.value("playlist").toArray();
            for(const QJsonValue& pl : playerlists)
            {
                if(pl.isObject())
                {
                    QJsonObject plo = pl.toObject();
                    QListWidgetItem* item = new QListWidgetItem;
                    ui->listWidget->addItem(item);
                    user_playerlist* from = new user_playerlist(this);
                    from->setItem(plo);
                    item->setSizeHint(from->size());
                    ui->listWidget->setItemWidget(item, from);
                }
            }
        }


    });
}
