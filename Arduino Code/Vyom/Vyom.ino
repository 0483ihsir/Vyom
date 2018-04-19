/********************************************************************
 * Name:     Vyom v1.0                                              *
 * Activity: Sensor-FanWiFi,Bluetooth,LCD Display                   *
 * Auther:   Rishi Mishra                                           *
 ********************************************************************
 */

 
//*********************LIBRARIES***********************************************

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#define DEBUG true

// initialize the LCD library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

const byte rxPin = 2; // Wire this to Tx Pin of ESP8266
const byte txPin = 3; // Wire this to Rx Pin of ESP8266

// We'll use a software serial interface to connect to ESP8266
SoftwareSerial ESP8266 (rxPin, txPin);
//SoftwareSerial Serial (2,3);

//******************************************************* VARIABLES ********************
int data=0;               //initial data set to be zero

int RELAY_SIGNAL=10;
int PM_SENSOR_PIN=A0;    // PIN no of Arduino Board that fetch data from sensor 
int PM_SENSOR_DATA=100;   // intial value of sensor
int PM_SENSOR_LED=9;
float PM_SENSTIVTY=0.004911;
float PM_VOLTAGE=0;
float PM_DENSITY=0;
float PM_LEVEL=0;
int delayTime=280;      // As per data sheet of GP2Y1010AU0F PM2.5 Sensor
int delayTime2=40;      // As per data sheet of GP2Y1010AU0F PM2.5 Sensor
float offTime=9680;     // As per data sheet of GP2Y1010AU0F PM2.5 Sensor



String sdata="";
String response="";
boolean sensor_status=true;
boolean fan_status=false;

int bmode=0;
boolean wific=true;


//**************************************MODES*************************************************************
  String s1="FONSOFF";
  String s2="FOFFSOFF";
  String s3="FONSON";
  String s4="FOFFSON";
  String mode="";
  String mode1="0";
  
//********URLs***********************************************************************************************


String server1="api.thingspeak.com";  // len=23
String server="kapot.000webhostapp.com";  // len=23
String mode_url="/apps/thinghttp/send_request?api_key=TYL4ZCX4D4NJ8M1M";             // len=14
String datasend_url="/vyom/receiver.php";     // len=18
String api_url="/vyom/api.php";           // len=13

//*****************************Functions*********************************************************************

int Linear(int AQIhigh, float AQIlow, float Conchigh,float Conclow,float Concentration)
{
int linear;
float Conc=(float)(Concentration);
float a;
a=((Conc-Conclow)/(Conchigh-Conclow))*(AQIhigh-AQIlow)+AQIlow;
linear=round(a);
return linear;
}

int AQIPM25(float Concentration)
{
float Conc=(float)(Concentration);
float c;
float AQI;
c=(floor(10*Conc))/10;
if (c>=0 && c<12.1)
{
  AQI=Linear(50,0,12,0,c);
}
else if (c>=12.1 && c<35.5)
{
  AQI=Linear(100,51,35.4,12.1,c);
}
else if (c>=35.5 && c<55.5)
{
  AQI=Linear(150,101,55.4,35.5,c);
}
else if (c>=55.5 && c<150.5)
{
  AQI=Linear(200,151,150.4,55.5,c);
}
else if (c>=150.5 && c<250.5)
{
  AQI=Linear(300,201,250.4,150.5,c);
}
else if (c>=250.5 && c<350.5)
{
  AQI=Linear(400,301,350.4,250.5,c);
}
else if (c>=350.5 && c<500.5)
{
  AQI=Linear(500,401,500.4,350.5,c);
}
else if (c>=500.5 && c<1000)
{
  AQI=Linear(600,501,650.4,450.5,c);
}
else
{
  AQI=773;
}
return AQI;
}



String AQICategory(int AQI)
{
String AQICategory="";
if (AQI<=50)
{
  AQICategory="Good";
}
else if (AQI>50 && AQI<=100)
{
  AQICategory="Moderate";
}
else if (AQI>100 && AQI<=150)
{
  AQICategory="Unhealthy for Sensitive Groups";
}
else if (AQI>150 && AQI<=200)
{
  AQICategory="Unhealthy";
}
else if (AQI>200 && AQI<=300)
{
  AQICategory="Very Unhealthy";
}
else if (AQI>300 && AQI<=400)
{
  AQICategory="Hazardous";
}
else if (AQI>400 && AQI<=500)
{
  AQICategory="Hazardous";
}
else
{
  AQICategory="Out of Range";
}
return AQICategory;
}

//**************************************** FUNCTION FOR DESPLAY *****************************

