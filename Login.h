#pragma once

#include <QtWidgets/QWidget>
#include "ui_Login.h"

class Login : public QWidget
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();
    void setStyle();

signals:
    void goEnroll();

public slots:
    void on_enroll_clicked();

private:
    Ui::LoginClass ui;
};
