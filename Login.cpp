#include "Login.h"
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
    ui.id->setPlaceholderText(toUTF8("请输入手机号码"));
    ui.password->setPlaceholderText(toUTF8("请输入密码"));
}

void Login::on_enroll_clicked()
{
    emit goEnroll();
}
