#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <QString>

enum LogicOperations
{
    AND_BOOL = 0,
    OR_BOOL,
    XOR_BOOL,
    NOT_BOOL,

    AND_BIT,
    OR_BIT,
    XOR_BIT,
    NOT_BIT,

    last
};

QString getLogicOperationName( LogicOperations operation );

#endif // OPERATIONS_H
