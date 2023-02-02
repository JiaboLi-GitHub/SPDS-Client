#include "UtilityClass.h"
#include<qfile.h>
namespace utility
{
	/*************************************************
	Description: ��qss�ļ���ȡ��QString����
		  Input: fileName=qss�ļ�·��
		 Return: QString��ʽ��qss�ļ�
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