void Display(String str1,String str2)
{ 
    lcd.clear();
    lcd.print(str1);
    lcd.setCursor(0, 1);
    lcd.print(str2);
}

//************************************DATA FROM PM SENSOR ********************************************************

void getdata()
{
  pinMode(PM_SENSOR_LED,OUTPUT);
  
  digitalWrite(PM_SENSOR_LED,LOW);                              //power on the LED  
  delayMicroseconds(delayTime);
  PM_SENSOR_DATA=analogRead(PM_SENSOR_PIN);                     //readthePM_SENSOR_DATAueviapin5onthesensor
  delayMicroseconds(delayTime2);
  digitalWrite(PM_SENSOR_LED,HIGH);                             //turn the LED off
  delayMicroseconds(offTime);
  delay(3000);
  Serial.println(PM_SENSOR_DATA);
  PM_VOLTAGE=PM_SENSTIVTY * PM_SENSOR_DATA; 
  PM_DENSITY=0.172 * PM_VOLTAGE - 0.100;



data=1000*PM_DENSITY;
data+=random(0,3);
data=AQIPM25(data);

Serial.println(PM_DENSITY);
Serial.println(data);

}



//************************************************** SENDING TO SERVER **************************

String Views()
{
  char inv = '"';
  String add = "AT+CIPSTART";
  add += "=";
  add += inv;
  add += "TCP";
  add += inv;
  add += ",";
  add += inv;
  add += "api.thingspeak.com";  // Host_server_name
  add += inv;
  add += ",";
  add += "80";
  add += "\r\n";
  sendData(add, 3000, 0);  // Establishing a TCP Connection. ( AT+CIPSTART = "TCP","Host_server_name",80 )

  String rest = "AT+CIPSEND=90";
  rest += "\r\n";
  sendData(rest, 2000, 0); //Number of characters to send. ( "AT+CIPSEND=90" )


  String hostt = "GET /apps/thinghttp/send_request?api_key=TYL4ZCX4D4NJ8M1M";
  hostt += "\r\n";
  hostt += "Host:api.thingspeak.com";
  hostt += "\r\n\r\n\r\n\r\n\r\n";
  String views = sendData(hostt, 2000, 0);  // GET request ( GET /apps/thinghttp/send_request?api_key=XXXXXXXXXXXXXXXX 
                                            //               Host: Host_server_name ) 
  
  
/********************** Steps To filter the received data. ****************************/
 
  int i = 0;        
  if (DEBUG)
    Serial.println(views);
long int timeout=millis()+ 1000;
  while (views.charAt(i) != ':') // Character before required data starts.
    {i++;
    if(timeout<millis()) break;
    }
    
  i++;
  views = views.substring(i);
  
  int j = 0;
  timeout=millis()+ 1000;
  while (views.charAt(j) != 'C') // Character after required data ends.
    {
      j++;
      if(timeout<millis()) break;
    }
    
  views = views.substring(0, j);
  

    Serial.println(views);
    
  
  return (views);
}



String sendData(String command, const int Goldout, boolean debug)
{
  String response = "";

  ESP8266.print(command); // send the read character to the ESP8266

  long int Gold = millis();

  while ( (Gold + Goldout) > millis())
  {
    while (ESP8266.available())
    {

      // The esp has data so display its output to the serial window
      char c = ESP8266.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    Serial.print(response);
  }

  return response;
}

//*********************************************MODES COFIGURATION ******************

void configure()
{
    digitalWrite(RELAY_SIGNAL,fan_status);
    
    if(sensor_status)getdata();
      
  }

*********************************************TO RESET THE WIFI MODULE****************

void Reset()
{
  Display("Enter SSID","MAX 16 char");
//below is for one bit communication....
/*  String a="",c="";
int b=0;b=Serial.read();
  while(b!=3)
  { b=Serial.read();
    if(b>48&&b<58)
    {
      a+=b;
    }
  }
    delay(100);
   
    Display("Enter pwd for:",a);
    b=0;
  while(b!=3)
  { b=Serial.read();
    if(b>48&&b<58)
    {
      c+=b;
    }
  }
  */

  String a="",c="";
  a=Serial.read();
  c=Serial.read();
  bmode=Serial.read();
 //Serial.println(a);
 //Serial.println(c);
  Display("Reseting","...");
  sendData("AT+RST\r\n", 2000, DEBUG);
  sendData("AT+CWQAP\r\n", 5000, DEBUG); // Disconnect WiFi if connected.

  sendData("AT+CWMODE=3\r\n", 1000, DEBUG); // configure as  both access point and station mode.

char inv = '"';
  delay(3000);
  
  sendData("AT+CWQAP\r\n", 4000, DEBUG); // again quit the connection if any.
  
  
  String conn = "AT+CWJAP";
  conn += "=";
  conn += inv;
  conn += a;  // Host name
  conn += inv;
  conn += ",";
  conn += inv;
  conn += c; // Password
  conn += inv;
  conn += "\r\n\r\n";
  sendData(conn, 7000, DEBUG); // Join the connection.( AT+CWJAP = "Host_name","password" )
  Display("Done!!!","Have a Nice day");
  
}



//%%%%%%%%%%%%%%%%%%%%%%%%%%%%  SETUP  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void setup() {
  
 if(data==0){ 
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
 
 } 
 
  Serial.begin(9600);
  ESP8266.begin(115200); // Change this to the baudrate used by ESP8266
  delay(6000); // Let the module self-initialize
  getdata();
  Display("Vyom Purifier","PM2.5: "+(String)data);
  ESP8266.println("AT");
  delay(1000);
  ESP8266.println("AT+CWMODE=3");
  delay(1000);
  if(data<100)String s="AT+CIPSEND=74";
  else String s="AT+CIPSEND=75";
  ESP8266.println("AT+CIPSTART=\"TCP\",\"kapot.000webhostapp.com\",80");
  delay(100);
  ESP8266.println("AT+CIPSEND=76");
   delay(100);
   String d=String(data);
  String ds="GET /vyom/receiver.php?data="+d+" HTTP/1.0";
  ESP8266.println(ds);
  ESP8266.println("Host: kapot.000webhostapp.com");
  ESP8266.println("");ESP8266.println("");
  delay(2000);
   

  
  
}

