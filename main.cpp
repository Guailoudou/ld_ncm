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
        //    MainWindow w;
        //    w.show();
        ncm_login logins;
        logins.show();
        return a.exec();
    }
    catch(const std::bad_alloc& e)
    {
        std::cerr << "�����������ڴ����ʧ��: " << e.what() << std::endl;
        return -1;
    }

}
