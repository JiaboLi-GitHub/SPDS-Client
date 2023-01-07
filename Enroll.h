#pragma once

#include <QtWidgets/QWidget>
#include<qwidgetaction.h>
#include "ui_Enroll.h"

class Enroll : public QWidget
{
    Q_OBJECT

public:
    Enroll(QWidget *parent = nullptr);
    ~Enroll();
    void setStyle();

signals:
    void goLogin();


public slots:
    void on_login_clicked();
    void on_requestCode_clicked();

private:
    Ui::EnrollClass ui;

};
