#pragma once

#include <QtWidgets/QMainWindow>
#include<Qt3DInput/qmouseevent.h>

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

private slots:
    void on_leftMenuTreeWidget_itemClicked(QTreeWidgetItem* indexItem, int itemID);

private:
    void showDetection();
    void showVisualization();
    void showFamilial();

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
    //´°Ìå¿ÉÒÆ¶¯
    QPoint pLast;
    bool m_bPressed; 
};
