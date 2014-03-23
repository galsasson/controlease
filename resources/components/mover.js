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
    ceInit("Mover", 2, 1);
    ceSetGuiSize(30, 30);
    
    currentValue = 0;
}

var update = function()
{
    ceSetOffset(inn[0], inn[1]);
}

var draw = function()
{
}
