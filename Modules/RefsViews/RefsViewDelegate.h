#ifndef REFSVIEWDELEGATE_H
#define REFSVIEWDELEGATE_H

#include <QStyledItemDelegate>


class RefsViewDelegate : public QStyledItemDelegate
{
public:
    RefsViewDelegate( QObject* parent = 0 );
    ~RefsViewDelegate();

public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex& index) const;
};

#endif
