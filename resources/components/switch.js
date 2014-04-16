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

var inputs = 2;

var setup = function()
{
    ceSetName("Switch");
    for (var i=0; i<inputs; i++)
    {
        ceAddInput("Source");
    }
    ceAddOutput("Target");
    ceSetGuiSize(40, 20);
    
    state.selectedInput = 0;
}

var update = function()
{
    out[0] = inn[state.selectedInput];
}

var draw = function()
{
    var y = -7 + state.selectedInput*9;
    ceLine(-20, y, 20, -7);
}

var mousedown = function(x, y)
{
    state.selectedInput++;
    if (state.selectedInput >= inputs) {
        state.selectedInput = 0;
    }
}
