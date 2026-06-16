#include "music_list_item.h"
#include "ui_music_list_item.h"

music_list_item::music_list_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::music_list_item)
{
    ui->setupUi(this);
}

music_list_item::~music_list_item()
{
    delete ui;
}
