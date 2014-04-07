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
#define PRECISION_FACTOR 100


#define CANVAS_WIDTH    4096
#define CANVAS_HEIGHT   2048

#include <string>
#include <sstream>

typedef enum
{
    TYPE_FLOAT = 0,
    TYPE_INT32 = 1,
    TYPE_BOOLEAN = 2
} ValueType;

typedef enum
{
    TYPE_NONE = 0,
    TYPE_INPUT = 1,
    TYPE_OUTPUT = 2,
    TYPE_DISCONNECT_INPUT = 3,
    TYPE_DISCONNECT_OUTPUT = 4,
} ConnectionType;

typedef enum
{
    COMPONENT_TYPE_PROGRAM,
    COMPONENT_TYPE_NUMBER,
    COMPONENT_TYPE_SPLIT,
    COMPONENT_TYPE_OSCILLATOR,
    COMPONENT_TYPE_EXP,
    COMPONENT_TYPE_JS,
    COMPONENT_TYPE_OSCCONTROLLER,
} ComponentType;

static float roundFloat(float f)
{
    return floorf(f*PRECISION_FACTOR + 0.5f) / PRECISION_FACTOR;
}

static std::string getValueString(float val)
{
    char buff[128];
    sprintf(buff, "%." FLOAT_PRECISION "f", val);
    std::ostringstream str;
    str << buff;
    return str.str();
}

#endif
