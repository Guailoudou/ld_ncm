#include "mainwindow.h"
#include <QApplication>
#include "ncm_login.h"
#include <new>
#include <iostream>
#include "userdata.h"
#include <exception>
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
    catch(...)
    {
        // 捕获所有其他未知异常
        std::cerr << "【未知错误】: 捕获到未预期的异常！" << std::endl;

        // (可选) 尝试获取当前异常的底层信息
        std::exception_ptr p = std::current_exception();
        if(p)
        {
            try
            {
                std::rethrow_exception(p);
            }
            catch(const std::exception& e)
            {
                std::cerr << "【深层解析】: " << e.what() << std::endl;
            }
            catch(...)
            {
                // 确实是非标准异常，无法获取更多信息
                std::cerr << "未知错误" << std::endl;
            }
        }

        return -1;
    }

}
