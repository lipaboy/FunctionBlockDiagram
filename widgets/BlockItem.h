#ifndef CBLOCKITEM_H
#define CBLOCKITEM_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>

class BlockItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    BlockItem( QObject * parent = nullptr );
    ~BlockItem() = default;

private:
    void mousePressEvent( QGraphicsSceneMouseEvent * event );
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );

private:
    Qt::CursorShape         m_cursor = Qt::OpenHandCursor;
};

#endif // CBLOCKITEM_H
