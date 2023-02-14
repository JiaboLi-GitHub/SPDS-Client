#pragma once

#include <QStyledItemDelegate>

class DailyDataDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	DailyDataDelegate(QObject *parent);
	~DailyDataDelegate();

private:
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
};
