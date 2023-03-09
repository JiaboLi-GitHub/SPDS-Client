#include "SPDS_Client.h"

#include <qtreewidget.h>
#include <qfile.h>
#include <qlabel.h>
#include <qimage.h>
#include <qfontdatabase.h>
#include <qdebug.h>
#include <qpainter.h>
#include <qpair.h>
#include <qvariant.h>
#include <qhostinfo.h>
#include <qmessagebox.h>

#include "JsonServer.h"
#include "Detection.h"
#include "UtilityClass.h"
#include "Login.h"
#include "Enroll.h"
#include "LoginShare.h"
#include "TcpSocket.h"
#include "ServerConfig.h"
#include "Visualization.h"
using namespace utility;

#define toUTF8(str)  QString::fromLocal8Bit(str)

SPDS_Client::SPDS_Client(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setStyle();     //设置样式
    openDatabase();

    location = Wait_For_Init;
    showDetection();

    setUserName();

    autoLogin();

    //连接网络
    //TcpSocket::connectToHost(ServerConfig::getServerIP(), 8888);
}

SPDS_Client::~SPDS_Client()
{
    
}

//设置样式
void SPDS_Client::setStyle()
{
    //设置程序图标
    this->setWindowIcon(QIcon(":/SPDS_Client/resources/logo/logo.png"));

    //修改边框
    this->setWindowFlags(Qt::FramelessWindowHint);      //隐藏标题栏
    this->setAttribute(Qt::WA_TranslucentBackground);   //隐藏mainwidget

    //加载qss
    QString mainWidgetQss = readQssFile(":/SPDS_Client/resources/qss/MainWidget.qss");
    this->setStyleSheet(mainWidgetQss);

    //设置字体
    int font_Id = QFontDatabase::addApplicationFont(":/SPDS_Client/resources/font/Alibaba_PuHuiTi_2.0_55_Regular_55_Regular.ttf");
    QStringList font_list = QFontDatabase::applicationFontFamilies(font_Id);
    if (!font_list.isEmpty())
    {
        QFont f;
        f.setFamily(font_list[0]);
        this->setFont(f);
    }

    //加载logo_txt
    QImage logo_txt_image;
    logo_txt_image.load(":/SPDS_Client/resources/logo/logo_txt.png");
    ui.logo_txt->setScaledContents(true);
    ui.logo_txt->setPixmap(QPixmap::fromImage(logo_txt_image));

    setLeftMenuTreeWidgetStyle();
    setUpMenuWidgetStyle();
}

//设置左边菜单栏样式
void SPDS_Client::setLeftMenuTreeWidgetStyle()
{
    auto detection_treeWidgetItem = new QTreeWidgetItem(QStringList(toUTF8(" 坐姿监测")));
    QIcon detection_icon;
    detection_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/detectionOn.png"), QIcon::Selected);
    detection_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/detectionOff.png"), QIcon::Normal);
    detection_treeWidgetItem->setIcon(0, detection_icon);
    detection_treeWidgetItem->setData(0, Qt::UserRole, Loc_Detection);

    auto visualization_treeWidgetItem = new QTreeWidgetItem(QStringList(toUTF8(" 坐姿数据")));
    QIcon visualization_icon;
    visualization_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/visualizationOn.png"), QIcon::Selected);
    visualization_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/visualizationOff.png"), QIcon::Normal);
    visualization_treeWidgetItem->setIcon(0, visualization_icon);
    visualization_treeWidgetItem->setData(0, Qt::UserRole, Loc_Visualization);

    //auto history_treeWidgetItem = new QTreeWidgetItem(QStringList(toUTF8(" 历史坐姿")));
    //QIcon history_icon;
    //history_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/historyOn.png"), QIcon::Selected);
    //history_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/historyOff.png"), QIcon::Normal);
    //history_treeWidgetItem->setIcon(0, history_icon);

    auto son_treeWidgetItem = new QTreeWidgetItem(QStringList(toUTF8(" 亲子坐姿")));
    QIcon son_icon;
    son_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/sonOn.png"), QIcon::Selected);
    son_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/sonOff.png"), QIcon::Normal);
    son_treeWidgetItem->setIcon(0, son_icon);
    son_treeWidgetItem->setData(0, Qt::UserRole, Loc_Familial);

    ui.leftMenuTreeWidget->addTopLevelItem(detection_treeWidgetItem);
    ui.leftMenuTreeWidget->addTopLevelItem(visualization_treeWidgetItem);
    //ui.leftMenuTreeWidget->addTopLevelItem(history_treeWidgetItem);
    ui.leftMenuTreeWidget->addTopLevelItem(son_treeWidgetItem);
    ui.leftMenuTreeWidget->setFrameStyle(QFrame::NoFrame);       //隐藏边框
    ui.leftMenuTreeWidget->setHeaderHidden(true);           //隐藏标题栏
    ui.leftMenuTreeWidget->setIconSize({ 30,30 });          //设置图标大小
}

