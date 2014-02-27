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

#define CANVAS_WIDTH    2048
#define CANVAS_HEIGHT   4096


typedef enum _ValueType
{
    TYPE_FLOAT = 0,
    TYPE_INT32 = 1,
    TYPE_BOOLEAN = 2
} ValueType;

typedef enum _ConnectionType
{
    TYPE_NONE = 0,
    TYPE_INPUT = 1,
    TYPE_OUTPUT = 2,
    TYPE_DISCONNECT_INPUT = 3,
    TYPE_DISCONNECT_OUTPUT = 4,
} ConnectionType;

#endif
