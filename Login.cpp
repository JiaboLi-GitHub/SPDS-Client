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
    ui.mailAddress->setPlaceholderText(toUTF8("请输入邮箱地址"));
    ui.password->setPlaceholderText(toUTF8("请输入密码"));
}

void Login::on_enroll_clicked()
{
    emit goEnroll();
}

/*************************************************
Description: 按下登录按钮后的处理逻辑
     Return: 是否成功登录
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
                QMessageBox::warning(NULL, u8"登录成功", u8"登录成功！", QMessageBox::Ok);
                {
                    QString userName = receivedData.userName;
                    emit setUserName(userName);

                    QString token = receivedData.token;
                    emit setUserToken(token, userName);
                }
                return true;

            case LoginData::Login_Response::Account_Error:
                QMessageBox::warning(NULL, u8"账号错误", u8"账号或密码错误！", QMessageBox::Ok);
                break;
            case LoginData::Login_Response::Login_Error:
                QMessageBox::warning(NULL, u8"未知错误", u8"请稍后重试！", QMessageBox::Ok);
                break;
            }
        }
        else 
        {
            QMessageBox::warning(NULL, u8"网络错误", u8"服务器无回应，请稍后再试！", QMessageBox::Ok);
        }
    }
    else
    {
        QHostInfo info = QHostInfo::fromName("baidu.com");
        if(info.error()!=QHostInfo::NoError)
        {
            QMessageBox::warning(NULL, toUTF8("网络错误"), toUTF8("请检查网络连接！"), QMessageBox::Ok);
        }
        else
        {
            QMessageBox::warning(NULL, u8"网络错误", u8"无法连接到服务器，请稍后再试！", QMessageBox::Ok);
        }
    }
    return false;
}

/*************************************************
Description: 对输入框信息进行基本校验
     Return: 校验是否通过
*************************************************/
bool Login::verifyInformation()
{
    QRegExp mailAddressRegExp(R"(\w[-\w.+]*@([A-Za-z0-9][-A-Za-z0-9]+\.)+[A-Za-z]{2,14})");
    QString mailAddress = ui.mailAddress->text();
    QString password = ui.password->text();

    if (mailAddress.isEmpty())
    {
        QMessageBox::warning(NULL, toUTF8("未输入邮箱"), toUTF8("请输入邮箱！"), QMessageBox::Ok);
        return false;
    }
    else if (!mailAddressRegExp.exactMatch(mailAddress))
    {
        QMessageBox::warning(NULL, toUTF8("邮箱格式错误"), toUTF8("请输入正确的邮箱！"), QMessageBox::Ok);
        return false;
    }
    
    if (password.isEmpty())
    {
        QMessageBox::warning(NULL, toUTF8("未输入密码"), toUTF8("请输入密码！"), QMessageBox::Ok);
        return false;
    }

    return true;
}