/*
 * This is an example for a ControlEase JS component
 *
 * functions:
 * - setup: will be called when the component is placed on the canvas
 * - update: will be called every frame BEFORE draw, this is the place to read inputs and write outputs
 * - draw: will be called every frame on draw.
 * - mousedown(x, y): will be called when the user clicks inside the component
 * - mouseup(x, y): will be called when the user release a click inside the component
 * - mousedrag(x, y): will be called when the user drags the mouse inside the component
 *
 ******/

/* the 'state' object is already defined for every component and is the place to store
 * any persistent values that you need to keep between save/load of a patch.
 * Here for example we store the values of the timeline.
 * When a JS component is loaded from a saved patch it will be initialized 
 * by executing the 'setup' function and immediately after it will load all fields saved 
 * in the 'state' object when the component was saved as part of the patch.
 ******/
state.values = [];


var frames = 200;
var height = 50;
var min = 0;
var max = 1;
var framesby2 = frames/2;
var heightby2 = height/2;


var tlPos;
var lastFrameClicked;

var setup = function()
{
    ceSetName("Timeline");
    ceAddInput("Frame");
    ceAddOutput();
    ceSetGuiSize(frames, height);

    for (var i=0; i<frames; i++)
    {
        state.values[i] = 0;
    }
    tlPos = 0;
    lastFrameClicked = -1;
}

var update = function()
{
    tlPos = inn[0];
    if (tlPos > frames-1) {
        tlPos = frames-1;
    }
    else if (tlPos < 0) {
        tlPos = 0;
    }
    
    var prev = state.values[Math.floor(tlPos)];
    var next = state.values[Math.ceil(tlPos)];
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
        ceLine(i-framesby2, heightby2, i-framesby2, heightby2-state.values[i]);
    }
    
    ceBrightness(0);
    ceLine(tlPos-framesby2, heightby2, tlPos-framesby2, -heightby2);
}

var mousedown = function(_x, _y)
{
    var i = Math.floor(_x);
    if (i<0 || i>frames) {
        return;
    }
    
    state.values[i] = height-_y;
    lastFrameClicked = i;
}

var mousedrag = function(_x, _y)
{
    var f = Math.floor(_x);
    if (f<0 || f>=frames) {
        return;
    }
    
    if (lastFrameClicked == -1) {
        lastFrameClicked = f;
    }
    
    var inc = f>lastFrameClicked?1:-1;
    for (var i=lastFrameClicked; i!=f; i+=inc) {
        state.values[i] = height-_y;
    }
    // set current
    state.values[f] = height-_y;
    lastFrameClicked = f;
}
