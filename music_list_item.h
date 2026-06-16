#ifndef MUSIC_LIST_ITEM_H
#define MUSIC_LIST_ITEM_H

#include <QWidget>

namespace Ui {
class music_list_item;
}

class music_list_item : public QWidget
{
    Q_OBJECT

public:
    explicit music_list_item(QWidget *parent = nullptr);
    ~music_list_item();

private:
    Ui::music_list_item *ui;
};

#endif // MUSIC_LIST_ITEM_H
