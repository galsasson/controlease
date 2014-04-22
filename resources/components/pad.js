/* This is an example for a JS component
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 *
 ******/

state.pos = [0, 0];
var bSendClick = false;

var setup = function()
{
    ceSetName("Pad");
    ceAddOutput("x");
    ceAddOutput("y");
    ceAddOutput("click");
    ceSetGuiSize(100, 100);
    
    bSendClick = false;
}

var update = function()
{
    out[0] = state.pos[0]/50-1;
    out[1] = state.pos[1]/50-1;
    out[2] = bSendClick;
    bSendClick = 0;
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
    bSendClick = true;
}

var mousedrag = function(x, y)
{
    state.pos[0] = x;
    state.pos[1] = y;
    bSendClick = true;
}


