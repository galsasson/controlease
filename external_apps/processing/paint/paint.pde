

Controlease c;

void setup()
{
  size(400, 600);
  smooth();
  colorMode(HSB, 360, 1, 1, 1);
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
  
  for (int i=0; i<5; i++)
  {
    pushMatrix();
    rotate(random(TWO_PI));
    ellipse(random(c.get("Brush Size")), 0, c.get("Width"), c.get("Height"));
    popMatrix();
  }

  popMatrix();  
}

