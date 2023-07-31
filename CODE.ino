//*THIS Program created by PRADDHUMN SINGH for switching the three phase motor using gsm module*//
                                   //~JAI HIND~~JAI BHARAT~//
#include "EEPROM.h"
#include "SoftwareSerial.h"
#include "Talkie.h"
#include "Vocab_US_Large.h"
#include "Vocab_Special.h"
//*Replace +ZZ with Country Code and XXXXXXXXXX with Phone Number*//
String Phone_no1 = "+91XXXXXXXXXX";
String Phone_no2 = "+91XXXXXXXXXX";
String SMS_no    = "+91XXXXXXXXXX";                         

                         
Talkie voice;

const int8_t sp_HELLO[]        PROGMEM ={0x00,0xC0,0x80,0x60,0x59,0x08,0x10,0x3D,0xB7,0x00,0x62,0x64,0x3D,0x55,0x4A,0x9E,0x66,0xDA,0xF6,0x56,0xB7,0x3A,0x55,0x76,0xDA,0xED,0x92,0x75,0x57,0xA3,0x88,0xA8,0xAB,0x02,0xB2,0xF4,0xAC,0x67,0x23,0x73,0xC6,0x2F,0x0C,0xF3,0xED,0x62,0xD7,0xAD,0x13,0xA5,0x46,0x8C,0x57,0xD7,0x21,0x0C,0x22,0x4F,0x93,0x4B,0x27,0x37,0xF0,0x51,0x69,0x98,0x9D,0xD4,0xC8,0xFB,0xB8,0x98,0xB9,0x56,0x23,0x2F,0x93,0xAA,0xE2,0x46,0x8C,0x52,0x57,0x66,0x2B,0x8C,0x07};
const int8_t sp_CONNECTED[]    PROGMEM ={0x0E,0x88,0xC7,0x35,0x00,0x31,0x15,0x7B,0x20,0x24,0xB1,0x78,0x5A,0xD8,0x92,0x9B,0x9F,0xDC,0xE7,0x70,0x53,0x9F,0x93,0x72,0xEF,0x5D,0x1D,0x2D,0x6D,0x2F,0x4E,0x96,0x72,0x84,0x43,0xD5,0x39,0x69,0x6E,0x91,0x86,0xDD,0x66,0xA4,0x69,0x59,0x90,0x8F,0xEB,0x94,0x45,0xED,0xAA,0x62,0x4A,0x78,0x0D,0x00,0x0A,0x90,0x41,0xA3,0x65,0x25,0x9A,0xA3,0xEB,0xB6,0x59,0xEC,0xAA,0xB6,0xA2,0x17,0x26,0x2C,0xC3,0x33,0xC6,0xDE,0xF6,0xAA,0x57,0xD2,0x2D,0x3D,0x1D,0xBA,0xA6,0x44,0x15,0x16,0xC9,0xAC,0xFA,0x18,0x45,0x94,0xA2,0x26,0xC0,0x03,0x00,0x00};

# define SIM800L_Tx 7
# define SIM800L_Rx 6
SoftwareSerial SIM800L(SIM800L_Tx, SIM800L_Rx);

# define Motor_Start_PIN 9
# define Motor_Stop_PIN 8
# define Green_led A0
# define Red_led A2

# define MOTOR_STATE A1
# define CHECK_RYB_Pin  4
# define Blue_led   2

boolean Motor_Status = 0;
boolean POSITION_MOTOR = 0;
boolean CHECK_RYBState = 0;
boolean A=0;
boolean B=0; 
boolean C=0;
boolean call_flag = 0;
char dtmf_cmd;

String textMessage;

void setup()
{
  SIM800L.begin(9600);
  Serial.begin(9600);

  INIT_GSM();
 
  pinMode(Motor_Start_PIN, OUTPUT);
  pinMode(Motor_Stop_PIN, OUTPUT);
  pinMode(Blue_led, OUTPUT);
  pinMode(CHECK_RYB_Pin, INPUT);
  pinMode(Green_led, OUTPUT);
  pinMode(Red_led, OUTPUT);
  pinMode(MOTOR_STATE, INPUT);
        
  digitalWrite(Motor_Start_PIN,HIGH);
  MOTOR_OFF();
  
   delay(1500);
   String intrd="";
   intrd="[KRISHI is ONLINE]\r\n";
   intrd=intrd+"Hello\r\n";
   intrd=intrd+"This is KRISHI{'_'}";
   SendSMS(intrd);
   delay(2000); 
   String intrd1="";   
   intrd1="WELCOME TO GSM DTMF BASED AUTOMATIC MOTOR ON/START AND OFF/STOP DEVICE.\r\n"; 
   intrd1=intrd1+"      \r\n "; 
   intrd1=intrd1+ "[ Created By:-  Er. PRADDHUMN SINGH ]";
   SendSMS(intrd1); 
      
   delay(7000);
   SIM800L.print("ATD+ "); SIM800L.print(SMS_no); SIM800L.println(";"); 
   delay(15000);
   	voice.say(sp2_POWER); delay(100); 	voice.say(sp2_ON);
   delay(500); 
   voice.say(sp2_I); delay(100); voice.say(sp2_REPEAT);
   delay(500); 
   voice.say(sp2_POWER); delay(100); voice.say(sp2_ON);
   delay(500);               
   SIM800L.println("ATH");
   delay(2000);    
     
}

