/** Processing sketch */
 
import processing.serial.*;
import processing.sound.*;
 
Serial xbee;
SoundFile file;
char lastChar;
char currChar;
int interval = 10000;
int last = 0;

void setup() {
  println(Serial.list());
  xbee = new Serial(this, Serial.list()[6], 9600);
  xbee.write("+++");
  delay(1100);
  xbee.write("ATID 3305, CH C, CN");
  delay(1100);
  xbee.bufferUntil(10);
  file = new SoundFile(this, "surprise.mp3");
}
 
void draw() {
  // you could map the incoming data to a graphical or 
  // text display
}
 
void serialEvent(Serial p) {
  
  try{
    
    if(last != 0 && (millis() - last) < interval){
      throw new Exception("");
    }
    currChar = p.readChar();
    if(currChar == '-' ){  
      file.stop();
      file.play();
      delay((int)(file.duration()*1000));
      file.stop();
      last = millis();
    }
    lastChar = currChar;
  }catch(Exception e){
  
  }
  print(p.readString());
  //print(p.readChar() == '!');
 
}

void keyPressed() {
  println("I pressed " + key);
  xbee.write( key );
}
