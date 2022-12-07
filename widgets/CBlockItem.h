#ifndef CBLOCKITEM_H
#define CBLOCKITEM_H

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QGraphicsItem>
#include <QPainter>

class CBlockItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    CBlockItem( int ins, int outs, QObject * parent = nullptr );
    ~CBlockItem() = default;

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QRect                   m_mainRect{};
    int                     m_pinEdge{ 12 };
    int                     m_pinSpace{ 12 };
    QSize                   m_mainRectBase{ 90, m_pinSpace * 4 };

    QColor                  m_inColor = Qt::green;
    QColor                  m_outColor = Qt::red;

    int                     m_ins{};
    int                     m_outs{};
};

#endif // CBLOCKITEM_H
