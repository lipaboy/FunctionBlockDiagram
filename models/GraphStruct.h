#ifndef GRAPHSTRUCT_H
#define GRAPHSTRUCT_H

#include <optional>

#include <QString>
#include <QVector>

struct SFunctionPinIndex
{
    enum EPinType
    {
        OUT = 0,
        IN,
    };

    EPinType pinType{};
    int funcId{};
    int pin{};

    SFunctionPinIndex()
    {}
    SFunctionPinIndex( EPinType type, int functionId, int pinIndex )
        : pinType( type ), funcId( functionId ), pin( pinIndex )
    {}

    bool operator==( SFunctionPinIndex const & other ) const
    {
        return pinType == other.pinType
                && funcId == other.funcId
                && pin == other.pin;
    }
    bool operator!=( SFunctionPinIndex const & other ) const
    {
        return ! ( *this == other );
    }
};

using SFunctionPinIndexOpt = std::optional< SFunctionPinIndex >;

struct SFunctionNode
{
    /** id - уникальный идентификатор узла (вершины) в графе. Каждая функция
     *  даже с одинаковым типом должна иметь разные id.
     */
    int                                 id{};
    /** Под типом функции может подразумеваться адрес её в микроконтроллере,
     *  либо идентификатор поведения функции (логическое И и т.п.).
     *  (Пока не используется. Задел на будущее)
     */
    int                                 type{};
    QString                             name{};
    /** Связь либо есть, либо её нет (поэтому используем std::optional).
     *  Но pin существует в любом случае (место в массиве выделяется в любом случае). */
    QVector< SFunctionPinIndexOpt >     inPins{};
    QVector< SFunctionPinIndexOpt >     outPins{};

    SFunctionNode( const QString & funcName,
                   int inPinCount, int outPinCount )
        : id( 0 )
        , name( funcName )
        , inPins( inPinCount )
        , outPins( outPinCount )
    {

    }
    SFunctionNode()
    {

    }
};

struct SFunctionInfo
{
    QString         funcName{};
    int             inputPinCount{};
    int             outputPinCount{};
};

#endif // GRAPHSTRUCT_H
