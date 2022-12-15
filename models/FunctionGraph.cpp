#include "FunctionGraph.h"

FunctionGraph::FunctionGraph( QObject * parent )
    : QObject( parent )
{

}

void FunctionGraph::loadVertices( QVector< SFunctionInfo > funcInfos )
{
    for ( auto & info : funcInfos )
    {
        SFunctionNode node{};
        node.name = info.funcName;
        node.inputPins.resize( info.inputPinCount );
        node.outputPins.resize( info.outputPinCount );
        m_vertices.push_back( node );
    }

    emit updated();
}

void FunctionGraph::connectVertices( const SFunctionPinIndex & inVertex,
                                     const SFunctionPinIndex & outVertex )
{
    if ( inVertex.func != outVertex.func )
    {
        m_vertices[ inVertex.func ].inputPins[ inVertex.pin ] = outVertex;
        m_vertices[ outVertex.func ].outputPins[ outVertex.pin ] = inVertex;

        emit connectionChanged( inVertex, outVertex, true );
    }
}
