//
//  Node.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef Controlease_Node_h
#define Controlease_Node_h

#include "cinder/app/App.h"
#include "Controlease.h"

class Node
{
public:
    ~Node() {};
    
    Node *prev;
    Node *next;
    
    virtual void updateVal(float val) = 0;
    virtual void connect(Node *node) = 0;
    virtual void disconnect(Node *node) = 0;
    virtual bool isConnected() = 0;

};

#endif
