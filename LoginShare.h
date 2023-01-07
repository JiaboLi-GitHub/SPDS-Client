#pragma once

#include <QtWidgets/QDialog>
#include "ui_LoginShare.h"
#include"Login.h"
#include"Enroll.h"

class LoginShare : public QDialog
{
    Q_OBJECT

public:
    LoginShare(QWidget *parent = nullptr);
    ~LoginShare();
    void setStyle();

public slots:
    void openLogin();
    void openEnroll();
    void on_close_clicked();

private:
    Ui::LoginShareClass ui;
    Login* login=NULL;
    Enroll* enroll=NULL;
};
