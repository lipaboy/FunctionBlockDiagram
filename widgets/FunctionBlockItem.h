#ifndef FUNCTIONBLOCKITEM_H
#define FUNCTIONBLOCKITEM_H

#include "BlockItem.h"
#include "PinItem.h"

#include <QObject>
#include <QGraphicsItemGroup>
#include <QVector>

class FunctionBlockItem : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    FunctionBlockItem( int ins, int outs, QObject * parent = nullptr );
    ~FunctionBlockItem() = default;

public:
    QSizeF size() const { return m_block->rect().size(); }

private:
    BlockItem *                 m_block{};
    QVector< PinItem * >        m_inPins{};
    QVector< PinItem * >        m_outPins{};

    qreal                   m_pinEdge{ 12 };
    qreal                   m_pinSpace{ 12 };
    QSizeF                  m_mainRectBase{ 90, m_pinSpace * 4 };
};

#endif // FUNCTIONBLOCKITEM_H
