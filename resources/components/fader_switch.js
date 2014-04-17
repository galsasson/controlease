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
 * Here for example we store the current selected input.
 * When a JS component is loaded from a saved patch it will be initialized
 * by executing the 'setup' function and immediately after it will load all fields saved
 * in the 'state' object when the component was saved as part of the patch.
 ******/

state.inputPos = 0;
state.outputPos = 0;

var height = 20;
var heightby2 = height/2;

var setup = function()
{
    ceSetName("Fader Switch");
    ceAddInput("Source 1");
    ceAddInput("Source 2");
    ceAddOutput("Target 1");
    ceAddOutput("Target 2");
    ceSetGuiSize(40, height);
}

var update = function()
{
    var val = inn[0]*((height-state.inputPos)/height) + inn[1]*(state.inputPos/height);
    
    out[0] = val * ((height-state.outputPos)/height);
    out[1] = val * (state.outputPos/height);
}

var draw = function()
{
    ceLine(-20, -heightby2+state.inputPos, 0, -heightby2 + state.inputPos);
    ceLine(0, -heightby2+state.outputPos, 20, -heightby2+state.outputPos);
}

var mousedown = function(x, y)
{
    if (x < 20) {
        state.inputPos = y;
    }
    else {
        state.outputPos = y;
    }
}

var mousedrag = function(x, y)
{
    if (x < 20) {
        state.inputPos = y;
    }
    else {
        state.outputPos = y;
    }
}