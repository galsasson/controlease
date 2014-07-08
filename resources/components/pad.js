/* This is an example for a JS component
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 *
 ******/

state.pos = [50, 50];
var bSendClick = 0;

var setup = function()
{
    ceSetName("Pad");
    ceAddInput("scale");
    ceAddOutput("x");
    ceAddOutput("y");
    ceAddOutput("click");
    ceSetGuiSize(100, 100);
    
    bSendClick = 0;
}

var update = function()
{
    var scale = inn[0];
    
    out[0] = (state.pos[0]/50-1)*scale;
    out[1] = (state.pos[1]/50-1)*scale;
    out[2] = bSendClick;
}

var draw = function()
{
    ceRect(-50, -50, 100, 100);
    ceEllipse(state.pos[0]-50, state.pos[1]-50, 5, 5);
}

var mousedown = function(x, y)
{
    state.pos[0] = x;
    state.pos[1] = y;
    bSendClick = 1;
}

var mousedrag = function(x, y)
{
    state.pos[0] = x;
    state.pos[1] = y;
}

var mouseup = function(x, y)
{
    bSendClick = 0;
}


