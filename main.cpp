#include "mainwindow.h"
#include <QApplication>
#include "ncm_login.h"
#include <new>
#include <iostream>
#include "userdata.h"

userData* user_data = new userData();
int main(int argc, char* argv[])
{
    try
    {
        QApplication a(argc, argv);

        //        ncm_login logins;
        //        logins.show();
        user_data->getfile();
        MainWindow w;
        w.show();
        return a.exec();
    }
    catch(const std::bad_alloc& e)
    {
        std::cerr << "【致命错误】内存分配失败: " << e.what() << std::endl;
        return -1;
    }

}
