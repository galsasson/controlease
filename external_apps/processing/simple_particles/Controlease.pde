
import oscP5.*;
import netP5.*;
import java.util.Set;

final int TYPE_FLOAT = 0;
final int TYPE_INT = 1;
final int TYPE_BOOL = 2;


class Controlease
{
  String name;
  int inputNum;
  int outputNum;
  HashMap<String, ControleaseNode> inputsByName;
  ArrayList<ControleaseNode> inputsByIndex;
//  HashMap<Integer, ControleaseNode> inputsByIndex;  
  HashMap<String, ControleaseNode> outputsByName;
  ArrayList<ControleaseNode> outputsByIndex;
//  HashMap<Integer, ControleaseNode> outputsByIndex;

  OscP5 controlease_oscP5;
  NetAddress controleaseAddr;
  boolean connected;
  
  
  public Controlease(String appName, int port)
  {
    name = appName;
    inputNum = 0;
    outputNum = 0;
    
    inputsByName = new HashMap<String, ControleaseNode>();
    inputsByIndex = new ArrayList<ControleaseNode>();
//    inputsByIndex = new HashMap<Integer, ControleaseNode>();
    outputsByName = new HashMap<String, ControleaseNode>();
    outputsByIndex = new ArrayList<ControleaseNode>();
//    outputsByIndex = new HashMap<Integer, ControleaseNode>();
    
    controlease_oscP5 = new OscP5(this, port);
    connected = false;
  }
  
  
  
  public void oscEvent(OscMessage msg) {
    String type = msg.addrPattern();
    
    if (type.contains("/ic")) {
      int index = msg.get(0).intValue();
      float value = msg.get(1).floatValue();
      inputsByIndex.get(index).setVal(value);
    }
    
    if (type.contains("/alive?")) {
      String controleaseHost = msg.get(0).stringValue();
      int controleasePort = msg.get(1).intValue();
      println("params: " + controleaseHost + ":" + controleasePort);
      controleaseAddr = new NetAddress(controleaseHost, controleasePort);
      
      // send the alive message
      OscMessage aliveMsg = new OscMessage("/alive!");
      aliveMsg.add(name);
      controlease_oscP5.send(aliveMsg, controleaseAddr);
      
      // send all the input points
      for (int i=0; i<inputNum; i++)
      {
        OscMessage inputMsg = new OscMessage("/input_node");
        ControleaseNode node = inputsByIndex.get(i);
        inputMsg.add(node.getName());  // name
        inputMsg.add("/ic");          // address
        inputMsg.add(i);                        // index
        inputMsg.add(node.getType());                    // type
        inputMsg.add(node.getFloat());         // value
        controlease_oscP5.send(inputMsg, controleaseAddr);
      }
      
      // send all the output points
      for (int i=0; i<outputNum; i++)
      {
        OscMessage outputMsg = new OscMessage("/output_node");
        ControleaseNode node = outputsByIndex.get(i);
        outputMsg.add(node.getName());  // name
        outputMsg.add(i);                       // index
        outputMsg.add(node.getType());          // type
        outputMsg.add(node.getFloat());         // value
        controlease_oscP5.send(outputMsg, controleaseAddr);
      }

      // send end params message
      OscMessage endMsg = new OscMessage("/end_nodes");
      controlease_oscP5.send(endMsg, controleaseAddr);
      connected = true;
    }
  }

  
  
  
  
  
  
  
  
  
  
  
  
  public void addInput(String n, float initVal)
  {
    ControleaseNode cval = new ControleaseNode(n, inputNum, initVal, TYPE_FLOAT);
    inputsByName.put(n, cval);
    inputsByIndex.add(cval);
    inputNum++;
  }
  
  public void addInput(String n, int initVal)
  {
    ControleaseNode cval = new ControleaseNode(n, inputNum, (float)initVal, TYPE_INT);
    inputsByName.put(n, cval);
    inputsByIndex.add(cval);
    inputNum++;
  }
  
  public void addInput(String n, boolean initVal)
  {
    ControleaseNode cval = new ControleaseNode(n, inputNum, initVal?1f:0f, TYPE_BOOL);
    inputsByName.put(n, cval);
    inputsByIndex.add(cval);
    inputNum++;
  }
  
  public void addOutput(String n, int type)
  {
    ControleaseNode node = new ControleaseNode(n, outputNum, 0, type);
    outputsByName.put(n, node);
    outputsByIndex.add(node);
    outputNum++;
  }
  
  public float get(String n)
  {
    return inputsByName.get(n).getFloat();
  }
  
  public int getInt(String n)
  {
    return inputsByName.get(n).getInt();
  }
  
  public boolean getBool(String n)
  {
    return inputsByName.get(n).getBool();
  }
  
  public void set(String n, float v)
  {
    if (!connected) {
      return;
    }
    
    // set the value
    ControleaseNode node = outputsByName.get(n);
    node.setVal(v);
    
    // send the value over osc
    OscMessage outputMsg = new OscMessage("/oc");
    outputMsg.add(node.index);                        // index
    outputMsg.add(node.getFloat());                   // value
    controlease_oscP5.send(outputMsg, controleaseAddr);
  }
}




