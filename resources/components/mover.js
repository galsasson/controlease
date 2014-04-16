/* This is an example for a JS component
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 *
 ******/

var currentValue;

var setup = function()
{
    ceSetName("Mover")
    ceAddInput("X");
    ceAddInput("Y");
    ceAddOutput("X");
    ceAddOutput("Y");
    ceSetGuiSize(30, 10);
    
    currentValue = 0;
}

var update = function()
{
    var pos = {'x':inn[0], 'y':inn[1]};
    ceSetOffset(pos.x, pos.y);
    out[0] = pos.x;
    out[1] = pos.y;
}

var draw = function()
{
}
