#ifndef SFUNCTIONDIAGRAM_H
#define SFUNCTIONDIAGRAM_H

#include <QVector>
#include <optional>

struct SFunctionPinIndex
{
    int func{};
    int pin{};

    bool operator==( SFunctionPinIndex const & other ) const
    {
        return func == other.func && pin == other.pin;
    }
    bool operator!=( SFunctionPinIndex const & other ) const
    {
        return ! ( *this == other );
    }
};

using SFunctionPinIndexOpt = std::optional< SFunctionPinIndex >;

struct SFunctionNode
{
//    int                                 id{};
    QString                             name{};
    /** Связь либо есть, либо её нет. Но pin существует в любом случае */
    QVector< SFunctionPinIndexOpt >     inPins{};
    QVector< SFunctionPinIndexOpt >     outPins{};
};

//struct SFunctionDiagram
//{
//public:
//    SFunctionDiagram();
//    SFunctionDiagram( const QVector< SFunctionNode > & functionNodes,
//                      int externalOutPinsCount,
//                      int externalInPinsCount );

//public:
//    QVector< SFunctionNode > getFunctionNodes() const { return m_functionNodes; }
//    SFunctionNode getExternalOutPins() const
//    {
//        return m_functionNodes[ m_externalOutPinsInd ];
//    }
//    SFunctionNode getExternalInPins() const
//    {
//        return m_functionNodes[ m_externalInPinsInd ];
//    }

//public:
//    SFunctionPinIndexOpt & rget( bool isIn, SFunctionPinIndex const & index );
//    void add( SFunctionNode node ) { m_functionNodes.push_back( std::move( node ) ); }
////    void setExternalInPinsCount( i

//    bool isEmpty() const { return m_functionNodes.empty(); }

//private:
//    QVector< SFunctionNode >            m_functionNodes{};
//    int                                 m_externalOutPinsInd{};
//    int                                 m_externalInPinsInd{};
//};

#endif // SFUNCTIONDIAGRAM_H