//右上角菜单样式
void SPDS_Client::setUpMenuWidgetStyle()
{
    ui.close->setIcon(QIcon(":/SPDS_Client/resources/icon/close.png"));
    ui.maximize->setIcon(QIcon(":/SPDS_Client/resources/icon/maximize.png"));
    ui.minimize->setIcon(QIcon(":/SPDS_Client/resources/icon/minimize.png"));
    ui.serverConfigButton->setIcon(QIcon(":/SPDS_Client/resources/icon/install.png"));
    ui.login->setIcon(QIcon(":/SPDS_Client/resources/icon/login.png"));
    ui.close->setFlat(true);
    ui.maximize->setFlat(true);
    ui.minimize->setFlat(true);
    ui.serverConfigButton->setFlat(true);
    ui.login->setFlat(true);
}

/*-------------------------使失去标题栏的窗体可移动----------------------------*/
//鼠标按下事件
void SPDS_Client::mousePressEvent(QMouseEvent* event)
{
    this->setFocus();
    if (Qt::LeftButton == event->button() && 0 == (Qt::WindowMaximized & this->windowState()))
    {
        QPoint temp = event->globalPos();
        pLast = temp;    //记录鼠标按下的位置
        event->ignore();
    }
    m_bPressed = true; //标记鼠标为按下状态
}
//鼠标移动事件
void SPDS_Client::mouseMoveEvent(QMouseEvent* event)
{
    if (this->isMaximized()) //如果当前是最大化，则不允许移动
        return;

    if ((event->buttons() & Qt::LeftButton) && m_bPressed)//是否左击
    {
        QPoint ptemp = event->globalPos();         //当前鼠标全局位置     
        move(ptemp - pLast + pos());
        pLast = ptemp ;                         
    }
}
//鼠标松开事件
void SPDS_Client::mouseReleaseEvent(QMouseEvent* event)
{
    QApplication::restoreOverrideCursor();//恢复鼠标指针性状
    event->ignore();
    m_bPressed = false;
}

/*------------------------右上角菜单----------------------------*/
void SPDS_Client::on_close_clicked()
{
    this->close();
}

void SPDS_Client::on_minimize_clicked()
{
    this->showMinimized();
}

void SPDS_Client::on_maximize_clicked()
{
    if (this->isMaximized())
    {
        this->showNormal();
    }
    else
    {
        this->showMaximized();
    }
}

/*---------------------------登录---------------------------------*/
void SPDS_Client::on_login_clicked()
{
    //创建背景遮罩
    QWidget widget(this);
    widget.resize(this->width(), this->height());
    widget.move(0, 0);
    widget.setStyleSheet("background-color:rgba(0, 0, 0,50%);");
    widget.show();

    LoginShare loginShare(this);
    loginShare.exec();
}

void SPDS_Client::on_serverConfigButton_clicked()
{
    ServerConfig serverconfig;
    serverconfig.exec();
}

/*---------------------------读写用户信息---------------------------------*/
void SPDS_Client::setUserName(QString userName)
{
    ui.userName->setText(userName);
    this->userName = userName;
}

void SPDS_Client::setUserToken(QString userName, QString token)
{
    QSqlDatabase db = openDatabase();
    createTokensTable(db);

    QSqlQuery query(db);
    query.exec("DROP * FROM tokens");
    query.prepare("INSERT INTO tokens (username, token) VALUES (:username, :token)");
    query.bindValue(":username", userName);
    query.bindValue(":token", token);
    query.exec();

    this->token = token;

    db.close();
}

QVariant SPDS_Client::getUserToken()
{
    QSqlDatabase db = openDatabase();
    createTokensTable(db);

    QSqlQuery query(db);
    bool exist = query.exec("SELECT * FROM tokens");

    if (!exist || !query.next())
    {
        db.close();
        return QVariant();
    }

    QString userName = query.value(0).toString();
    QString token = query.value(1).toString();

    db.close();

    QVariant data;
    data.setValue(qMakePair(token, userName));

    return data;
}

