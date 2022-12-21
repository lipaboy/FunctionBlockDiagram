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
    ~BlockItem() override = default;

signals:
    void positionChanged();

protected:
    QVariant itemChange( QGraphicsItem::GraphicsItemChange change,
                         const QVariant & value ) override;

private:
    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;

private:
    Qt::CursorShape         m_cursor = Qt::OpenHandCursor;
};

#endif // CBLOCKITEM_H
