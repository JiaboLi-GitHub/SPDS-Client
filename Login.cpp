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
    ui.id->setPlaceholderText(toUTF8("�������ֻ�����"));
    ui.password->setPlaceholderText(toUTF8("����������"));
}

void Login::on_enroll_clicked()
{
    emit goEnroll();
}
