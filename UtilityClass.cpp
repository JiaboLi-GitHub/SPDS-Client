#include "UtilityClass.h"
#include<qfile.h>
namespace utility
{
	/*************************************************
	Description: 将qss文件读取成QString变量
		  Input: fileName=qss文件路径
		 Return: QString形式的qss文件
	*************************************************/
	QString readQssFile(QString fileName)
	{
		QFile qssFile(fileName);
		qssFile.open(QFile::ReadOnly);
		QString qss = qssFile.readAll();
		qssFile.close();
		return qss;
	}
}