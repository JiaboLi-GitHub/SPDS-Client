#include "Login.h"

#include <qmessagebox.h>
#include <QtNetwork>

#include "TcpSocket.h"
#include "JsonServer.h"
#include "ServerConfig.h"

#define toUTF8(str)  QString::fromLocal8Bit(str)

Login::Login(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setStyle();
}

Login::~Login()
{}

void Login::setStyle()
{
    ui.mailAddress->setPlaceholderText(toUTF8("�����������ַ"));
    ui.password->setPlaceholderText(toUTF8("����������"));
}

void Login::on_enroll_clicked()
{
    emit goEnroll();
}

/*************************************************
Description: ���µ�¼��ť��Ĵ����߼�
     Return: �Ƿ�ɹ���¼
*************************************************/
bool Login::on_login_clicked() 
{
    if (!this->verifyInformation())
    {
        return false;
    }

    QString mailAddress = ui.mailAddress->text();
    QString password = ui.password->text();

    if (TcpSocket::isConnected()|| TcpSocket::connectToHost(ServerConfig::getServerIP(), 8888))
    {
        QByteArray byteArray = JsonServer::toQByteArray(LoginData(mailAddress, password));
        TcpSocket::write(byteArray);

        if (TcpSocket::isReceived())
        {
            QByteArray receivedByteArray = TcpSocket::read();
            LoginData receivedData = JsonServer::toLogInData(receivedByteArray);
            switch (receivedData.login_response)
            {
            case LoginData::Login_Response::Login_Correct:
                QMessageBox::warning(NULL, u8"��¼�ɹ�", u8"��¼�ɹ���", QMessageBox::Ok);
                {
                    QString userName = receivedData.userName;
                    emit setUserName(userName);

                    QString token = receivedData.token;
                    emit setUserToken(token, userName);
                }
                return true;

            case LoginData::Login_Response::Account_Error:
                QMessageBox::warning(NULL, u8"�˺Ŵ���", u8"�˺Ż��������", QMessageBox::Ok);
                break;
            case LoginData::Login_Response::Login_Error:
                QMessageBox::warning(NULL, u8"δ֪����", u8"���Ժ����ԣ�", QMessageBox::Ok);
                break;
            }
        }
        else 
        {
            QMessageBox::warning(NULL, u8"�������", u8"�������޻�Ӧ�����Ժ����ԣ�", QMessageBox::Ok);
        }
    }
    else
    {
        QHostInfo info = QHostInfo::fromName("baidu.com");
        if(info.error()!=QHostInfo::NoError)
        {
            QMessageBox::warning(NULL, toUTF8("�������"), toUTF8("�����������ӣ�"), QMessageBox::Ok);
        }
        else
        {
            QMessageBox::warning(NULL, u8"�������", u8"�޷����ӵ������������Ժ����ԣ�", QMessageBox::Ok);
        }
    }
    return false;
}

/*************************************************
Description: ���������Ϣ���л���У��
     Return: У���Ƿ�ͨ��
*************************************************/
bool Login::verifyInformation()
{
    QRegExp mailAddressRegExp(R"(\w[-\w.+]*@([A-Za-z0-9][-A-Za-z0-9]+\.)+[A-Za-z]{2,14})");
    QString mailAddress = ui.mailAddress->text();
    QString password = ui.password->text();

    if (mailAddress.isEmpty())
    {
        QMessageBox::warning(NULL, toUTF8("δ��������"), toUTF8("���������䣡"), QMessageBox::Ok);
        return false;
    }
    else if (!mailAddressRegExp.exactMatch(mailAddress))
    {
        QMessageBox::warning(NULL, toUTF8("�����ʽ����"), toUTF8("��������ȷ�����䣡"), QMessageBox::Ok);
        return false;
    }
    
    if (password.isEmpty())
    {
        QMessageBox::warning(NULL, toUTF8("δ��������"), toUTF8("���������룡"), QMessageBox::Ok);
        return false;
    }

    return true;
}