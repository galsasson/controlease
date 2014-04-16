/* This is an example for a JS component
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 *
 ******/

var currentValue;
var maxValue;

var setup = function()
{
    ceSetName("Balloon");
    ceAddInput("Trigger");
    ceAddOutput();
    ceSetGuiSize(40, 30);
    
    currentValue = 0;
}

var update = function()
{
    if (inn[0] > 0) {
        maxValue = currentValue = inn[0];
    }
    else {
        currentValue *= 0.85;
    }
    
    out[0] = currentValue;
}

var draw = function()
{
    var size = 15*(currentValue / maxValue);
    ceEllipse(0, 0, size, size);
}

