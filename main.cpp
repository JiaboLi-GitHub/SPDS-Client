#include "ModuleLoader.h"
#include "SPDS_Client.h"

#include <QtWidgets/QApplication>
#include <vector>
#include <qvector.h>
#include <iostream>
#include <memory>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    SPDS_Client w;
    w.show();
    return a.exec(); 

    
}
// 李工
