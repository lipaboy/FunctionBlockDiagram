#ifndef CPINITEM_H
#define CPINITEM_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QGraphicsItem>
#include <QPainter>

class PinItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    PinItem( const QColor & baseColor, QObject * parent = nullptr );
    ~PinItem() = default;

public:
    void setSize( const QSizeF & newSize );
    void setPos( const QPointF & newPos );

public:
    QSizeF size() const { return m_rect.size(); }

private:
    QRectF boundingRect() const;
    void paint( QPainter * painter,
                const QStyleOptionGraphicsItem * option,
                QWidget * widget );
    void mouseClickEvent( QGraphicsSceneMouseEvent * event );

private:
    QRectF                  m_rect{};
    QColor                  m_selectColor = Qt::blue;
};

#endif // CPINITEM_H
