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
    void clicked();

public:
    void setLine( const QLineF & line );

protected:
//    QVariant itemChange( QGraphicsItem::GraphicsItemChange change,
//                         const QVariant & value ) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;

private:
    void recalcSelectionPolygon();

private:
    const qreal                 m_selectionOffset = 10;
    QPolygonF                   m_selectionPolygon;
};

#endif // CONNECTIONITEM_H
