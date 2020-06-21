const int LEYE = A4;
const int REYE = A3;
const int LED_PIN = 13;
const int TPIN = 9;
const int EPIN = 8;
const int IR_WHEEL_SENSOR_PIN = 2;

unsigned long us_last_poll = 0;
unsigned long ir_last_poll = 0;

bool last_saw_left;
bool last_saw_right;

int move_right_analog = 130;
int move_left_analog = 120;
int thresholdTime = 10;
long duration;
int distance;
int changes = 0;
int revolutions = 0;

char theChar = 's';

// more sensive C less sensitive AC
// 1.5-2mms
const unsigned long us_polling_time = 500;
const unsigned long ir_polling_time = 10;

const int stopDistance = 25;

const int left_neg = 4;
const int left_pos = 5;

const int right_pos = 6;
const int right_neg = 7;

const int interval = 1000;

void move_forward();
void move_reverse();

bool mssg = false;

void set_right_positive_high();
void set_left_positive_high();
void set_left_negative_high();
void set_right_negative_high();

void set_right_positive_low();
void set_left_positive_low();
void set_left_negative_low();
void set_right_negative_low();

void move_reverse_left();
void move_reverse_right();
void move_forward_left();
void move_forward_right();
void move_forward_slowly();
void move_forward_right_analog();
void move_forward_left_analog();

void move_stop();

int get_distance();

void trigger();
void send_distance_travelled();

void setup() {
 Serial.begin(9600);
 pinMode( LEYE, INPUT );
 pinMode( REYE, INPUT );
 pinMode( TPIN, OUTPUT ); 
 pinMode( EPIN, INPUT ); 
 pinMode( LED_PIN, OUTPUT);
 pinMode(IR_WHEEL_SENSOR_PIN, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(IR_WHEEL_SENSOR_PIN), trigger, CHANGE);
 delay(1000);
 Serial.print("+++");
 delay(1000);
 Serial.println("ATID  3305, CH  C,  CN");
 delay(1000);
 while(  Serial.read() !=  -1  );
}
void loop(){

 if( Serial.available()  > 0 ){  
  theChar = Serial.read();  
  Serial.print(" Okay boss: "); 
  Serial.println( theChar );

  if(theChar == 's'){
    Serial.println(" Stopping"); 
   }else{
    Serial.println(" Going"); 
   }

 }
 
 if( theChar == 's'){
   move_stop(); 
   return;
 }
 
 unsigned long us_timeDifference = (millis() - us_last_poll);
 unsigned long ir_timeDifference = (millis() - ir_last_poll);
 
 if(us_last_poll == 0 ||  us_timeDifference > us_polling_time ){
   if( get_distance() < stopDistance){

    if( mssg == false){
      Serial.print("-Object was detected ");
      Serial.print((get_distance()/100));
      Serial.println("m away. Stop");
      mssg = true;
    }
    move_stop();
    return;
   }else if(mssg == true){
      mssg = false;
   }
  send_distance_travelled();
 }

 if(ir_last_poll == 0 ||  ir_timeDifference > ir_polling_time ){

   bool all_clear_left = digitalRead( LEYE );
   bool all_clear_right = digitalRead( REYE );

   if(all_clear_right == false && all_clear_left == false){
//    move_right_analog = 180;
//    move_left_analog = 180;
    move_forward_slowly();
    last_saw_left = all_clear_left;
    last_saw_right = all_clear_right;
  
   }
   
   if(all_clear_right == true && all_clear_left == true){
      if(last_saw_right == false && last_saw_left == false){
//        move_right_analog = 60;
//        move_left_analog = 60;
        move_forward_slowly();
//        set_left_positive_low();
//          move_stop();
      }else if(last_saw_right == true){
//        move_right_analog = 60;
        move_forward_slowly();
        set_left_positive_low();
      }else if(last_saw_left == true){
//        move_left_analog = 60;
        move_forward_slowly();
        set_right_positive_low();
      }
      
   }
   
   if(all_clear_right == true && all_clear_left == false){
//      move_right_analog = 60;
      move_forward_slowly();
      set_left_positive_low();
      last_saw_left = all_clear_left;
      last_saw_right = all_clear_right;
   }
   
   if(all_clear_left == true && all_clear_right == false){
//      move_left_analog = 60;
      move_forward_slowly();
      set_right_positive_low();
      last_saw_left = all_clear_left;
      last_saw_right = all_clear_right;
   }
   
 }
 
}

void move_forward(){

  set_right_negative_low();
  set_left_negative_low();
  set_right_positive_high();
  set_left_positive_high();
}
void move_reverse(){
  set_right_positive_low();
  set_left_positive_low();
  set_right_negative_high();
  set_left_negative_high();

}
void move_stop(){
//  Serial.println("Attempting to stop");
  set_right_positive_low();
  set_left_positive_low();
  set_right_negative_low();
  set_left_negative_low();
}

void move_forward_right(){
  set_left_positive_low();
  set_left_negative_low();
  set_right_negative_low();
  set_right_positive_high();
  
}

void move_forward_right_analog(){
  analogWrite(right_pos, move_right_analog);
}
void move_forward_left_analog(){
  analogWrite(left_pos, move_left_analog);
}

void move_forward_slowly(){
  move_forward_right_analog();
  move_forward_left_analog();  
}

void move_reverse_right(){
  set_left_positive_low();
  set_left_negative_low();
  set_right_positive_low();
  set_right_negative_high();
}
void move_forward_left(){
  set_right_negative_low();
  set_left_negative_low();
  set_right_positive_low();
  set_left_positive_high();
}
void move_reverse_left(){
  set_right_negative_low();
  set_left_positive_low();
  set_right_positive_low();
  set_left_negative_high();
}
void set_right_positive_high(){
  digitalWrite(right_pos, HIGH);
}
void set_left_positive_high(){
  digitalWrite(left_pos, HIGH);
}
void set_left_negative_high(){
  digitalWrite(left_neg, HIGH);
}
void set_right_negative_high(){
  digitalWrite(right_neg, HIGH);
}

void set_right_positive_low(){
  digitalWrite(right_pos, LOW);
}
void set_left_positive_low(){
  digitalWrite(left_pos, LOW);
}
void set_left_negative_low(){
  digitalWrite(left_neg, LOW);
}
void set_right_negative_low(){
  digitalWrite(right_neg, LOW);
}

int get_distance(){

  digitalWrite(TPIN, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for thresholdTime micro seconds
  digitalWrite(TPIN, HIGH);
  delayMicroseconds(thresholdTime);
  digitalWrite(TPIN, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(EPIN, HIGH);
  
  // Calculating the distance
  distance= (duration*0.034)/2;
  
//  Serial.print("Distance: ");
//  Serial.println(distance);

  return distance;
}

void send_distance_travelled(){
  
  Serial.print(" Distance Travelled: ");
  Serial.print(revolutions * 0.0635*3.14);
  Serial.println("m");
  
}

void trigger(){
  changes++;

  if(changes == 10){
    revolutions++;
    changes = 0; 
  }
}
