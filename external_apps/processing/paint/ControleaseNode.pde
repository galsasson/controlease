

class ControleaseNode
{
  String name;
  int index;
  float val;
  int type;
  
  public ControleaseNode(String name, int index, float v, int t)
  {
    this.name = name;
    this.index = index;
    val = v;
    type = t;
  }
  
  public String getName()
  {
    return name;
  }
  
  public int getType()
  {
    return type;
  }
  
  public void setVal(float v)
  {
    val = v;
  }
  
  public float getFloat()
  {
    return val;
  }
  
  public int getInt()
  {
    return (int)val;
  }
  
  public boolean getBool()
  {
    return (val!=0);
  }
}
  
  
  
  
  