void loop()
{
 String gsm_data;
 int x = -1;
 // Store serial data from SIM800L
 while (SIM800L.available())
 {
  char c = SIM800L.read();
  gsm_data += c;
  delay(10);
 }
    
 if (call_flag==1)
 {        
  x = gsm_data.indexOf("NO CARRIER");
  if (x > -1)
  {
   // Terminate ongoing call when call is disconnected
   SIM800L.println("ATH");
   Serial.println("CALL_END1");
   C=0;  
   call_flag=0;          
  }
  call_flag=0;     
 } 
 else 
 {
  x = gsm_data.indexOf(Phone_no1);
  if (x > -1)
  {  
   delay(2000);
   SIM800L.println("ATA");
   Serial.println("CALL_ANS");
   delay(500);  

   INTRD();  
   Serial.println("1");                     
   VPN();
   VOICE();
   C=1; 
   call_flag=1;     
  }         
 }   
 if(call_flag==0)
 {
  x = gsm_data.indexOf(Phone_no2);    
  if (x > -1)   
  {      
   delay(2000);
   SIM800L.println("ATA");
   Serial.println("CALL_ANS");
   delay(500);  
            
   INTRD(); 
   Serial.println("2");                   
   VPN();
   VOICE();
   C=1;
   call_flag=1;                              
  }
 }
    
 if(C==1)
 {
 // Check if DTMF is receive from SIM800L
  x = gsm_data.indexOf("DTMF");
  if (x > -1)
  {
   dtmf_cmd = gsm_data[x + 6];
   Serial.println(dtmf_cmd);
   update_Motor(); 
  } 
  x = gsm_data.indexOf("NO CARRIER");
  if (x > -1)
  {
   // Terminate ongoing call when call is disconnected
   SIM800L.println("ATH");
   Serial.println("CALL_END");
   C=0;
   call_flag=0;  
  }
 }     
 loop_checkRYB();
        
}

void INIT_GSM()
{
 Serial.println("initializing GSM module");
 boolean GSM_COM = 1;
 while (GSM_COM > 0)
 {
  SIM800L.println("AT");
  while (SIM800L.available())
  {
   if (SIM800L.find("OK") > 0)
   {
    GSM_COM = 0;
   }
  }
  delay(1000);
 }
 
 boolean NTW = 1;
 while (NTW > 0)
 {
  SIM800L.println("AT+CPIN?");
  while (SIM800L.available())
  {
   if (SIM800L.find("+CPIN: READY") > 0)
   {
    NTW = 0;
   }
  }
  delay(1000);
 }
 
 boolean DTMF = 1;
 while (DTMF > 0)
 {
  SIM800L.println("AT+DDET=1");
  while (SIM800L.available())
  {
   if (SIM800L.find("OK") > 0)
   {
    DTMF= 0;
   }
  }
  delay(2000);
 }
 
 boolean CALL_ID = 1;
 while (CALL_ID > 0)
 { 
  SIM800L.println("AT+CLIP=1");
  while (SIM800L.available())
  {  
   if (SIM800L.find("OK") > 0);
   {
    CALL_ID = 0;           
   }         
  }
  delay(1000);
 }
    
 boolean CUT = 1;
 while(CUT > 0)
 {
  SIM800L.println("AT+CHUP");     
  while (SIM800L.available())
  {  
   if (SIM800L.find("OK") > 0);
   {
    CUT = 0;           
   }         
  }
  delay(1000);       
 } 
    
 boolean SMS_MODE = 1;
 while(SMS_MODE > 0)
 {
  SIM800L.println("AT+CMGF=1");
  while (SIM800L.available())
  {  
   if (SIM800L.find("OK") > 0);
   {
    SMS_MODE = 0;           
   }         
  }
  delay(1000);
 }
          
   Serial.println("initializing GSM module successful");
   delay(3000);
    
    
 }

