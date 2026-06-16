#include "userdata.h"

userData::userData()
{

}

QString userData::getCookie() const
{
    return cookie;
}

void userData::setCookie(const QString& value)
{
    cookie = value;
}

QJsonObject userData::getData() const
{
    return data;
}

void userData::setData(const QJsonObject& value)
{
    data = value;
}

void userData::savefile()
{
    QJsonObject rootObj;
    rootObj["cookie"] = getCookie();
    QJsonDocument doc(rootObj);
    QFile file("./ncm_data.json");
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        qDebug() << "JSON saveok！";
    }
    else
    {
        qWarning() << "文件打开失败：" << file.errorString();
    }

}

void userData::getfile()
{
    QFile file("./ncm_data.json");
    // 1. 尝试以只读模式打开文件
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "getjson err";
        return;
    }

    // 2. 读取所有字节数据并关闭文件
    QByteArray jsonData = file.readAll();
    file.close();

    // 3. 解析 JSON 数据，捕获可能的语法错误
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

    if(parseError.error != QJsonParseError::NoError)
    {
        qWarning() << "JSON geterr:" << parseError.errorString();
        return;
    }

    // 4. 获取根对象
    QJsonObject root = doc.object();
    setCookie(root.value("cookie").toString());
}
