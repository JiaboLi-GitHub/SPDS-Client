#include "SPDS_Client.h"
#include <QtWidgets/QApplication>
#include"Detection.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SPDS_Client w;
    w.show();
    return a.exec(); 
}
//6�̽ܲ���������VS��V��˼��V��SV��6