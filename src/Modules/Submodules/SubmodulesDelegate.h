#ifndef SUBMODULESDELEGATE_H
#define SUBMODULESDELEGATE_H

#include <QItemDelegate>

/**
 * @brief The SubmodulesViewDelegate class
 */
class SubmodulesViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    SubmodulesViewDelegate( QObject* parent );

public:
    void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;

    /**
     * @brief Internal function to draw the item.
     * @param painter
     * @param option
     * @param rect
     * @param index
     */
    inline void drawDisplay(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QModelIndex &index) const;

public slots:
    bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index);

};

#endif // SUBMODULESDELEGATE_H
