#pragma once

#include <QtWidgets/QWidget>
#include "ui_Login.h"

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget* parent = nullptr);
    ~Login();
    void setStyle();
    bool verifyInformation();

signals:
    void goEnroll();
    void setUserName(QString userName);
    void setUserToken(QString userName, QString token);
    void closeWindow();

public slots:
    bool on_login_clicked();
    void on_enroll_clicked();

private:
    Ui::LoginClass ui;
};
