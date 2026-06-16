#include "ncm_login.h"
#include "ui_ncm_login.h"
#include "network.h"
#include <QByteArray>
#include <QImageReader>

ncm_login::ncm_login(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ncm_login),
    net(new network(this))
{
    ui->setupUi(this);

    net->login_getQR([this](const QString & qrBase64)
    {
        if(!qrBase64.isEmpty())
        {
            QByteArray ba = QByteArray::fromBase64((qrBase64.section(",", 1).toLocal8Bit()));
            //            QImageReader reader(ba);
            QPixmap pix;
            pix.loadFromData(ba);
            ui->login_QR->setPixmap(pix.scaled(ui->login_QR->size(), Qt::KeepAspectRatio));
        }
    });

    connect(&timer, &QTimer::timeout, [this]
    {
        net->login_check([this](const QJsonObject & data)
        {
            int code = data.value("code").toInt();
            qDebug() << "[登录状态]:" << data.value("message").toString();
            ui->login_state->setText(data.value("message").toString());
            if(code == 802)
            {
                net->geturl_data(data.value("avatarUrl").toString(),
                                 [this](const QByteArray & data)
                {
                    QPixmap pixmap;
                    pixmap.loadFromData(data);
                    ui->login_QR->setPixmap(pixmap.scaled(ui->login_QR->size(), Qt::KeepAspectRatio));
                });
            }
            else if(code == 803)
            {
                user_data->setCookie(data.value("cookie").toString());
                this->timer.stop();
                this->close();
            }
        });
    });
    timer.start(5000);
}
void ncm_login::closeEvent(QCloseEvent* event)
{
    emit windowClosed(); // 窗口关闭时发出信号
    QWidget::closeEvent(event); // 调用基类的默认关闭行为
}
ncm_login::~ncm_login()
{
    delete ui;
}
