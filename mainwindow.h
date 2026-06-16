#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "network.h"
#include "userdata.h"
#include "common.h"
#include "music_list_item.h"
#include "ncm_login.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>
#include <QTimer>
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


private slots:
    void on_searchButton_clicked();

    void on_listWidget_itemClicked(QListWidgetItem* item);

    void on_mpbar_sliderMoved(int position);

    void on_stopButton_clicked();

    void on_loginButton_clicked();
    void onLoginWindowClosed();

private:
    Ui::MainWindow* ui;
    network* net;
    QMediaPlayer* player;               //≤•∑≈∂‘œÛ
    QMediaPlaylist* playerlist;
    QTimer timer;
};

#endif // MAINWINDOW_H