void update_Motor()
{  

 if (dtmf_cmd == '1')
 {   
  MOTOR_ON();         
  voice.say(sp2_MOTOR); delay(100); voice.say(sp2_IS); delay(100); voice.say(sp2_ON); delay(100);
  delay(500);
  SendSMS("MOTOR IS ON(START)");
  delay(1500);
    
 }

 if (dtmf_cmd == '2')
 {
  MOTOR_OFF();
  Motor_Status=0;
  EEPROM.write(1,Motor_Status); 
  voice.say(sp2_MOTOR); delay(100); voice.say(sp2_IS); delay(100); voice.say(sp2_OFF); delay(100); 
  delay(500);  
  SendSMS("MOTOR IS OFF(STOP)");
  delay(1500);    
 }

 if (dtmf_cmd == '3')
 {
  CHECK_RYBState = digitalRead(CHECK_RYB_Pin);
  if (CHECK_RYBState == HIGH)
  {
   voice.say(sp2_ALL); delay(100); voice.say(sp2_THREE); delay(100); voice.say(sp2_POWER); delay(100); voice.say(sp_CONNECTED);                                                                     
   delay(500);
  }
   else                                                                                                           
  {                     
   voice.say(sp2_ALL); delay(100); voice.say(sp2_THREE); delay(100); voice.say(sp2_POWER); delay(100); voice.say(sp2_NOT); delay(100); voice.say(sp_CONNECTED);                                                                        
   delay(500);
  }
  
 }
 if (dtmf_cmd == '4')
 {      
  POSITION_MOTOR = digitalRead(MOTOR_STATE);
  if (POSITION_MOTOR == HIGH)
  {
   voice.say(sp2_POSITION); delay(100); voice.say(sp2_OFF); delay(100); voice.say(sp2_MOTOR); delay(100); voice.say(sp2_IS); delay(100); voice.say(sp2_ON); 
   delay(500);              
  }
  else
  {
   voice.say(sp2_POSITION); delay(100); voice.say(sp2_OFF); delay(100); voice.say(sp2_MOTOR); delay(100); voice.say(sp2_IS); delay(100); voice.say(sp2_OFF);
   delay(500);                  
  }  
 
  delay(500);             
 }     
 if (dtmf_cmd == '0')
 {
  VOICE();        
 }
}

    
void MOTOR_ON()
{
 digitalWrite(Motor_Start_PIN, LOW);
 delay(1500);   
 digitalWrite(Motor_Start_PIN, HIGH);
   
 digitalWrite(Green_led,1);
 digitalWrite(Red_led,0);
 Motor_Status=1;
 EEPROM.write(1,Motor_Status);    
}

void MOTOR_OFF()
{
 digitalWrite(Motor_Stop_PIN, LOW);  
 delay(1500);
 digitalWrite(Motor_Stop_PIN, HIGH);
 digitalWrite(Green_led,0);
 digitalWrite(Red_led,1);         
}

void INTRD()
{
 voice.say(sp_HELLO); delay(500); voice.say(sp2_G); delay(50); voice.say(sp2_S); delay(50);  voice.say(sp2_M); delay(700); voice.say(sp2_D); delay(50); voice.say(sp2_T); delay(50);  voice.say(sp2_M); delay(50);  voice.say(sp2_F); delay(700); 
 voice.say(sp4_BASE); delay(100); voice.say(sp2_AUTOMATIC); delay(100); voice.say(sp2_MOTOR); delay(100); voice.say(sp2_ON); delay(100); 		voice.say(sp2_AND); delay(100); voice.say(sp2_OFF); delay(100);	voice.say(sp2_DEVICE);
 delay(500); 
}
void VOICE()
{
 voice.say(sp2_PRESS); delay(100); voice.say(sp2_ONE); delay(100); voice.say(sp4_FOR); delay(100); voice.say(sp2_MOTOR); delay(100); voice.say(sp2_ON); 
delay(500);
 voice.say(sp2_PRESS); delay(100); voice.say(sp2_TWO); delay(100); voice.say(sp4_FOR); delay(100); voice.say(sp2_MOTOR); delay(100); voice.say(sp2_OFF); 
 delay(500);
 voice.say(sp2_PRESS); delay(100); voice.say(sp2_THREE); delay(100); voice.say(sp4_FOR); delay(100); voice.say(sp2_CHECK); delay(100); voice.say(sp2_POWER);
 delay(500);       
 voice.say(sp2_PRESS); delay(100); voice.say(sp2_FOUR); delay(100); voice.say(sp4_FOR); delay(100); voice.say(sp2_CHECK); delay(100); voice.say(sp2_POSITION); delay(100); voice.say(sp2_OFF); delay(100); voice.say(sp2_MOTOR); delay(100);
 delay(500);                    
 voice.say(sp2_PRESS); delay(100); voice.say(sp2_ZERO); delay(100); voice.say(sp4_FOR); delay(100); voice.say(sp2_REPEAT);
 delay(500);            
             
}
        
