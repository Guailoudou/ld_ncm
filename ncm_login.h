#ifndef NCM_LOGIN_H
#define NCM_LOGIN_H

#include <QDialog>
#include <QPixmap>
#include <QTimer>
#include "userData.h"
namespace Ui
{
class ncm_login;
}
class network;
extern userData* user_data;
class ncm_login : public QDialog
{
    Q_OBJECT

public:
    explicit ncm_login(QWidget* parent = nullptr);
    ~ncm_login();

private:
    Ui::ncm_login* ui;
    network* net;
    QTimer timer;
};

#endif // NCM_LOGIN_H
