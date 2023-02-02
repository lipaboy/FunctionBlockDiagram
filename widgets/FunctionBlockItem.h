#ifndef FUNCTIONBLOCKITEM_H
#define FUNCTIONBLOCKITEM_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QGraphicsSimpleTextItem>
#include <QVector>
#include <QString>

#include <optional>

#include "models/FunctionGraph.h"

class BlockItem;
class PinItem;

class FunctionBlockItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    FunctionBlockItem( QString const & labelText,
                       int ins,
                       int outs,
                       QObject * parent = nullptr );
    ~FunctionBlockItem() override = default;

public:
    void setPinSelected( SFunctionPinIndex::EPinType type,
                         int index,
                         bool isSelected );
    void setSize( const QSizeF & newSize );
    void setTopLeftPos( const QPointF & newPos );

public:
    QSizeF size() const;
    QPointF getEdgePinPoint( SFunctionPinIndex::EPinType type,
                             int pinIndex ) const;

signals:
    void pinClicked( SFunctionPinIndex::EPinType type, int pinIndex );
    void positionChanged();

protected:
    QVariant itemChange( QGraphicsItem::GraphicsItemChange change,
                         const QVariant & value ) override;
    QRectF boundingRect() const override;

private:
    void mousePressEvent( QGraphicsSceneMouseEvent * event ) override;
    void mouseReleaseEvent( QGraphicsSceneMouseEvent * event ) override;
    void hoverEnterEvent( QGraphicsSceneHoverEvent * event ) override;

    void recalcItemsPos();

private:
    BlockItem *                 m_block{};
    QVector< PinItem * >        m_inPins{};
    QVector< PinItem * >        m_outPins{};
    QGraphicsSimpleTextItem *   m_label{};

    std::optional< int >        m_inPinSelected{ std::nullopt };
    std::optional< int >        m_outPinSelected{ std::nullopt };

    qreal                       m_pinEdge{ 12 };
    qreal                       m_pinSpace{ 12 };
    QSizeF                      m_mainRectBase{ 50, m_pinSpace * 4 };
};

#endif // FUNCTIONBLOCKITEM_H
