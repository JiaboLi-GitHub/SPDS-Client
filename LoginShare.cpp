#include "LoginShare.h"
#include "SPDS_Client.h"
#include"UtilityClass.h"
using namespace utility;

LoginShare::LoginShare(QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);
    openLogin();
    setStyle();
}

LoginShare::~LoginShare()
{
    delete login;
    delete enroll;
}


void LoginShare::setStyle()
{
    ui.close->setFlat(true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);

    QString qss = readQssFile(":/SPDS_Client/resources/qss/LoginShare.qss");
    this->setStyleSheet(qss);
}

/*************************************************
Description: ע��ҳ�л��ɵ�¼ҳ
*************************************************/
void LoginShare::openLogin()
{
    delete enroll;
    enroll = NULL;

    login = new Login(ui.widgetMain);

    connect(login, &Login::goEnroll, this, &LoginShare::openEnroll);
    connect(login, &Login::setUserName, (SPDS_Client*) this->parent(), &SPDS_Client::setUserName);
    connect(login, &Login::setUserToken, (SPDS_Client*) this->parent(), &SPDS_Client::setUserToken);
    connect(login, &Login::closeWindow, this, &LoginShare::on_close_clicked);
    login->show();
}

/*************************************************
Description: ��¼ҳ�л���ע��ҳ
*************************************************/
void LoginShare::openEnroll()
{
    delete login;
    login = NULL;

    enroll = new Enroll(ui.widgetMain);
    connect(enroll, &Enroll::goLogin, this, &LoginShare::openLogin);
    enroll->show();
}

void LoginShare::on_close_clicked()
{
    close();
}