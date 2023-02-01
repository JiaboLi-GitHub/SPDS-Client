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

    if (TcpSocket::isConnected()|| TcpSocket::connectToHost("127.0.0.1", 8888))
    {
        
    }
    else
    {
        QMessageBox::warning(NULL, toUTF8("�������"), toUTF8("�����������ӣ�"), QMessageBox::Ok);
    }

    return true;
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

/*************************************************
Description: ������У����Ϣ
     Return: У���Ƿ�ͨ��
*************************************************/
//bool Login::serverVerify()
//{
//    
//}