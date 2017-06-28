#define piezo A0
#define photoRes A1
#define pot A2

#define sync1 3
#define sync2 2
#define led1 6
#define led2 7
#define flash 8
#define trigger 9
#define focus 10
#define button1 11
#define switch1 12

int mode = 0;
int prevMode = 0;
boolean untriggered = true;


void setup() {
  // put your setup code here, to run once:
  pinMode(trigger,OUTPUT);
  pinMode(focus,OUTPUT);
  pinMode(flash,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(button1,INPUT);
  pinMode(switch1,INPUT);
  pinMode(piezo,INPUT);
  pinMode(photoRes,INPUT);
  pinMode(pot,INPUT);
  Serial.begin(9600);
  digitalWrite(led1,HIGH);
  digitalWrite(led2,HIGH);
  delay(500);
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
}

void loop() {
  if(Serial.available()>0){
    readSerial();
  }

  switch(mode){
    case 0:
      mode0();
      break;
    case 1:
      mode1();
      break;
    case 2:
      mode2();
      break;
    case 3:
      mode3();
      break;
    case 4:
      mode4();
      break;
    case 5:
      mode5();
      break;
    case 6:
      mode6();
      break;
    case 7:
      mode7();
      break;
    default:
      mode0();
      break;
  }
}

void mode0(){
  digitalWrite(led1,HIGH);
  delay(300);
  digitalWrite(led1,LOW);
  delay(300);
  if (prevMode != 0){
    prevMode = 0;
  }
}

void mode1(){
  armCamera();
  mode = prevMode;
}

void mode2(){
  disarmCamera();
  mode = prevMode;
}

void mode3(){
  triggerCamera();
  mode = 0;
  Serial.println("mode 0");
}

void mode4(){
  if(digitalRead(button1)&&untriggered){
    triggerCameraAndFlashWithDelay();
    untriggered = false;
  } else if(!digitalRead(button1)){
    untriggered = true;
  }

  if (prevMode != 4){
    prevMode = 4;
  }
}

void mode5(){
  openShutterTillPiezo();
  //Serial.println(analogRead(piezo));
  if (prevMode != 5){
    prevMode = 5;
  }
}

void mode6(){
  
}

void mode7(){
  
}

void readSerial(){
  mode = Serial.readString().toInt();
  Serial.println((String) "mode: " + mode);
  
}

boolean isAboveLight(){
  return analogRead(photoRes)>analogRead(pot);
}

boolean isBelowLight(){
  return analogRead(photoRes)<analogRead(pot);
}

boolean isLight(){
  return analogRead(photoRes)>500;
}

boolean isDark(){
  return analogRead(photoRes)<200;
}

void armCamera(){
  if (!digitalRead(focus)){
    digitalWrite(focus,HIGH);
    digitalWrite(led2,HIGH);
    delay(100);
    Serial.println("Camera armed");
  }
}

void disarmCamera(){
  digitalWrite(focus,LOW);
  digitalWrite(led2,LOW);
  Serial.println("Camera disarmed");
}

void triggerCamera(){
  digitalWrite(trigger,HIGH);
  delay(10);
  digitalWrite(trigger,LOW);
}

void triggerCameraAndFlash(){
  digitalWrite(trigger,HIGH);
  delay(90);
  digitalWrite(flash,HIGH);
  delay(10);
  digitalWrite(trigger,LOW);
  digitalWrite(flash,LOW);
  Serial.println(analogRead(pot));
}

void triggerCameraAndFlashWithDelay(){
  digitalWrite(trigger,HIGH);
  delay(analogRead(pot));
  digitalWrite(flash,HIGH);
  delay(10);
  digitalWrite(trigger,LOW);
  digitalWrite(flash,LOW);
  Serial.println(analogRead(pot));
}


void openShutterTillPiezo(){
  if(digitalRead(button1)){
    digitalWrite(trigger,HIGH);
  }
  if (analogRead(piezo)<1010){
    delay(analogRead(pot)/10);
    digitalWrite(flash,HIGH);
    delay(10);
    digitalWrite(flash,LOW);
    digitalWrite(trigger,LOW);
    digitalWrite(focus,LOW);
    delay(20);
  }
}

