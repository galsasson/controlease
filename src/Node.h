//
//  Node.h
//  Controlease
//
//  Created by Gal Sasson on 2/21/14.
//
//

#ifndef Controlease_Node_h
#define Controlease_Node_h

class InputNode
{
public:
    ~InputNode() {};
    virtual void updateVal(float val) = 0;
};

class OutputNode
{
public:
    ~OutputNode() {};
    virtual void connect(InputNode *node) = 0;
};

class Connectable
{
public:
    
    Connectable(int t);
};

#endif
