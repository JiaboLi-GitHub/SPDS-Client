#include "Enroll.h"
#include "TcpData.h"
#include "TcpSocket.h"
#include "JsonServer.h"
#include "ServerConfig.h"

#include<qmessagebox.h>
#include<qhostinfo.h>
#include<qtimer.h>

#define toUTF8(str)  QString::fromLocal8Bit(str)

qint32 Enroll::countDown = 0;

Enroll::Enroll(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setStyle();
    qtimer = nullptr;
    
    if (countDown > 0)
    {
        ui.requestCode->setText(QString("%1S").arg(countDown));
        startTimer();
    }
}

Enroll::~Enroll()
{}


void Enroll::setStyle()
{
    ui.mailAddress->setPlaceholderText(toUTF8("�����������ַ"));
    ui.userName->setPlaceholderText(toUTF8("�������û���"));
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
    QRegExp mailAddressRegExp(R"(\w[-\w.+]*@([A-Za-z0-9][-A-Za-z0-9]+\.)+[A-Za-z]{2,14})");
    QString mailAddress = ui.mailAddress->text();

    if (mailAddress.isEmpty())
    {
        QMessageBox::warning(NULL, toUTF8("δ��������"), toUTF8("���������䣡"), QMessageBox::Ok);
        return;
    }
    else if (!mailAddressRegExp.exactMatch(mailAddress))
    {
        QMessageBox::warning(NULL, toUTF8("�����ʽ����"), toUTF8("��������ȷ�����䣡"), QMessageBox::Ok);
        return;
    }

    if (TcpSocket::isConnected() || TcpSocket::connectToHost(ServerConfig::getServerIP(), 8888))
    {
        QString mailAddress = ui.mailAddress->text();
        QByteArray byteArray = JsonServer::toQByteArray(CodeData(mailAddress));
        TcpSocket::write(byteArray);

        countDown = 60;
        startTimer();

        QMessageBox::information(NULL, toUTF8("���ͳɹ�"), toUTF8("���ͳɹ����뵽�����ȡ��֤�룡"), QMessageBox::Ok);
    }
    else
    {
        QHostInfo info = QHostInfo::fromName("baidu.com");
        if (info.error() != QHostInfo::NoError)
        {
            QMessageBox::warning(NULL, toUTF8("�������"), toUTF8("�����������ӣ�"), QMessageBox::Ok);
        }
        else
        {
            QMessageBox::warning(NULL, u8"�������", u8"�޷����ӵ������������Ժ����ԣ�", QMessageBox::Ok);
        }
    }
}


/*************************************************
Description: ����ע�ᰴť��Ĵ����߼�
     Return: �Ƿ�ɹ�ע��
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
    qint32 code = ui.code->text().toInt();

    if (TcpSocket::isConnected() || TcpSocket::connectToHost(ServerConfig::getServerIP(), 8888))
    {
        QByteArray byteArray = JsonServer::toQByteArray(EnrollData(userName,mailAddress, password,code));
        TcpSocket::write(byteArray);

        if (TcpSocket::isReceived())
        {
            QByteArray receivedByteArray = TcpSocket::read();
            EnrollData receivedData = JsonServer::toEnrollData(receivedByteArray);
            switch (receivedData.enroll_response)
            {
            case EnrollData::Enroll_Response::Enroll_Correct:
                QMessageBox::warning(NULL, u8"ע��ɹ�", u8"ע��ɹ���", QMessageBox::Ok);
                on_login_clicked();
                return true;

            case EnrollData::Enroll_Response::Exist_Error:
                QMessageBox::warning(NULL, u8"�û��Ѵ���", u8"��������ע�ᣬ��ֱ�ӵ�¼��", QMessageBox::Ok);
                break;
            case EnrollData::Enroll_Response::Code_Error:
                QMessageBox::warning(NULL, u8"��֤�����", u8"������֤���Ƿ���ȷ��", QMessageBox::Ok);
                break;
            case EnrollData::Enroll_Response::Enroll_error:
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
        if (info.error() != QHostInfo::NoError)
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
    else if (password.size() < 6)
    {
        QMessageBox::warning(NULL, toUTF8("�������"), toUTF8("���������6���ַ��Ҳ�����14���ַ���"), QMessageBox::Ok);
        return false;
    }
    else if (password.size() > 14)
    {
        QMessageBox::warning(NULL, toUTF8("�������"), toUTF8("�û��������6���ַ��Ҳ�����14���ַ���"), QMessageBox::Ok);
        return false;
    }
    else if(!passwordRegExp.exactMatch(password))
    {
        QMessageBox::warning(NULL, toUTF8("�����ʽ����"), toUTF8("����Ӧֻ�������֡�Ӣ���ַ����»��ߡ�С����"), QMessageBox::Ok);
        return false;
    }

    if (userName.isEmpty())
    {
        QMessageBox::warning(NULL, toUTF8("δ�����û���"), toUTF8("�������û�����"), QMessageBox::Ok);
        return false;
    }
    else if (userName.size() < 2)
    {
        QMessageBox::warning(NULL, toUTF8("�û�������"), toUTF8("�û��������2���ַ��Ҳ�����8���ַ���"), QMessageBox::Ok);
        return false;
    }
    else if(userName.size()>8)
    {
        QMessageBox::warning(NULL, toUTF8("�û�������"), toUTF8("�û��������2���ַ��Ҳ�����8���ַ���"), QMessageBox::Ok);
        return false;
    }

    if (code.isEmpty())
    {
        QMessageBox::warning(NULL, toUTF8("δ������֤��"), toUTF8("��������֤�룡"), QMessageBox::Ok);
        return false;
    }
    else if (code.size() != 4)
    {
        QMessageBox::warning(NULL, toUTF8("��֤�����"), toUTF8("��������ȷ����֤�룡"), QMessageBox::Ok);
        return false;
    }

    return true;
}

/*************************************************
Description: ���µ���ʱUI
*************************************************/
void Enroll::updateProgress()
{
    ui.requestCode->setText(QString("%1S").arg(countDown));
    if (--countDown <= 0)
    {
        ui.requestCode->setEnabled(true);
        qtimer->stop();
        ui.requestCode->setText(u8"��ȡ��֤��");
    }
}

/*************************************************
Description: ��ʼ����ʱ
*************************************************/
void Enroll::startTimer()
{
    if(qtimer == nullptr)
    {
        qtimer = new QTimer(this);
        connect(qtimer, SIGNAL(timeout()), this, SLOT(updateProgress()));
    }

    ui.requestCode->setEnabled(false);
    qtimer->start(1000);
}