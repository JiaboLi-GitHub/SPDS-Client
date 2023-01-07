#include "UtilityClass.h"
#include<qfile.h>
namespace utility
{
	QString readQssFile(QString fileName)
	{
		QFile qssFile(fileName);
		qssFile.open(QFile::ReadOnly);
		QString qss = qssFile.readAll();
		qssFile.close();
		return qss;
	}
}