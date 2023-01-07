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
    ui.id->setPlaceholderText(toUTF8("�������ֻ�����"));
    ui.password->setPlaceholderText(toUTF8("����������"));
    ui.code->setPlaceholderText(toUTF8("��������֤��"));
}

void Enroll::on_login_clicked()
{
    emit goLogin();
}

//�û���ȡ��֤��
void Enroll::on_requestCode_clicked()
{
    QString mailAddress = ui.id->text();
    QByteArray byteArray = MessageJson::verificationDataToQByteArrey(mailAddress);
    TcpSocket::write(byteArray);
}