
class Particle
{
  PVector pos;
  PVector vel;
  PVector acc;
  float maxVel;
  
  public Particle(float x, float y)
  {
    pos = new PVector(x, y);
    vel = new PVector(random(3), random(3));
    acc = new PVector();
    
    maxVel = random(5);
  }
  
  public void applyForce(PVector f)
  {
    acc.add(f);
  }
  
  public void avoidMouse()
  {
    PVector m = new PVector((float)mouseX, (float)mouseY);
    PVector dist = PVector.sub(pos,m);
    if (dist.mag() < 60) {
      float mag = dist.mag()/10;
      dist.normalize();
      dist.mult(6-mag);
      applyForce(dist);
    }
  }
  
  public void update()
  {
    avoidMouse();
    
    vel.add(acc);
    pos.add(vel);
    
    if (vel.mag() > maxVel)
    {
      vel.mult(0.95);
    }
    
    acc.set(0, 0);
    
    applyBounds();
  }
  
  public void draw()
  {
    pushMatrix();
    translate(pos.x, pos.y);
    fill(255, 150);
    noStroke();
    ellipse(0, 0, c.get("Particle size"), c.get("Particle size"));
    popMatrix();
  }
  
  public void drawLineTo(Particle p, float maxDistance)
  {
    PVector diff = pos.get();
    diff.sub(p.pos);
    if (diff.mag() < maxDistance)
    {
      line(pos.x, pos.y, p.pos.x, p.pos.y);
    }
  }
  
  private void applyBounds()
  {
    if (pos.x < 0) {
      pos.x = 0;
      vel.x *= -1;
    }
    else if (pos.x > width) {
      pos.x = width;
      vel.x *= -1;
    }
    
    if (pos.y < 0) {
      pos.y = 0;
      vel.y *= -1;
    }
    else if (pos.y > height) {
      pos.y = height;
      vel.y *= -1;
    }
  }
}
