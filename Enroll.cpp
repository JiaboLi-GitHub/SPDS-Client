#include "Enroll.h"
#include"TcpData.h"
#include"TcpSocket.h"
#include"MessageJson.h"

#include<qmessagebox.h>
#include<qhostinfo.h>

#define toUTF8(str)  QString::fromLocal8Bit(str)

Enroll::Enroll(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setStyle();
}

Enroll::~Enroll()
{}


void Enroll::setStyle()
{
    ui.mailAddress->setPlaceholderText(toUTF8("请输入邮箱地址"));
    ui.userName->setPlaceholderText(toUTF8("请输入用户名"));
    ui.password->setPlaceholderText(toUTF8("请输入密码"));
    ui.code->setPlaceholderText(toUTF8("请输入验证码"));
}

void Enroll::on_login_clicked()
{
    emit goLogin();
}

//用户获取验证码
void Enroll::on_requestCode_clicked()
{
    QRegExp mailAddressRegExp(R"(\w[-\w.+]*@([A-Za-z0-9][-A-Za-z0-9]+\.)+[A-Za-z]{2,14})");
    QString mailAddress = ui.mailAddress->text();

    if (mailAddress.isEmpty())
    {
        QMessageBox::warning(NULL, toUTF8("未输入邮箱"), toUTF8("请输入邮箱！"), QMessageBox::Ok);
        return;
    }
    else if (!mailAddressRegExp.exactMatch(mailAddress))
    {
        QMessageBox::warning(NULL, toUTF8("邮箱格式错误"), toUTF8("请输入正确的邮箱！"), QMessageBox::Ok);
        return;
    }

    if (TcpSocket::isConnected() || TcpSocket::connectToHost("127.0.0.1", 8888))
    {
        QString mailAddress = ui.mailAddress->text();
        QByteArray byteArray = MessageJson::verificationDataToQByteArray(mailAddress);
        TcpSocket::write (byteArray);
        QMessageBox::warning(NULL, toUTF8("发送成功"), toUTF8("请检查您的邮箱！"), QMessageBox::Ok);
    }
    else
    {
        QHostInfo info = QHostInfo::fromName("baidu.com");
        if (info.error() != QHostInfo::NoError)
        {
            QMessageBox::warning(NULL, toUTF8("网络错误"), toUTF8("请检查网络连接！"), QMessageBox::Ok);
        }
        else
        {
            QMessageBox::warning(NULL, u8"网络错误", u8"无法连接到服务器，请稍后再试！", QMessageBox::Ok);
        }
    }
}


/*************************************************
Description: 按下注册按钮后的处理逻辑
     Return: 是否成功注册
*************************************************/
bool Enroll::on_enroll_clicked()
{
    if (!this->verifyInformation())
    {
        return false;
    }

    QString mailAddress = ui.mailAddress->text();
    QString password = ui.password->text();
    QString userName = ui.userName->text();
    QString code = ui.code->text();

    if (TcpSocket::isConnected() || TcpSocket::connectToHost("127.0.0.1", 8888))
    {
        QByteArray byteArray = MessageJson::enrollToQByteArray(mailAddress, password,userName,code);
        TcpSocket::write(byteArray);

        if (TcpSocket::isReceived())
        {
            QByteArray receivedByteArray = TcpSocket::read();
            QMap<QString, QString> receivedData = MessageJson::getResponseData(receivedByteArray);
            switch (receivedData["Enroll_Response"].toInt())
            {
            case TcpData::Enroll_Response::Enroll_Correct:
                QMessageBox::warning(NULL, u8"登录成功", u8"登录成功！", QMessageBox::Ok);
                return true;

            case TcpData::Enroll_Response::Exist_Error:
                QMessageBox::warning(NULL, u8"用户已存在", u8"该邮箱已注册，请直接登录！", QMessageBox::Ok);
                break;
            case TcpData::Enroll_Response::Code_Error:
                QMessageBox::warning(NULL, u8"验证码错误", u8"请检查验证码是否正确！", QMessageBox::Ok);
                break;
            case TcpData::Enroll_Response::Enroll_error:
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
        if (info.error() != QHostInfo::NoError)
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
bool Enroll::verifyInformation()
{
    QRegExp mailAddressRegExp(R"(\w[-\w.+]*@([A-Za-z0-9][-A-Za-z0-9]+\.)+[A-Za-z]{2,14})");
    QRegExp passwordRegExp(R"(^[0-9a-zA-Z_.]{6,14}$)");

    QString mailAddress = ui.mailAddress->text();
    QString password = ui.password->text();
    QString userName = ui.userName->text();
    QString code = ui.code->text();


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
    else if (password.size() < 6)
    {
        QMessageBox::warning(NULL, toUTF8("密码过短"), toUTF8("密码需大于6个字符且不超过14个字符！"), QMessageBox::Ok);
        return false;
    }
    else if (password.size() > 14)
    {
        QMessageBox::warning(NULL, toUTF8("密码过长"), toUTF8("用户名需大于6个字符且不超过14个字符！"), QMessageBox::Ok);
        return false;
    }
    else if(!passwordRegExp.exactMatch(password))
    {
        QMessageBox::warning(NULL, toUTF8("密码格式错误"), toUTF8("密码应只包含数字、英文字符、下划线、小数点"), QMessageBox::Ok);
        return false;
    }

    if (userName.isEmpty())
    {
        QMessageBox::warning(NULL, toUTF8("未输入用户名"), toUTF8("请输入用户名！"), QMessageBox::Ok);
        return false;
    }
    else if (userName.size() < 2)
    {
        QMessageBox::warning(NULL, toUTF8("用户名过短"), toUTF8("用户名需大于2个字符且不超过8个字符！"), QMessageBox::Ok);
        return false;
    }
    else if(userName.size()>8)
    {
        QMessageBox::warning(NULL, toUTF8("用户名过长"), toUTF8("用户名需大于2个字符且不超过8个字符！"), QMessageBox::Ok);
        return false;
    }

    if (code.isEmpty())
    {
        QMessageBox::warning(NULL, toUTF8("未输入验证码"), toUTF8("请输入验证码！"), QMessageBox::Ok);
        return false;
    }
    else if (code.size() != 4)
    {
        QMessageBox::warning(NULL, toUTF8("验证码错误"), toUTF8("请输入正确的验证码！"), QMessageBox::Ok);
        return false;
    }

    return true;
}