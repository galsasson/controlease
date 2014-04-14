/* This is an example for a JS component
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 *
 ******/

var bSendOne;

var setup = function()
{
    ceInit("Button", 0, 1);
    ceSetGuiSize(30, 30);
    
    bSendOne = false;
}

var update = function()
{
    out[0] = bSendOne?1:0;
    bSendOne = false;
}

var draw = function()
{
    if (bSendOne) {
        ceDrawEllipse(0, 0, 10, 10);
    }
}

var mousedown = function(x, y)
{
    bSendOne = true;
}

var getCurrentTime = function()
{
    var t = new Date();
    return t.getTime();
}
