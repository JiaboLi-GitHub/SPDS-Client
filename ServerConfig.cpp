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
Description: ȷ����ť����󣬳��Ը���ip��ַ
*************************************************/
void ServerConfig::on_okButton_clicked()
{
    QString ipAddr = ui.newIP->text();

    if (setServerIP(ipAddr))
    {
        QMessageBox::information(NULL, u8"���ĳɹ�", u8"IP��ַ�Ѹ���Ϊ:"+ipAddr.toUtf8(), QMessageBox::Ok);
        close();
    }
    else
    {
        QMessageBox::warning(NULL, u8"����ʧ��", u8"�޷����ӵ���ַ:" + ipAddr.toUtf8(), QMessageBox::Ok);
    }
}

/*************************************************
Description: ���ȡ����ť���˳�����
*************************************************/
void ServerConfig::on_cancelButton_clicked()
{
    close();
}

/*************************************************
Description: ����IP��ַ�����ļ����浽server.ini
      Input: ipAddr=��Ҫ���ĵ�ip��ַ
     Return: �Ƿ���ĳɹ�
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
Description: ��ȡserver.ini�е�IP��ַ
     Return: IP��ַ
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