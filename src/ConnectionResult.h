//
//  ConnectionResult.h
//  Controlease
//
//  Created by Gal Sasson on 2/22/14.
//
//

#ifndef Controlease_ConnectionResult_h
#define Controlease_ConnectionResult_h

#include "Controlease.h"
#include "Node.h"

class ConnectionResult
{
public:
    
    ConnectionResult(ConnectionType t, Node *n) {type = t; node = n; }
    
    ConnectionType type;
    Node *node;
};
#endif
