#include "Enroll.h"
#include"TcpData.h"
#include"TcpSocket.h"
#include"MessageJson.h"

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
    QString mailAddress = ui.mailAddress->text();
    QByteArray byteArray = MessageJson::verificationDataToQByteArray(mailAddress);
    TcpSocket::write(byteArray);
}
