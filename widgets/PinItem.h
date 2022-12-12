#ifndef CPINITEM_H
#define CPINITEM_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QPainter>

class PinItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    PinItem( const QColor & baseColor, QObject * parent = nullptr );
    ~PinItem() = default;

public:
//    void setSize( const QSizeF & newSize );

public:
    QSizeF size() const { return m_rect.size(); }

private:
//    void mouseClickEvent( QGraphicsSceneMouseEvent * event );
//    void mouseMoveEvent( QGraphicsSceneMouseEvent * event );

private:
    QRectF                  m_rect{};
    QColor                  m_selectColor = Qt::blue;
};

#endif // CPINITEM_H
