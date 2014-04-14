/* This is an example for a JS component
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 *
 ******/

var lastClickTime = 0;
var timeDiff = -1;
var counter = 0;
var lastUpdateTime=0;
var nClicks = 0;

var setup = function()
{
    ceInit("Rhythm", 0, 1);
    ceSetGuiSize(40, 20);
}

var update = function()
{
    if (nClicks < 8) {
        return;
    }
    
    var updateTime = getCurrentTime();
    counter += updateTime - lastUpdateTime;
    if (counter > timeDiff) {
        out[0] = 1;
        counter -= timeDiff;
    }
    else {
        out[0] = 0;
    }
    
    lastUpdateTime = updateTime;
}

var draw = function()
{
    ceDrawString(nClicks, 0, 0);
}

var mousedown = function(x, y)
{
    if (nClicks > 7) {
        nClicks = 0;
    }
    
    var curTime = getCurrentTime();
    nClicks++;
    if (nClicks > 1)
    {
        if (nClicks == 2) {
            // second click
            timeDiff = curTime - lastClickTime;
            ceLog("second click ("+(curTime - lastClickTime)+"): " + timeDiff);
        }
        else {
            // third or more, average timing
            timeDiff += curTime - lastClickTime;
            timeDiff /= 2;
            ceLog("third or more ("+(curTime - lastClickTime)+"): " + timeDiff);
        }
    }
    lastClickTime = curTime;
    counter = 0;
    lastUpdateTime = curTime;
}

var getCurrentTime = function()
{
    var t = new Date();
    return t.getTime();
}
