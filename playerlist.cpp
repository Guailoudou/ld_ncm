#include "playerlist.h"
#include "ui_playerlist.h"

playerlist::playerlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::playerlist)
{
    ui->setupUi(this);
}

playerlist::~playerlist()
{
    delete ui;
}
