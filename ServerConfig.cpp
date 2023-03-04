#include "ServerConfig.h"
#include "TcpSocket.h"
#include "ui_ServerConfig.h"

#include <qmessagebox.h>

QSettings *ServerConfig::configManager=nullptr;

ServerConfig::ServerConfig(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    if(configManager == nullptr)
        configManager = new QSettings("./server.ini", QSettings::IniFormat);
    ui.oldIP->setText(getServerIP());
}

/*************************************************
Description: 确定按钮点击后，尝试更改ip地址
*************************************************/
void ServerConfig::on_okButton_clicked()
{
    QString ipAddr = ui.newIP->text();

    if (setServerIP(ipAddr))
    {
        QMessageBox::information(NULL, u8"更改成功", u8"IP地址已更改为:"+ipAddr.toUtf8(), QMessageBox::Ok);
        close();
    }
    else
    {
        QMessageBox::warning(NULL, u8"更改失败", u8"无法连接到地址:" + ipAddr.toUtf8(), QMessageBox::Ok);
    }
}

/*************************************************
Description: 点击取消按钮，退出程序
*************************************************/
void ServerConfig::on_cancelButton_clicked()
{
    close();
}

/*************************************************
Description: 更改IP地址并将文件保存到server.ini
      Input: ipAddr=需要更改的ip地址
     Return: 是否更改成功
*************************************************/
bool ServerConfig::setServerIP(QString ipAddr)
{
    if (configManager == nullptr)
    {
        configManager = new QSettings("./server.ini", QSettings::IniFormat);
    }

    QRegExp ipAddrRegExp(R"(^((2(5[0-5]|[0-4]\d))|[0-1]?\d{1,2})(\.((2(5[0-5]|[0-4]\d))|[0-1]?\d{1,2})){3}$)");
    if (!ipAddrRegExp.exactMatch(ipAddr))
    {
        return false;
    }

    TcpSocket::connectToHost(ipAddr, 8888);
    if (!TcpSocket::isConnected())
    {
        return false;
    }

    configManager->setValue("IP", ipAddr);
    configManager->sync();

    return true;
}

/*************************************************
Description: 读取server.ini中的IP地址
     Return: IP地址
*************************************************/
QString ServerConfig::getServerIP()
{
    if(configManager == nullptr)
    {
        configManager = new QSettings("./server.ini", QSettings::IniFormat);
    }

    QString ipAddr = configManager->value("IP", "127.0.0.1").toString();
    
    return ipAddr;
}