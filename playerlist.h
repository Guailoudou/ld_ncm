#ifndef PLAYERLIST_H
#define PLAYERLIST_H

#include <QWidget>

namespace Ui {
class playerlist;
}

class playerlist : public QWidget
{
    Q_OBJECT

public:
    explicit playerlist(QWidget *parent = nullptr);
    ~playerlist();

private:
    Ui::playerlist *ui;
};

#endif // PLAYERLIST_H
