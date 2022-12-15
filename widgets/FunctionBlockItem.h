#ifndef FUNCTIONBLOCKITEM_H
#define FUNCTIONBLOCKITEM_H

#include "BlockItem.h"
#include "PinItem.h"

#include <QObject>
#include <QGraphicsItemGroup>
#include <QVector>

#include <optional>

class FunctionBlockItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    FunctionBlockItem( int ins, int outs, QObject * parent = nullptr );
    ~FunctionBlockItem() = default;

public:
    void setPinSelected(bool isIn, int index, bool isSelected );

public:
    QSizeF size() const { return m_block->rect().size(); }
    QPointF getEdgePinPoint( bool isIn, bool pinIndex ) const;

signals:
    void pinClicked( bool isIn, int pinIndex );

private slots:

private:
    virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );

private:
    BlockItem *                 m_block{};
    QVector< PinItem * >        m_inPins{};
    QVector< PinItem * >        m_outPins{};

    std::optional< int >        m_inPinSelected{ std::nullopt };
    std::optional< int >        m_outPinSelected{ std::nullopt };

    qreal                       m_pinEdge{ 12 };
    qreal                       m_pinSpace{ 12 };
    QSizeF                      m_mainRectBase{ 90, m_pinSpace * 4 };
};

#endif // FUNCTIONBLOCKITEM_H