void PH()
{
  String PH="";               
  PH="PHASE ARE CONNECTED.\r\n";
  PH=PH+"[POWER SUPPLY OK]";
  SendSMS(PH);              
  delay(3000);
}

void PHM()
{
  String PHM="";
  PHM="PHASE ARE CONNECTED.\r\n"; 
  PHM=PHM+"[POWER SUPPLY OK]\r\n";
  PHM=PHM+"  \r\n";
  PHM=PHM+"{MOTOR IS ON(START)}";
  SendSMS(PHM);
  delay(7000);               
                
  SIM800L.print("ATD+ "); SIM800L.print(SMS_no); SIM800L.println(";");               
  delay(15000);               
  voice.say(sp2_MOTOR); delay(100); voice.say(sp2_IS); delay(100); voice.say(sp2_ON); delay(100);  
  voice.say(sp2_I); delay(100); voice.say(sp2_REPEAT);
  delay(500); 
  voice.say(sp2_MOTOR); delay(100); voice.say(sp2_IS); delay(100); voice.say(sp2_ON);
  delay(500);

 SIM800L.println("ATH");              
 delay(500);
    
}

void PHN()
{
 String PHN="";
 PHN="PHASE ARE NOT CONNECTED.\r\n"; 
 PHN=PHN+"[CHECK YOUR POWER SUPPLY]";
 SendSMS(PHN);        
 delay(7000);
 SIM800L.print("ATD + "); SIM800L.print(SMS_no); SIM800L.println(";");
 delay(15000);
 voice.say(sp2_ALL); delay(100); voice.say(sp2_THREE); delay(100); voice.say(sp2_POWER); delay(100); voice.say(sp2_NOT); delay(100); voice.say(sp_CONNECTED);                                                                        
 delay(500);
 voice.say(sp2_I); delay(100); voice.say(sp2_REPEAT);
 delay(500); 
 voice.say(sp2_ALL); delay(100); voice.say(sp2_THREE); delay(100); voice.say(sp2_POWER); delay(100); voice.say(sp2_NOT); delay(100); voice.say(sp_CONNECTED);                                                                        
 delay(500);
 SIM800L.println("ATH"); 
 delay(500);     
    
}
void VPN()
{
 if (CHECK_RYBState ==  LOW)
 { 
  voice.say(sp2_ALL);  delay(100);  voice.say(sp2_THREE);  delay(100);  voice.say(sp2_POWER);  delay(100);   voice.say(sp2_NOT);  delay(100);  voice.say(sp_CONNECTED);  delay(100);
  voice.say(sp2_I); delay(100); voice.say(sp2_REPEAT);
  delay(500);   
  voice.say(sp2_ALL);  delay(100);  voice.say(sp2_THREE);  delay(100);  voice.say(sp2_POWER);  delay(100);   voice.say(sp2_NOT);  delay(100);  voice.say(sp_CONNECTED);  delay(100);
  delay(500);                       
  SIM800L.println("ATH");               
   
 }   
}

void SendSMS(String SMS)
{
  //Serial.println ("Sendinga Message");
SIM800L.println("AT+CMGF=1");      //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  SIM800L.println("AT+CMGS=\"" + SMS_no + "\"\r\n"); //Mobile phone number to send message
  delay(1000);
  SIM800L.println(SMS);
  delay(100);
  SIM800L.println((char)26);// ASCII code of CTRL+Z
  delay(1000);

}

void loop_checkRYB()
{
 CHECK_RYBState = digitalRead(CHECK_RYB_Pin);
 if (CHECK_RYBState == HIGH)
 {
  if(A==0)
  {
   digitalWrite(Blue_led, 1); 
   SIM800L.println("ATH");
   delay(500);           
   Motor_Status = EEPROM.read(1);
   if (Motor_Status == HIGH)
   {
    MOTOR_ON();       
    PHM();             
   }
   else
   {
    PH();                                                                                                                            
   }
   SIM800L.println("AT+DDET=1");
   while (SIM800L.available())
   {
    if (SIM800L.find("OK") > 0);
   }
   delay(1000);
   A++;
   B=0;
  }          
 }
 if (CHECK_RYBState ==  LOW)
 {
  if(B==0)
  {
   digitalWrite(Blue_led, 0);
   SIM800L.println("ATH");    
   delay(500);         
   MOTOR_OFF();
   SIM800L.println("AT+DDET=0");
   while (SIM800L.available())
   {
    if (SIM800L.find("OK") > 0);    
   }
   delay(1000);
   PHN();                 
   B++;        
   A=0;
  }
 }
}