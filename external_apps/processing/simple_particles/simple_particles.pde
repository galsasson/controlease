

ArrayList<Particle> particles;

Controlease c;

void setup()
{
  size(500, 500);
  smooth();
  frameRate(30);
  
  particles = new ArrayList<Particle>();
  for (int i=0; i<100; i++)
  {
    particles.add(new Particle(random(width), random(height)));
  }
  
  initControls();
}

void draw()
{
  background(0);
 
  for (Particle p: particles)
  {
    p.update();
    p.draw();
    stroke(255, 50);
    float sw = c.get("Line width");
    sw = sw>0?sw:0;
    strokeWeight(sw);
    for (Particle p2: particles)
    {
      p.drawLineTo(p2, c.get("Minimum distance"));
    }
  }
  
  c.set("Mouse X", mouseX);
  c.set("Mouse Y", mouseY);
}

void initControls()
{
  c = new Controlease("Particle example sketch", 7777);
  
  c.addInput("Particle size", 8);
  c.addInput("Minimum distance", 30);
  c.addInput("Line width", 1);
  
  c.addOutput("Mouse X", 0);
  c.addOutput("Mouse Y", 0);
}
