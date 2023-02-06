#ifndef CBLOCKITEM_H
#define CBLOCKITEM_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPainter>

namespace view
{

class BodyItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES( QGraphicsItem )
public:
    BodyItem( QObject * parent = nullptr );
    ~BodyItem() override = default;

signals:
    void positionChanged();

public:
    QRectF boundingRect() const override;
    void paint( QPainter * painter,
                const QStyleOptionGraphicsItem * option,
                QWidget * widget) override;

    void setRect( const QRectF & rect );
    void setBrush( const QBrush & brush );
    void setPen( const QPen & pen );
    void setSelected( bool isSelected );

    QRectF rect() const { return m_rect; }

protected:
    QVariant itemChange( QGraphicsItem::GraphicsItemChange change,
                         const QVariant & value ) override;

private:
    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;

private:
    Qt::CursorShape         m_cursor = Qt::OpenHandCursor;
    QRectF                  m_rect{};
    QBrush                  m_brush{};
    QPen                    m_pen{};
    QLinearGradient         m_gradient{};
    int                     m_radiusRounding = 10;
};

}

#endif // CBLOCKITEM_H
