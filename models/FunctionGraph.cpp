#include "FunctionGraph.h"

FunctionGraph::FunctionGraph( int externalOutPinsCount,
                              int externalInPinsCount,
                              QObject * parent )
    : QObject( parent )
    , m_externalOutPins( externalOutPinsCount, std::nullopt )
    , m_externalInPins( externalInPinsCount, std::nullopt )
{

}

void FunctionGraph::loadVertices( QVector< SFunctionInfo > funcInfos )
{
    for ( auto & info : funcInfos )
    {
        SFunctionNode node{};
        node.name = info.funcName;
        node.inPins.resize( info.inputPinCount );
        node.outPins.resize( info.outputPinCount );
        m_vertices.push_back( node );
    }

    emit updated();
}

void FunctionGraph::connectVertices( const SFunctionPinIndex & inVertex,
                                     const SFunctionPinIndex & outVertex )
{
    SFunctionPinIndexOpt & rInPin = unzipInIndex( inVertex );
    SFunctionPinIndexOpt & rOutPin = unzipOutIndex( outVertex );
    if ( inVertex.func != outVertex.func
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

SFunctionDiagram FunctionGraph::getDiagram() const
{

}

SFunctionPinIndexOpt & FunctionGraph::unzipInIndex( const SFunctionPinIndex & index )
{
    return m_vertices[ index.func ].inPins[ index.pin ];
}

SFunctionPinIndexOpt &FunctionGraph::unzipOutIndex(const SFunctionPinIndex &index)
{
    return m_vertices[ index.func ].outPins[ index.pin ];
}
