#include "Login.h"

#include <qmessagebox.h>

#include "TcpSocket.h"
#include "MessageJson.h"

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

    if (TcpSocket::isConnected()|| TcpSocket::connectToHost("127.0.0.1", 8888))
    {
        
    }
    else
    {
        QMessageBox::warning(NULL, toUTF8("网络错误"), toUTF8("请检查网络连接！"), QMessageBox::Ok);
    }

    return true;
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

/*************************************************
Description: 服务器校验信息
     Return: 校验是否通过
*************************************************/
//bool Login::serverVerify()
//{
//    
//}