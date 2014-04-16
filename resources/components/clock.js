/* This is an example for a JS component
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 *
 ******/

var currentValue;
var bang;

var setup = function()
{
    ceSetName("Clock");
    ceAddInput("Period");
    ceAddInput("Speed");
    ceAddOutput("Value");
    ceAddOutput("Loop Start");
    
    ceSetGuiSize(50, 50);
    
    currentValue = 0;
    bang = false;
}

var update = function()
{
    var period = inn[0];
    var step = inn[1];
    
    if (step == 0) {
        return;
    }

    // handle bang off
    if (bang == true) {
        bang = false;
        out[1] = 0;
    }

    currentValue += step;
    if (currentValue >= period) {
        currentValue = 0;
        // handle bang
        if (!bang) {
            bang = true;
            out[1] = 1;
        }
    }
    else if (currentValue < 0) {
        currentValue = period + currentValue;
    }
    
    out[0] = currentValue;
}

var draw = function()
{
    var period = inn[0];
    var currentAngle = ceMap(currentValue, 0, period, 0, Math.PI*2);
    var y = -Math.cos(currentAngle) * 25;
    var x = Math.sin(currentAngle)*25;
    
    ceEllipse(0, 0, 25, 25);
    ceLine(0, 0, x, y);
}
