#pragma once

#include <QObject>
#include <qdatetime.h>

class SPDDataPerDay  : public QObject
{
	Q_OBJECT

public:
	SPDDataPerDay(QObject *parent);
	~SPDDataPerDay();

private:
	QDate date;
	qint32 totalCount;
	qint32 pos0, pos1, pos2, pos3, pos4, pos5, pos6;
};
