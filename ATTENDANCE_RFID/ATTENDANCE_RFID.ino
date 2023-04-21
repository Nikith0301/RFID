// THIS VERSION INCLUDES ONLY PRINTING TAG ADDRESS INTO THE GOOGLE SHEETS

#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5//for esp32
#define RST_PIN 21

MFRC522 rfid(SS_PIN, RST_PIN);


//Wifi
const char* ssid="Redmi 9 Power";
const char* password="12345678";
//google script
String GOOGLE_SCRIPT_ID ="AKfycbwfZ9XMh2sIUOT17CiKNKJG4G34C-VB-T9Zm83WtKg41rP88YdBFPrvAqhwT7MpoVNo";
int count=1;
String name="Sai";

void setup() {
  // put your setup code here, to run once:
       Serial.begin(9600);
  delay(1000);
  // connect to WiFi
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
     SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522

  Serial.println("Tap RFID/NFC Tag on reader");  


}
void loop() {
  // put your main code here, to run repeatedly:
      String str="";
      
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
//    Serial.println("card is present");
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.print("RFID/NFC Tag Type: ");
      Serial.println(rfid.PICC_GetTypeName(piccType));

      // print NUID in Serial Monitor in the hex format
      Serial.print("UID:");
      for (int i = 0; i < rfid.uid.size; i++) {
//        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        
//        Serial.print(rfid.uid.uidByte[i], HEX);
        Serial.print(rfid.uid.uidByte[i]);
        str=str+String(rfid.uid.uidByte[i]);
//        Serial.print(rfid.uid.uidByte[i], HEX);
      }
      Serial.println("String is"+str);

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
else{
  Serial.println("place card");
  delay(1500);
}
  if(str=="351902426"){
    name="Jilla";
  }
  else if(str=="1942278927"){
    name="K.Santosh";
  }
  else if(str=="10116423333"){
    name="Bahadur";
  }
  else{
     name="Empty";
  }
String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+ "count=" + String(count)+"&tag=" + name ;

Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
   int httpCode =  http.GET(); 
//    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);    
    }
  //  ---------------------------------------------------------------------
    http.end();
  
  count++;
  delay(500);




}
