/*********************************************************************
Date: 12/07/2019 : DQScript
Author : Abhishek Kumar, Fatigue & Fracture Laboratory, CSIR-SERC
This is the code for Fetching LOAD and CMOD value from analog outputs of actuators and UTM. 
Output: In google sheets in tabular form 
*********************************************************************/
#include <HTTPClient.h>
#include "HTTPSRedirect.h"
#include <WiFi.h>
char myssid[] = "Constantine";         // your network SSID (name)
char mypass[] = "ArthurDent";          // your network password
String url= "https://script.google.com/macros/s/AKfycbwRNCc8k98wNDe3Fsxp6q7bWoLL1PGFOo3qhD7ZcKwK5MEIjtQ/exec?";
//https://script.google.com/macros/s/AKfycbwRNCc8k98wNDe3Fsxp6q7bWoLL1PGFOo3qhD7ZcKwK5MEIjtQ/exec

#define host "script.google.com"
#define googleRedirHost "script.googleusercontent.com"
HTTPSRedirect client(443);
char bssid[6];
String reading1="";
String reading2="";

unsigned int localPort = 8888;  // local port to listen for UDP packets
int status = WL_IDLE_STATUS;
int more_text = 1; // set to 1 for more debug output
void setup()   {
  Serial.begin(115200);
  WiFi.begin(myssid,mypass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    Serial.println(".");
  }  
  for(int x=0;x<=300;x++)
  {
 double Load = (analogRead(A0) * 3.3 ) / (4095);
  Serial.print("Load = ");
  Serial.print(Load);
  Serial.print("volts");
  double CMOD = (analogRead(A3) * 3.3 ) / (4095);
  Serial.print("          CMOD = ");
  Serial.print(CMOD);
  Serial.println("volts"); 
  reading1 = reading1 + String(Load)+"a";
  reading2 = reading2 + String(CMOD)+"a";
  }
  postData(reading1,reading2);
  ESP.restart();
}
void loop()
{

}

void postData(String s1,String s2){ 
   
    bool flag = false; 
    for (int i=0; i<5; i++){ 
            int retval = client.connect(host, 443); 
            if (retval == 1) { 
                        flag = true; 
                        break; 
            } 
            else 
                    Serial.println("Connection failed. Retrying.."); 
    }

    // Connection Status, 1 = Connected, 0 is not. 
    //Serial.println("Connection Status: " + String(client.connected())); 
    Serial.flush(); 
        if (!flag){ 
            Serial.print("Could not connect to server: "); 
            Serial.println(host); 
            Serial.println("Exiting.."); 
            Serial.flush(); 
            return; 
    }

  
    if (!client.connected()){ 
            Serial.println("Connecting to client again.."); 
            client.connect(host, 443); 
    } 


    String urlFinal = url+"sensor_value1="+String(s1)+"&sensor_value2="+String(s2); 
    Serial.println(urlFinal);
    client.printRedir(urlFinal, host, googleRedirHost); 
    Serial.println("closing connection");
    client.stop();
}
