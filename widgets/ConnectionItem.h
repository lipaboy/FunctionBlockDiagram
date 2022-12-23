#ifndef CONNECTIONITEM_H
#define CONNECTIONITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPen>

class ConnectionItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    ConnectionItem( QObject * parent = nullptr );
    ~ConnectionItem() override = default;

signals:
    void selected( bool isSelected );

public:
    void setLine( const QLineF & line );

protected:
    QVariant itemChange( QGraphicsItem::GraphicsItemChange change,
                         const QVariant & value ) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    void recalcSelectionPolygon();

private:
    QColor                      m_baseColor = QColor::fromRgb( 0, 119, 255 );
    QColor                      m_selectedColor = Qt::yellow;
    int                         m_width = 3;

    const qreal                 m_selectionOffset = 10;
    QPolygonF                   m_selectionPolygon;
};

#endif // CONNECTIONITEM_H