void loop() {
 
    int z=0;

    z=Serial.read();
    if(z>48&&z<55){bmode=z;Serial.print("bmode");Serial.print(bmode);}
  
 
 
  if(bmode==87)Reset();
  else if (bmode==86)wific=true;
  
  if(bmode!=0)wific=false;
//************************************************CONDITION FOR DIFFERENT MODES******************
  if(wific){
    
        mode=Views();
        if(mode=="1"||mode=="2"||mode=="3"||mode=="4")mode1=mode;
        else mode1=mode1;
          }
  
  
  
    if(mode1=="1"||bmode==90){
               sensor_status=false;
              fan_status=true;
              configure();
               Display("Vyom Purifier","MODE: FONSOFF");
              delay(1000);
              Display("Sensor is OFF" ,"Filter is ON");
             
    }
    else if(mode1=="2"||bmode==91){
               sensor_status=false;
               fan_status=false;
              configure();
              Display("Vyom Purifier","MODE: FOFFSOFF");
              delay(1000);
              Display("Sensor is OFF","Filter is OFF");
                
    }

    else if(mode1=="3"||bmode==92){
                sensor_status=true;
                fan_status=true;
              configure();
              sdata=datasend_url;
              sdata+="?data=";
              sdata+=(String)data;
             // sendd();//response=send_to_server(server,sdata,(data<100)?49:50);
              Display("MODE: FONSON","filter is ON");
             
                      
    }
              
    else if(mode1=="4"||bmode==93){
              sensor_status=true;
              fan_status=false;
              configure();
              sdata=datasend_url;
              sdata+="?data=";
              sdata+=(String)data;
             // sendd();//response=send_to_server(server,sdata,(data<100)?49:50);
              Display("Vyom Purifier","PM2.5: "+(String)data);
              delay(1000);
              Display("MODE: FOFFSON","filter is OFF");
              
    }
    else { //getdata();
              Display("MODE: SENSING","filter is ON");
              delay(1000);
              Display("Vyom Purifier","PM2.5: "+(String)data);
              if(PM_DENSITY>0.030)
              {
                digitalWrite(RELAY_SIGNAL,HIGH);   
              }
              else digitalWrite(RELAY_SIGNAL,LOW);
              delay(1000);
               
            
    }



ESP8266.println("AT+CIPSTART=\"TCP\",\"kapot.000webhostapp.com\",80");
  delay(100);
  ESP8266.println("AT+CIPSEND=76");
  delay(100);
  String d=String(data);
  String ds="GET /vyom/receiver.php?data="+d+" HTTP/1.0";
  ESP8266.println(ds);
  ESP8266.println("Host: kapot.000webhostapp.com");
  ESP8266.println("");ESP8266.println("");
 delay(3000);
getdata();
Display("Vyom Purifier"+mode,"PM2.5: "+(String)data);
  delay(30);
// String inData = ESP8266.readStringUntil('\n');
// Serial.println("Got reponse from ESP8266: " + inData);
delay(3000);
  
  
 setup();
}


