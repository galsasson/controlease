//
//  Controlease.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef Controlease_Controlease_h
#define Controlease_Controlease_h

#define FLOAT_PRECISION "2"

typedef enum _ValueType
{
    TYPE_INT32 = 1,
    TYPE_FLOAT = 2,
    TYPE_BOOLEAN = 3
} ValueType;

typedef enum _ConnectionType
{
    TYPE_NONE = 0,
    TYPE_INPUT = 1,
    TYPE_OUTPUT = 2
} ConnectionType;

#endif
