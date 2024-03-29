#pragma once

#include <QtWidgets/QMainWindow>
#include <Qt3DInput/qmouseevent.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include "HttpServer.h"
#include "ui_SPDS_Client.h"

class SPDS_Client : public QMainWindow
{
    Q_OBJECT

public:
    SPDS_Client(QWidget *parent = nullptr);
    ~SPDS_Client();
    void setStyle();
    void setLeftMenuTreeWidgetStyle();
    void setUpMenuWidgetStyle();
    bool isLogined();

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

public slots:
    void on_close_clicked();
    void on_login_clicked();
    void on_minimize_clicked();
    void on_maximize_clicked();
    void on_serverConfigButton_clicked();
    void setUserName(QString userName = u8"δ��¼");
    void setUserToken(QString userName, QString token);

private slots:
    void on_leftMenuTreeWidget_itemClicked(QTreeWidgetItem* indexItem, int itemID);

private:
    void showDetection();
    void showVisualization();
    void showFamilial();
    QSqlDatabase openDatabase();
    QVariant getUserToken();
    void createTokensTable(QSqlDatabase db);
    void autoLogin();
    void logout();
    void clearUserToken();

private:
    enum Location
    {
        Wait_For_Init,
        Loc_Detection,
        Loc_Visualization,
        Loc_Familial
    };

    Location location;
    Ui::SPDS_ClientClass ui;
    //������ƶ�
    QPoint pLast;
    bool m_bPressed;
    QString userName;
    QString token;
    HttpServer httpserver;
};
