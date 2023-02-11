#include "SPDS_Client.h"

#include<qtreewidget.h>
#include<qfile.h>
#include<qlabel.h>
#include<qimage.h>
#include<qfontdatabase.h>
#include<qdebug.h>
#include<qpainter.h>

#include"Detection.h"
#include"UtilityClass.h"
#include"Login.h"
#include"Enroll.h"
#include"LoginShare.h"
#include"TcpSocket.h"
using namespace utility;

#define toUTF8(str)  QString::fromLocal8Bit(str)

SPDS_Client::SPDS_Client(QWidget *parent)q
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setStyle();     //������ʽ

    //��ʾ������
    ui.mainWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    Detection *detection=new Detection(this);
    int x = ui.mainWidget->x(), y = ui.mainWidget->y();
    int w = ui.mainWidget->width(), h = ui.mainWidget->height();
    detection->setGeometry(x, y, w, h);
    detection->activateWindow();
    detection->show();

    //��������
    TcpSocket::connectToHost("192.168.2.106", 8888);
}

SPDS_Client::~SPDS_Client()
{
    
}

//������ʽ
void SPDS_Client::setStyle()
{
    //���ó���ͼ��
    this->setWindowIcon(QIcon(":/SPDS_Client/resources/logo/logo.png"));

    //�޸ı߿�
    this->setWindowFlags(Qt::FramelessWindowHint);      //���ر�����
    this->setAttribute(Qt::WA_TranslucentBackground);   //����mainwidget

    //����qss
    QString mainWidgetQss = readQssFile(":/SPDS_Client/resources/qss/MainWidget.qss");
    this->setStyleSheet(mainWidgetQss);

    //��������
    int font_Id = QFontDatabase::addApplicationFont(":/SPDS_Client/resources/font/Alibaba_PuHuiTi_2.0_55_Regular_55_Regular.ttf");
    QStringList font_list = QFontDatabase::applicationFontFamilies(font_Id);
    if (!font_list.isEmpty())
    {
        QFont f;
        f.setFamily(font_list[0]);
        this->setFont(f);
    }

    //����logo_txt
    QImage logo_txt_image;
    logo_txt_image.load(":/SPDS_Client/resources/logo/logo_txt.png");
    ui.logo_txt->setScaledContents(true);
    ui.logo_txt->setPixmap(QPixmap::fromImage(logo_txt_image));

    setLeftMenuTreeWidgetStyle();
    setUpMenuWidgetStyle();
   
}

//������߲˵�����ʽ
void SPDS_Client::setLeftMenuTreeWidgetStyle()
{
    auto detection_treeWidgetItem = new QTreeWidgetItem(QStringList(toUTF8(" ���˼��")));
    QIcon detection_icon;
    detection_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/detectionOn.png"), QIcon::Selected);
    detection_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/detectionOff.png"), QIcon::Normal);
    detection_treeWidgetItem->setIcon(0, detection_icon);


    auto visualization_treeWidgetItem = new QTreeWidgetItem(QStringList(toUTF8(" ��������")));
    QIcon visualization_icon;
    visualization_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/visualizationOn.png"), QIcon::Selected);
    visualization_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/visualizationOff.png"), QIcon::Normal);
    visualization_treeWidgetItem->setIcon(0, visualization_icon);

    auto history_treeWidgetItem = new QTreeWidgetItem(QStringList(toUTF8(" ��ʷ����")));
    QIcon history_icon;
    history_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/historyOn.png"), QIcon::Selected);
    history_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/historyOff.png"), QIcon::Normal);
    history_treeWidgetItem->setIcon(0, history_icon);

    auto son_treeWidgetItem = new QTreeWidgetItem(QStringList(toUTF8(" ��������")));
    QIcon son_icon;
    son_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/sonOn.png"), QIcon::Selected);
    son_icon.addPixmap(QPixmap(":/SPDS_Client/resources/icon/sonOff.png"), QIcon::Normal);
    son_treeWidgetItem->setIcon(0, son_icon);

    ui.leftMenuTreeWidget->addTopLevelItem(detection_treeWidgetItem);
    ui.leftMenuTreeWidget->addTopLevelItem(visualization_treeWidgetItem);
    ui.leftMenuTreeWidget->addTopLevelItem(history_treeWidgetItem);
    ui.leftMenuTreeWidget->addTopLevelItem(son_treeWidgetItem);
    ui.leftMenuTreeWidget->setFrameStyle(QFrame::NoFrame);       //���ر߿�
    ui.leftMenuTreeWidget->setHeaderHidden(true);           //���ر�����
    ui.leftMenuTreeWidget->setIconSize({ 30,30 });          //����ͼ���С
}

//���Ͻǲ˵���ʽ
void SPDS_Client::setUpMenuWidgetStyle()
{
    ui.close->setIcon(QIcon(":/SPDS_Client/resources/icon/close.png"));
    ui.maximize->setIcon(QIcon(":/SPDS_Client/resources/icon/maximize.png"));
    ui.minimize->setIcon(QIcon(":/SPDS_Client/resources/icon/minimize.png"));
    ui.install->setIcon(QIcon(":/SPDS_Client/resources/icon/install.png"));
    ui.login->setIcon(QIcon(":/SPDS_Client/resources/icon/login.png"));
    ui.close->setFlat(true);
    ui.maximize->setFlat(true);
    ui.minimize->setFlat(true);
    ui.install->setFlat(true);
    ui.login->setFlat(true);
}

/*-------------------------ʹʧȥ�������Ĵ�����ƶ�----------------------------*/
//��갴���¼�
void SPDS_Client::mousePressEvent(QMouseEvent* event)
{
    this->setFocus();
    if (Qt::LeftButton == event->button() && 0 == (Qt::WindowMaximized & this->windowState()))
    {
        QPoint temp = event->globalPos();
        pLast = temp;    //��¼��갴�µ�λ��
        event->ignore();
    }
    m_bPressed = true; //������Ϊ����״̬
}
//����ƶ��¼�
void SPDS_Client::mouseMoveEvent(QMouseEvent* event)
{
    if (this->isMaximized()) //�����ǰ����󻯣��������ƶ�
        return;

    if ((event->buttons() & Qt::LeftButton) && m_bPressed)//�Ƿ����
    {
        QPoint ptemp = event->globalPos();         //��ǰ���ȫ��λ��     
        move(ptemp - pLast + pos());
        pLast = ptemp ;                         
    }
}
//����ɿ��¼�
void SPDS_Client::mouseReleaseEvent(QMouseEvent* event)
{
    QApplication::restoreOverrideCursor();//�ָ����ָ����״
    event->ignore();
    m_bPressed = false;
}

/*------------------------���Ͻǲ˵�----------------------------*/
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

/*---------------------------��¼---------------------------------*/
void SPDS_Client::on_login_clicked()
{
    //������������
    QWidget widget(this);
    widget.resize(this->width(), this->height());
    widget.move(0, 0);
    widget.setStyleSheet("background-color:rgba(0, 0, 0,50%);");
    widget.show();

    LoginShare loginShare;
    loginShare.exec();
}