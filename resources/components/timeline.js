/* This is an example for a JS component
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 *
 ******/


var frames = 200;
var height = 50;
var min = 0;
var max = 1;
var framesby2 = frames/2;
var heightby2 = height/2;

var values = [];

var tlPos;
var lastFrameClicked;

var setup = function()
{
    ceInit("Timeline", 1, 1);
    ceSetGuiSize(frames+10, height);

    for (var i=0; i<frames; i++)
    {
        values[i] = 0;
    }
    tlPos = 0;
    lastFrameClicked = -1;
}

var update = function()
{
    tlPos = inn[0];
    if (tlPos >= frames) {
        tlPos = frames-1;
    }
    else if (tlPos < 0) {
        tlPos = 0;
    }
    
    var prev = values[Math.floor(tlPos)];
    var next = values[Math.ceil(tlPos)];
    var diff = next - prev;
    var amount = tlPos - Math.floor(tlPos);
    out[0] = ceMap(prev + diff*amount, 0, height, min, max);
}

var draw = function()
{
    ceRect(-framesby2, -heightby2, frames, height);
    
    ceBrightness(0.5);
    for (var i=0; i<frames; i++)
    {
        ceLine(i-framesby2, heightby2, i-framesby2, heightby2-values[i]);
    }
    
    ceBrightness(0);
    ceLine(tlPos-framesby2, heightby2, tlPos-framesby2, -heightby2);
}

var mousedown = function(_x, _y)
{
    var i = Math.floor(_x)-5;
    if (i<0 || i>frames) {
        return;
    }
    
    values[i] = height-_y;
    lastFrameClicked = i;
}

var mousedrag = function(_x, _y)
{
    var f = Math.floor(_x)-5;
    if (f<0 || f>=frames) {
        return;
    }
    
    if (lastFrameClicked == -1) {
        lastFrameClicked = Math.floor(_x)-5;
    }
    
    var inc = f>lastFrameClicked?1:-1;
    for (var i=lastFrameClicked; i!=f; i+=inc) {
        values[i] = height-_y;
    }
    // set current
    values[f] = height-_y;
    lastFrameClicked = f;
}
