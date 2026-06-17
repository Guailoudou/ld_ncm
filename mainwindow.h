#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "network.h"
#include "userdata.h"
#include "common.h"
#include "music_list_item.h"
#include "user_playerlist.h"
#include "ncm_login.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QTimer>
#include <QMap>
namespace Ui
{
class MainWindow;
}
extern userData* user_data;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();


    void getLyric(const QString mid);

    QString getLyricAtMs(qint64 currentMs);
    void openplayer(const QString murl);
    QString getNewmid() const;
    void setNewmid(QString value);

private slots:
    void on_searchButton_clicked();

    void on_listWidget_itemClicked(QListWidgetItem* item);

    void on_mpbar_sliderMoved(int position);

    void on_stopButton_clicked();

    void on_loginButton_clicked();
    void onLoginWindowClosed();

    void on_recButton_clicked();

    void on_playlistButton_clicked();

private:
    Ui::MainWindow* ui;
    network* net;
    QMediaPlayer* player;               //播放对象
    QMediaPlaylist* playerlist;
    QMap<qint64, QString> lrcMap;
    QString newmid;
    QTimer timer;
};

#endif // MAINWINDOW_H
