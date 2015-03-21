#include "RefsViewDelegate.h"

#include "Branches/RefItem.hpp"

#include <QPainter>


RefsViewDelegate::RefsViewDelegate(QObject* parent)
    : QStyledItemDelegate( parent )
{

}

RefsViewDelegate::~RefsViewDelegate()
{

}

void RefsViewDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    const QRect rowRect = option.rect.adjusted( -(option.rect.left()), 0, 0, 0 );
    const QVariant rowBg        = index.data( RefItem::RowBgRole );
    const QVariant rowGradient  = index.data( RefItem::RowBgGradientRole );

    if ( rowGradient.isValid() )
    {
        QColor back = rowGradient.value<QColor>();
        QColor back2 = back.lighter(135);

        const qreal wLimit = qreal( qMin(30, rowRect.width()) ) / qreal( qMax(30, rowRect.width()) );
        QLinearGradient gradient( 0, 0, rowRect.width(), 0 );
        gradient.setColorAt( 0.0, back2 );
        gradient.setColorAt( wLimit, back );
        gradient.setColorAt( 1.0 - wLimit, back );
        gradient.setColorAt( 1.0, back2 );

        painter->fillRect( rowRect, gradient );
    }

    else if ( rowBg.isValid() )
    {
        painter->fillRect( rowRect, rowBg.value<QBrush>() );
    }

    QStyledItemDelegate::paint( painter, option, index );
}


QSize RefsViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex& index) const
{
    QSize s = QStyledItemDelegate::sizeHint(option, index);

    if (!index.parent().isValid()) {
        s.setHeight(25);
    }

    return s;
}
