

Controlease c;

void setup()
{
  size(800, 600);
  smooth();
  colorMode(HSB, 360, 1, 1, 1);
  frameRate(60);
  initControls();
  
  background(0);
}

void initControls()
{
  c = new Controlease("Paint", 6666);
  
  c.addInput("Hue", 1);
  c.addInput("Saturation", 0);
  c.addInput("Brightness", 1);
  c.addInput("Alpha", 0.5);
  c.addInput("Num Strokes", 5);
  c.addInput("Brush Size", 10);
  c.addInput("Width", 1);
  c.addInput("Height", 1);
  c.addInput("MouseX", 0);
  c.addInput("MouseY", 0);
  
  c.addInput("ClearScreen", 0);
}

void draw()
{
  if (c.get("ClearScreen") > 0) {
    background(0);
  }
  
  pushMatrix();
  translate(c.get("MouseX"), c.get("MouseY"));
  
  fill(c.get("Hue"), c.get("Saturation"), c.get("Brightness"), c.get("Alpha"));
  noStroke();
  
  int numStrokes = c.getInt("Num Strokes");
  float size = c.get("Brush Size");
  float w = c.get("Width");
  float h = c.get("Height");
  
  for (int i=0; i<numStrokes; i++)
  {
    pushMatrix();
    rotate(random(TWO_PI));
    ellipse(random(size), 0, w, h);
    popMatrix();
  }

  popMatrix();  
}

