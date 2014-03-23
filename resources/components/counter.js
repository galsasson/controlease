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
    ceInit("Counter", 1, 1);
    ceSetGuiSize(30, 30);
    
    currentValue = 0;
}

var update = function()
{
    var step = inn[0];
    
    if (step == 0) {
        return;
    }

    currentValue += step;
    if (currentValue < 0) {
        currentValue = 0;
    }

    out[0] = currentValue;
}

var draw = function()
{
    var currentAngle = ceMap(currentValue%100, 0, 100, 0, Math.PI*2);
    var y = -Math.cos(currentAngle) * 15;
    var x = Math.sin(currentAngle)*15;
    
    ceEllipse(0, 0, 15, 15);
    ceLine(0, 0, x, y);
}
