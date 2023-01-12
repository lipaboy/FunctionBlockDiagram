#include "SFunctionDiagram.h"

//SFunctionDiagram::SFunctionDiagram()
//{

//}

//SFunctionDiagram::SFunctionDiagram( const QVector< SFunctionNode > &functionNodes,
//                                    int externalOutPinsCount,
//                                    int externalInPinsCount )
//    : m_functionNodes( functionNodes )
//{
//    if ( externalOutPinsCount > 0 )
//    {
//        SFunctionNode node{};
//        node.outPins.resize( externalOutPinsCount );
//        m_functionNodes.push_back( node );
//        m_externalOutPinsInd = m_functionNodes.size() - 1;
//    }
//    if ( externalInPinsCount > 0 )
//    {
//        SFunctionNode node{};
//        node.inPins.resize( externalInPinsCount );
//        m_functionNodes.push_back( node );
//        m_externalOutPinsInd = m_functionNodes.size() - 1;
//    }
//}

//SFunctionPinIndexOpt & SFunctionDiagram::rget( bool isIn,
//                                               const SFunctionPinIndex & index)
//{
//    return isIn ? m_functionNodes[ index.func ].inPins[ index.pin ]
//            : m_functionNodes[ index.func ].outPins[ index.pin ];
//}
