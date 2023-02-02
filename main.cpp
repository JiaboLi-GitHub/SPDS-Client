#include "SPDS_Client.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SPDS_Client w;
    w.show();
    return a.exec(); 
}
// 李工