void SPDS_Client::autoLogin()
{
    /*
    QVariant userInfo = getUserToken();
    if (userInfo.isNull())
    {
        return;
    }

    QPair<QString, QString> userInfoPair = userInfo.value<QPair<QString, QString>>();
    QString token = userInfoPair.first;
    QString userName = userInfoPair.second;

    if (TcpSocket::isConnected() || TcpSocket::connectToHost(ServerConfig::getServerIP(), 8888))
    {
        QByteArray byteArray = JsonServer::toLogInData(token);
        TcpSocket::write(byteArray);

        if (TcpSocket::isReceived())
        {
            QByteArray receivedByteArray = TcpSocket::read();
            QMap<QString, QString> receivedData = MessageJson::getResponseData(receivedByteArray);
            switch (receivedData["Auto_LogIn_Response"].toInt())
            {
            case TcpData::Auto_Login_Response::Auto_Login_Correct:
                {
                    emit setUserName(userName);
                    this->token = token;
                }
                break;
            default: 
                break;

            }
        }
        else
        {
            QMessageBox::warning(NULL, u8"网络错误", u8"服务器无回应！", QMessageBox::Ok);
        }
    }
    else
    {
        QHostInfo info = QHostInfo::fromName("baidu.com");
        if (info.error() != QHostInfo::NoError)
        {
            QMessageBox::warning(NULL, toUTF8("网络错误"), toUTF8("请检查网络连接！"), QMessageBox::Ok);
        }
        else
        {
            QMessageBox::warning(NULL, u8"网络错误", u8"无法连接到服务器！", QMessageBox::Ok);
        }
    }
    */
}


/*---------------------------页面切换---------------------------------*/
void SPDS_Client::on_leftMenuTreeWidget_itemClicked(QTreeWidgetItem* indexItem, qint32 itemID)
{
    if (location == Location::Loc_Detection && ((Detection*)ui.mainWidget)->isFreeze())
        return;

    qint32 selecter = indexItem->data(0, Qt::UserRole).toInt();
    switch (selecter)
    {
    case Loc_Detection:showDetection(); break;
    case Loc_Visualization:showVisualization(); break;
    case Loc_Familial:showFamilial(); break;
    default:assert(0);
    }
    return;
}

void SPDS_Client::showDetection()
{
    if (location == Loc_Detection)
    {
        return;
    }
    location = Loc_Detection;

    Detection* detection = new Detection(this);
    detection->setGeometry(ui.mainWidget->geometry());
    delete ui.mainWidget;
    ui.mainWidget = detection;
    detection->show();
}

void SPDS_Client::showVisualization()
{
    if (location == Loc_Visualization)
    {
        return;
    }
    if (token.isEmpty())
    {
        QMessageBox::warning(NULL, u8"用户未登录", u8"请先登录再使用该功能！", QMessageBox::Ok);
        return;
    }
    location = Loc_Visualization;

    if (TcpSocket::isConnected() || TcpSocket::connectToHost(ServerConfig::getServerIP(), 8888))
    {
        QByteArray byteArray = JsonServer::toQByteArray(GetSPDData(token));
        TcpSocket::write(byteArray);

        if (TcpSocket::isReceived())
        {
            QByteArray receivedByteArray = TcpSocket::read();
            auto receivedData = JsonServer::toSPDDataList(receivedByteArray);

            Visualization* visualization = new Visualization(this, receivedData);
            visualization->setGeometry(ui.mainWidget->geometry());
            delete ui.mainWidget;
            ui.mainWidget = visualization;
            visualization->show();
        }
        else
        {
            QMessageBox::warning(NULL, u8"网络错误", u8"服务器无回应！", QMessageBox::Ok);
        }
    }
    else
    {
        QHostInfo info = QHostInfo::fromName("baidu.com");
        if (info.error() != QHostInfo::NoError)
        {
            QMessageBox::warning(NULL, toUTF8("网络错误"), toUTF8("请检查网络连接！"), QMessageBox::Ok);
        }
        else
        {
            QMessageBox::warning(NULL, u8"网络错误", u8"无法连接到服务器！", QMessageBox::Ok);
        }
    }
}

void SPDS_Client::showFamilial()
{
    if (location == Loc_Familial)
    {
        return;
    }
    location = Loc_Familial;
}

/*---------------------------数据库---------------------------------*/
QSqlDatabase SPDS_Client::openDatabase()
{
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("sqlite.db");
        database.setUserName("master");
        database.setPassword("SPDS_Client_SQL");
    }

    if (!database.isOpen())
    {
        database.open();
    }

    return database;
}

void SPDS_Client::createTokensTable(QSqlDatabase db)
{
    QSqlQuery query(db);
    bool exist = query.exec("SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'tokens'");
    
    if (!exist || !query.next()) 
    {
        query.exec("CREATE TABLE tokens (username TEXT, token TEXT)");
    }

    return;
}
