#ifndef CFUNCTIONBLOCKDIAGRAMWIDGET_H
#define CFUNCTIONBLOCKDIAGRAMWIDGET_H

#include "models/FunctionGraph.h"

#include <QWidget>
#include <QGraphicsView>
#include <QHash>

#include <optional>

class FunctionBlockItem;
class ConnectionItem;

struct SConnection
{
    SFunctionPinIndex inFunc{};
    SFunctionPinIndex outFunc{};

    SConnection( SFunctionPinIndex const & inputFunc,
                 SFunctionPinIndex const & outputFunc )
        : inFunc( inputFunc ),
          outFunc( outputFunc )
    {}

    bool operator==( SConnection const & other ) const
    {
        return inFunc == other.inFunc && outFunc == other.outFunc;
    }
    bool operator!=( SConnection const & other ) const
    {
        return ! ( *this == other );
    }
};

inline size_t qHash( const SConnection & key, size_t seed )
{
    return qHash( key.inFunc.func, seed ) ^ size_t( key.inFunc.pin )
            ^ qHash( key.outFunc.func, seed ) ^ size_t( key.outFunc.pin );
}

class FunctionBlockDiagramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionBlockDiagramWidget(QWidget *parent = nullptr);
    ~FunctionBlockDiagramWidget() override;

signals:

public slots:
    void graphUpdated();

    void blockPinClicked( bool isIn, int blockIndex, int pinIndex );

protected:
    void keyPressEvent( QKeyEvent * event ) override;

private:
    void setPinSelected( bool isIn, const SFunctionPinIndex & pairIndex, bool isSelected );

private slots:
    void setConnection( const SFunctionPinIndex & inBlock,
                        const SFunctionPinIndex & outBlock,
                        bool hasConnection );

private:
    // View
    QGraphicsView *                                 m_graphicsView{};
    QGraphicsScene *                                m_scene{};
    QPointF                                         m_blockAppearPoint{ 100, 100 };
    QPointF                                         m_stepAppearPoint{ 0, 40 };
    /** i of m_blockMap == i of CFunctionGraph->getVertices() */
    QVector< FunctionBlockItem * >                  m_blockMap{};

    // Model
    FunctionGraph *                                 m_functionGraph{};

    // Connections
    std::optional< SFunctionPinIndex >              m_inPinSelected{ std::nullopt };
    std::optional< SFunctionPinIndex >              m_outPinSelected{ std::nullopt };

    QHash< SConnection, ConnectionItem * >          m_linesMap{};
    std::optional< SConnection >                    m_connectionSelected{ std::nullopt };
};

#endif // CFUNCTIONBLOCKDIAGRAMWIDGET_H
