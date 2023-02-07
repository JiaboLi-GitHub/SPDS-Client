#pragma once

#include <QtWidgets/QDialog>
#include <qsettings.h>
#include "ui_ServerConfig.h"

class ServerConfig : public QDialog
{
    Q_OBJECT

public:
    ServerConfig(QWidget* parent = nullptr);
    static QString getServerIP();

public slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::ServerConfigClass ui;
    static QSettings *configManager;
    bool setServerIP(QString ipAddr);
};
