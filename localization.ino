#include <SoftwareSerial.h>

SoftwareSerial sim(2, 3);

#include "Wire.h"
struct sms{
  String msg1 = "0";
  String msg2 = "0";
};
struct sms tmp;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  sim.begin(9600);
  sim.setTimeout(250);
  sim.println("AT+CLIP=1");
  delay(1000);
  sim.println("AT+IPR=9600");
  delay(1000);
}

void loop() {
struct sms message = GetMessage();
if(message.msg1!="0"){
  Serial.println("-----------------Message available------------");
  Serial.print("the message----->");
  Serial.print(message.msg1);
  Serial.println(message.msg2);
  tmp = message;
}
 bool value = true;
  String nbr = getNumber();
  Serial.println("the number is "+nbr);
  while(nbr!="0" and value){
    Serial.println("the Message is "+ tmp.msg2);
    if(tmp.msg1!="0")  {
    sendSMS(tmp,nbr);
    /*Serial.println("******************MESSAGE***************");
    Serial.print(tmp.msg1);
    Serial.print(tmp.msg2);
    Serial.println("******************MESSAGE***************");*/
    value = false;
    Serial.println("Message Sent");
    }
    else{
      Serial.println("Nothing");
      value = false;
    }
  }
}




struct sms GetMessage(){
    struct sms Message;
    String ReadString;
    ReadString=Serial.readStringUntil(13);   
  ReadString.trim();                             
  if (ReadString.indexOf("$GPRMC")>=0) { 
      int Pos=ReadString.indexOf("$GPRMC");
      ReadString.remove(0, Pos); 
      Serial.println(ReadString);    
      if(ReadString.indexOf("V")<0){ 
      Pos=ReadString.indexOf(',');
      ReadString.remove(0, Pos+1); 
      Pos=ReadString.indexOf(',');
      char TimeStamp[Pos];          //declare different variable name
      for (int i=0; i <= Pos-1; i++){
            TimeStamp[i]=ReadString.charAt(i);         //fill the array     
      }
      String Time = getTime(atof (TimeStamp));
       
      ReadString.remove(0, Pos+1);
      Pos=ReadString.indexOf(','); 
      ReadString.remove(0, Pos+1);
      Pos=ReadString.indexOf(','); 
      char Lat[Pos];            
      for (int i=0; i <= Pos-1; i++){   
            Lat[i]=ReadString.charAt(i);           
      }          
       //Get Lattitude North or South
      ReadString.remove(0, Pos+1);               
      Pos=ReadString.indexOf(',');    
      char LatSide[Pos];           
      for (int i=0; i <= Pos-1; i++){
            LatSide[i]=ReadString.charAt(i);   
      }
      float LatAsFloat = atof (Lat);             
      String LatString= ConvertData(LatAsFloat, LatSide[0]);    
             
       //Get Longitude
      ReadString.remove(0, Pos+1);               
      Pos=ReadString.indexOf(',');    
      char Longit[Pos];            
      for (int i=0; i <= Pos-1; i++){
            Longit[i]=ReadString.charAt(i);     
      }
      ReadString.remove(0, Pos+1);              
      Pos=ReadString.indexOf(',');    
      char LongitSide[Pos];        
      for (int i=0; i <= Pos-1; i++){
            LongitSide[i]=ReadString.charAt(i);    
      }     
      float LongitAsFloat = atof (Longit);    //atof converts the char array to a float type
      String LongString=ConvertData(LongitAsFloat, LongitSide[0]);  
      ReadString.remove(0, Pos+1);                
      Pos=ReadString.indexOf(','); 
      if(LatString!=0){
      String link = "https://www.google.com/maps/place/" + LatString + "+" + LongString;
      Message.msg1 = "At " + Time + " the device was at the location below: \n";
      Message.msg2 = link;
       Serial.println("////////////////////////////////////////");
       Serial.println(Message.msg1);
       Serial.println(Message.msg2);
       return Message;
      }
      else{
        Message.msg1 = "0";
        Message.msg2 = "0";
        Serial.println("No location found");
        return Message;
      }
    }
    else{
      Message.msg1 = "0";
      Message.msg2 = "0";
      Serial.println("No signal found");
      return Message;  
    }
  }
  else{
     Message.msg1 = "0";
     Message.msg2 = "0";
     Serial.println("No data found");
     return Message;
  }
  
}

String getTime(float TimeStamp){
  int Hours = int(TimeStamp/10000);
  int Seconds = int(TimeStamp - (int)(TimeStamp/100)*100);
  int Minutes = int(int(TimeStamp/100)-Hours*100);
  char TimeString[8];
  sprintf(TimeString,"%02d:%02d:%02d", Hours, Minutes, Seconds);
  return TimeString;
}



//Conversion function
String ConvertData(float RawDegrees, char pol)
{ 
  float RawAsFloat = RawDegrees; 
  int degree = ((int)RawAsFloat)/100; 
  float minutes = int(RawAsFloat - (float)(degree*100));
  float Seconds = ((RawAsFloat - (float)(degree*100)) - minutes) * 60; 
  String Converted = String(degree) + "%C2%B0" + String(int(minutes)) + "'" + String(Seconds) + "%22" + pol;
  return Converted;
}





String getNumber(){
  sim.readString();
  int t = 0;
  long nbrs[8];
  int target;
  String test = sim.readString();
  Serial.println(test);
  if(test.indexOf("CLIP")>=0 or test.indexOf("IP")>=0){
  while(t<8){
  if(test.indexOf("CLIP")>=0 or test.indexOf("IP")>=0){
  nbrs[t] = test.substring(test.indexOf("\"0")+1,test.indexOf("\"0")+11).toInt();
    Serial.println(t);
    t++;
  }
  test = sim.readString();
  }
  sim.println("ATH");
  for(int i=0;i<7;i++){
    int cnt = 0;
    for (int j=i+1;j<8;j++){
      if(nbrs[i] == nbrs[j]){
        cnt++;
      }
    }
    if(cnt>2){
      target = i;
      break;
    }
  }
 
  return String(nbrs[target]);}
  else{
    return String(0);
  }
  
}


void sendSMS(struct sms Message, String number){ 
     sim.println("AT"); //   set SMS mode to text
    // Serial.println("set SMS mode to txt"); 
     delay(100);
     Serial.println(sim.readString());
     sim.println("AT+CMGF=1"); //   set SMS mode to text
    // Serial.println("set SMS mode to txt"); 
     delay(1500);
     Serial.println(sim.readString());
      sim.println("AT+CSCS=\"GSM\"");  // set SMS mode to text
     delay(1500);
     Serial.println(sim.readString());
     //Serial.println("blurt out contents of new SMS upon receipt to the GSM shield's serial out"); 
      sim.println("AT+CNMI=2,2,0,0,0"); 
     delay(1500);
     Serial.println(sim.readString());
 
      sim.print("AT+CMGS=\"+213"+number+"\""); 
     delay(500);
     Serial.println(sim.readString());
      sim.write(0x0d); // send carriage return  \r
     delay(500);
      sim.print(Message.msg1);
      sim.print(Message.msg2);
     delay(1000);
      sim.write(0x1a); //  send Ctrl-Z
}
