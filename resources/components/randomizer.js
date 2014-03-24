/* This is an example for a JS component
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 *
 ******/

var currentValue;
var lookCounter;
var bConnected;
var targetNode;

var setup = function()
{
    ceInit("Randomizer", 0, 1);
    ceSetGuiSize(30, 10);
    
    currentValue = 0;
    lookCounter = 0;
    bConnected = false;
}

var update = function()
{
    if (Math.random() > 0.1)
    {
        lookForInputNode();
    }
}

var draw = function()
{
    if (targetNode != undefined) {
        ceDrawString(targetNode.name, -15, 5);
    }
}


var lookForInputNode = function()
{
    var nodes = ceGetCanvasInputs(100);
    if (nodes.length == 0) {
        // no input node found, disconnect
        ceDisconnectOutput(0);
        targetNode = undefined;
        return;
    }
    
    var available
    
    var closest = 101;
    var closestNode;
    for (var i=0; i<nodes.length; i++)
    {
        if (nodes[i].distance < closest && !nodes[i].connected) {
            closest = nodes[i].distance;
            closestNode = nodes[i];
        }
    }
    
    ceDisconnectOutput(0);
    targetNode = undefined;
    if (ceConnectOutput(0, closestNode.id))
    {
        targetNode = closestNode;
        out[0] = Math.random()*100;
    }
}