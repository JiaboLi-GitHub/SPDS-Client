#include "LoginShare.h"
#include"UtilityClass.h"
using namespace utility;

LoginShare::LoginShare(QWidget *parent)
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
    this->setWindowFlags(Qt::FramelessWindowHint);

    QString qss = readQssFile(":/SPDS_Client/resources/qss/LoginShare.qss");
    this->setStyleSheet(qss);
}

void LoginShare::openLogin()
{
    delete enroll;
    enroll = NULL;

    login = new Login(ui.widgetR);
    connect(login, &Login::goEnroll, this, &LoginShare::openEnroll);
    login->show();
}

void LoginShare::openEnroll()
{
    delete login;
    login = NULL;

    enroll = new Enroll(ui.widgetR);
    connect(enroll, &Enroll::goLogin, this, &LoginShare::openLogin);
    enroll->show();
}

void LoginShare::on_close_clicked()
{
    close();
}