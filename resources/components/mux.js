/* This is an example for a JS component
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 *
 ******/

var selectedInput;
var topLeft = {};
var inputs = 3;

var setup = function()
{
    ceInit("Mux", inputs, 1);
    ceSetGuiSize(50, 50);
    
    selectedInput = 0;
    topLeft.x = -25;
    topLeft.y = -25;
}

var update = function()
{
    out[0] = inn[selectedInput];
}

var draw = function()
{
    var y = -25 + selectedInput*9;
    ceLine(-25, y, 25, -25);
}

var mousedown = function(x, y)
{
    selectedInput++;
    if (selectedInput >= inputs) {
        selectedInput = 0;
    }
}
