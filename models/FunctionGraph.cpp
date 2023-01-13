#include "FunctionGraph.h"

FunctionGraph::FunctionGraph( int externalOutPinsCount,
                              int externalInPinsCount,
                              QObject * parent )
    : QObject( parent )
{
    if ( externalOutPinsCount > 0 )
    {
        SFunctionNode node{};
        node.outPins.resize( externalOutPinsCount );
        node.id = m_externalOutPinsInd = m_functionNodes.size();
        m_functionNodes.push_back( node );
    }
    if ( externalInPinsCount > 0 )
    {
        SFunctionNode node{};
        node.inPins.resize( externalInPinsCount );
        node.id = m_externalInPinsInd = m_functionNodes.size();
        m_functionNodes.push_back( node );
    }
}

void FunctionGraph::loadVertices( QVector< SFunctionInfo > funcInfos )
{
    for ( auto & info : funcInfos )
    {
        SFunctionNode node{};
        node.name = info.funcName;
        node.inPins.resize( info.inputPinCount );
        node.outPins.resize( info.outputPinCount );
        node.id = m_functionNodes.size();
        m_functionNodes.push_back( node );
    }

    emit updated();
}

void FunctionGraph::connectVertices( const SFunctionPinIndex & inVertex,
                                     const SFunctionPinIndex & outVertex )
{
    SFunctionPinIndexOpt & rInPin = unzipInIndex( inVertex );
    SFunctionPinIndexOpt & rOutPin = unzipOutIndex( outVertex );
    if ( inVertex.funcId != outVertex.funcId
         && ! rInPin.has_value()
         && ! rOutPin.has_value() )
    {
        rInPin = outVertex;
        rOutPin = inVertex;

        emit connectionChanged( inVertex, outVertex, true );
    }
}

void FunctionGraph::disconnectVertices( const SFunctionPinIndex & inVertex,
                                        const SFunctionPinIndex & outVertex )
{
    SFunctionPinIndexOpt & rInPin = unzipInIndex( inVertex );
    SFunctionPinIndexOpt & rOutPin = unzipOutIndex( outVertex );
    if ( rInPin == outVertex && rOutPin == inVertex )
    {
        rInPin.reset();
        rOutPin.reset();

        emit this->connectionChanged( inVertex, outVertex, false );
    }
}

SFunctionPinIndexOpt & FunctionGraph::rget( bool isIn,
                                            const SFunctionPinIndex & index)
{
    return isIn ? m_functionNodes[ index.funcId ].inPins[ index.pin ]
            : m_functionNodes[ index.funcId ].outPins[ index.pin ];
}

SFunctionPinIndexOpt & FunctionGraph::unzipInIndex( const SFunctionPinIndex & index )
{
    return rget( true, index );
}

SFunctionPinIndexOpt &FunctionGraph::unzipOutIndex(const SFunctionPinIndex &index)
{
    return rget( false, index );
}
