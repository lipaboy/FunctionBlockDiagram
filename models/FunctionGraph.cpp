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
