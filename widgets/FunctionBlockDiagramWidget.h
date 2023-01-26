#ifndef CFUNCTIONBLOCKDIAGRAMWIDGET_H
#define CFUNCTIONBLOCKDIAGRAMWIDGET_H

#include "models/FunctionGraph.h"

#include <QWidget>
#include <QGraphicsView>
#include <QHash>

#include <optional>
#include <functional>

template < class T >
class SimpleSwitcher
{
public:
    using TriggerType = std::function< void ( T, bool ) >;
public:
    SimpleSwitcher( std::optional< T > var = std::nullopt,
                    TriggerType triggerFunc = TriggerType() )
        : m_var(), m_trigger()
    {}

    void turnOn( T newVal )
    {
        if ( m_var.has_value() )
        {
            m_trigger( m_var.value(), false );
        }
        m_var = newVal;
        m_trigger( m_var.value(), true );
    }

    void turnOff()
    {
        if ( m_var.has_value() )
        {
            m_trigger( m_var.value(), false );
        }
        m_var.reset();
    }

    void setSwitchFunc( TriggerType switchFunc )
    {
        m_trigger = switchFunc;
    }

private:
    std::optional< T > m_var;
    TriggerType m_trigger;
};

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
    return qHash( key.inFunc.funcId, seed ) ^ size_t( key.inFunc.pin )
            ^ qHash( key.outFunc.funcId, seed ) ^ size_t( key.outFunc.pin );
}

class FunctionBlockDiagramWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FunctionBlockDiagramWidget(
            int externalOutPinsCount,
            int externalInPinsCount,
            QVector< SFunctionInfo > const & functionList,
            QWidget *parent = nullptr );
    ~FunctionBlockDiagramWidget() override;

public:
    decltype(auto) view() { return m_view; }

signals:

public slots:
    void graphUpdated();

    void blockPinClicked( SFunctionPinIndex funcPinIndex );

    void exportGraphToFile( const QString & filename );

protected:
    void keyPressEvent( QKeyEvent * event ) override;
    void mousePressEvent( QMouseEvent * event ) override;
    void resizeEvent( QResizeEvent * event ) override;

private:
    void setPinSelected(const SFunctionPinIndex & pairIndex, bool isSelected );

private slots:
    void setConnection( const SFunctionPinIndex & inBlock,
                        const SFunctionPinIndex & outBlock,
                        bool hasConnection );

private:
    // View
    QGraphicsView *                                 m_view{};
    QGraphicsScene *                                m_scene{};
    QPointF                                         m_blockAppearPoint{};
    QPointF                                         m_stepAppearPoint{ 0, 40 };
    /** m_blockMap[ i ] == CFunctionGraph->getVertices()[ i ] */
    QHash< int, FunctionBlockItem * >               m_blockMap{};
    FunctionBlockItem *                             m_externalOutBlock{};
    FunctionBlockItem *                             m_externalInBlock{};

    // Model
    FunctionGraph *                                 m_functionGraph{};

    // Connections
    SimpleSwitcher< SFunctionPinIndex >             m_inPinSel{};
    std::optional< SFunctionPinIndex >              m_inPinSelected{ std::nullopt };
    SimpleSwitcher< SFunctionPinIndex >             m_outPinSel{};
    std::optional< SFunctionPinIndex >              m_outPinSelected{ std::nullopt };

    QHash< SConnection, ConnectionItem * >          m_linesMap{};
    std::optional< SConnection >                    m_connectionSelected{ std::nullopt };
};

#endif // CFUNCTIONBLOCKDIAGRAMWIDGET_H
