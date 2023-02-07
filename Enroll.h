#pragma once

#include <QtWidgets/QWidget>
#include<qwidgetaction.h>
#include<qtimer.h>
#include "ui_Enroll.h"

class Enroll : public QWidget
{
    Q_OBJECT

public:
    Enroll(QWidget *parent = nullptr);
    ~Enroll();
    void setStyle();
    bool verifyInformation();

signals:
    void goLogin();

public slots:
    void on_login_clicked();
    bool on_enroll_clicked();
    void on_requestCode_clicked();
    void updateProgress();

private:
    void startTimer();

private:
    Ui::EnrollClass ui;
    static qint32 countDown;
    QTimer* qtimer;
};
