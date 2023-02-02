#include "FunctionGraph.h"

#include <QSaveFile>

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

void FunctionGraph::saveToFile(const QString &filename)
{
    QSaveFile file( filename );
    file.open( QIODevice::WriteOnly );

    QByteArray data{};

    auto appendUint8 = [] ( QByteArray & data, quint8 r ) -> QByteArray &
    {
        return data.append( r );
    };
    auto appendUint16 =
            [ appendUint8 ] ( QByteArray & data, quint16 r ) -> QByteArray &
    {
        return appendUint8( appendUint8( data, r >> 8 ), r );
    };

    for ( int i = 0; i < m_functionNodes.size(); i++ )
    {
        auto & node = m_functionNodes[ i ];
        /** Индекс блока */
        appendUint16( data, static_cast< quint16 >( i ) );
        /** Адрес функции в микроконтроллере */
        appendUint16( data, static_cast< quint16 >( 0 ) );
        for ( int inPinInd = 0; inPinInd < node.inPins.size(); ++inPinInd )
        {
            auto & inPinOpt = node.inPins[ inPinInd ];
            if ( inPinOpt.has_value() )
            {
                auto inPin = inPinOpt.value();
                /** Индекс входного пина */
                appendUint16( data, static_cast< quint16 >( inPinInd ) );
                quint16 zip = inPin.funcId;
                quint16 mask = ~( ( quint16( 1 ) << 4 ) - 1 );
                zip <<= 4;
                zip = ( mask & zip ) | static_cast< quint16 >( inPin.pin );
                appendUint16( data, zip );
//                appendUint16( data, static_cast< quint16 >( inPin.funcId ) );
//                appendUint16( data, static_cast< quint16 >( inPin.pin ) );
            }
        }
        data.append( '\n' ).append( '\r' );
    }

    file.write( data );

    file.commit();
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

void FunctionGraph::addOperation( LogicOperations operation )
{
    int id = m_functionNodes.empty() ? 0 : m_functionNodes.back().id + 1;
    m_functionNodes.push_back( SFunctionNode{ id, "AND Logic", 2, 1 } );
    emit updated();
}
