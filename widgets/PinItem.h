#ifndef CPINITEM_H
#define CPINITEM_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QPainter>

namespace view
{

class PinItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES( QGraphicsItem )
public:
    enum Type
    {
        RECT,
        CIRCULAR
    };

public:
    PinItem( const QColor & baseColor, QObject * parent = nullptr );
    ~PinItem() override = default;

public slots:
    void setSelected( bool isSelected );

public:
    QRectF boundingRect() const override;
    void paint( QPainter * painter,
                const QStyleOptionGraphicsItem * option,
                QWidget * widget) override;

    void setRect( const QRectF & rect );
    void setBrush( const QBrush & brush );
    void setPen( const QPen & pen );
    void setType( const Type & type );

    QRectF rect() const { return m_rect; }

signals:
    void clicked();

private:
    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
//    void mouseMoveEvent( QGraphicsSceneMouseEvent * event ) override;

private:
    QColor                  m_baseColor{};
    QColor                  m_selectColor = Qt::blue;
    QRectF                  m_rect{};
    QPen                    m_pen;
    QBrush                  m_brush{};
    Type                    m_type = Type::RECT;
};

}

#endif // CPINITEM_H
