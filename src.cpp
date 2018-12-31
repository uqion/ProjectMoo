#include <TimerOne.h>
#include <SoftwareSerial.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <SPI.h>

#include <Wire.h>
#include <Adafruit_PN532.h>

#include <StandardCplusplus.h>
#include <vector>
#include <iterator>

using namespace std;

#define PN532_IRQ   (2)
#define PN532_RESET (3)
#include <SoftwareSerial.h>
#include <HX711.h>

#define DAT  30
#define CLK  31
#define calibration_factor 190400

HX711 scale(DAT, CLK);

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x60E, 0xFA};
EthernetServer server(80);

void internetStuff();

class Cow{
   private:
   uint32_t  RFID;
   char cowName[50];
   char sex [1];
   float age;
   float weight;
   float consumption;

    public:
   static std::vector <Cow> herd;
   static float startWeight;
   static float finishWeight;
   static float total;
   static volatile int flags;
   static int starttime;
   static int finish;
   static uint32_t id;

 //*************************************************************//
  Cow( uint32_t  rfid, char n [], char g[], float a, float w){

    strcpy(cowName, n);
    strcpy(sex,g);
     RFID = rfid;
     age = a;
     weight = w;
  }
  //****************************************************//
  void recalculate(float i){
    consumption += i;
   }

  //****************************************************//
   uint32_t getID(){
    return RFID;
   }
  //*****************************************************//
  char* getName(){
    return cowName;
  }
  //*****************************************************//

  char* getSex(){
    return sex;
  }
  //*****************************************************//
 float getWeight(){
    return weight;
  }
  //*****************************************************//
 float getAge(){
    return age;
  }
  //*****************************************************//

  float getConsumption(){

    return consumption;
  }
//***************************************************************//

  static void report(){

     std::vector<Cow>::iterator it;
      if(flags){
       if (starttime == false){
          id = id_cow();
          auto it = find_if(herd.begin(), herd.end(), [&id](const Cow& obj) {return obj.RFID == id;});

          if (it != herd.end())
            {
              Serial.println("************************************************************");
              Serial.print("Successfully identified cow: ");Serial.println(id);
              Serial.print("Name: ");Serial.println(it->getName());
              Serial.print("Sex: ");Serial.println(it->getSex());
              Serial.print("Age: ");Serial.print(it->getAge());Serial.println(" mo");
              Serial.print("Weight: ");Serial.print(it->getWeight());Serial.println(" kg");
            }

        Serial.println("**********************CHOW TIME*****************************");
        startWeight = read_weight();
        Serial.print("Trough mass: ");Serial.print(startWeight);Serial.println(" g");
        starttime = true;
        finish = false;
    }if(starttime){
      delay(1000);
      auto it = find_if(herd.begin(), herd.end(), [&id](const Cow& obj) {return obj.RFID == id;});

          if (it != herd.end())
            {
      Serial.print(it->getName());Serial.println(" is eating...");}
    }
      }

  if (!flags){
    if (finish == false&&starttime){

      finishWeight = read_weight();
      Serial.println("**********************CHOW TIME OVER******************************");
       Serial.print("Trough mass: ");Serial.print(finishWeight);Serial.println(" g");
      finish = true;
      starttime = false;
      total = abs((int)(startWeight)) - abs((int)(finishWeight));
      Serial.print("Total chowed this session: ");Serial.println(total);
      //debug
      auto it = find_if(herd.begin(), herd.end(), [&id](const Cow& obj) {return obj.RFID == id;});

          if (it != herd.end())
            {
      it->recalculate(total);
      Serial.print("Total consumption:");Serial.println(it->getConsumption());
      Serial.println("************************************************************");}

  }

  if (finish){
    }
 Serial.println("Waiting for cow...");
}
  }
//******************************************************//
static void RFID_read(){

  int success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength,1000);
  if(success){
    flags = true;
     report();
  }
  if (!success){
    flags =false;
    report();
  }
}
//**********************************//
static float read_weight(){
  int i;
  float rawRead;
  float readArray [10];
  float averageRead;
  long long unsigned int currentMillis = 0;
  long long unsigned int previousMillis = 0;
  long long unsigned int interval = 500;
  char tempChar;

  if(Serial.available()) {     // This IF is the tare condition. Could be changed to a button push or something
    tempChar = Serial.read();

    if(tempChar == 't') {
      scale.tare();     // Nice function built into library
      Serial.print("\n *** Scale Tared *** \n\n");
    }
  }

  currentMillis = millis();

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;     // Keep track of current time

    averageRead = 0.0;     // Reset the average to zero

    for(i = 0; i <=9; i++) {     // Put 10 scale readings into an array

      rawRead = 100 * (scale.get_units());
      readArray [i] = rawRead;
    }

    for(i = 0; i <=9; i++) {     // Extract the 10 readings from the array
      averageRead += readArray [i];
    }
    averageRead /= 10.0;     // Average the 10 readings

    return averageRead;
  }
}
//******************************************//
static uint32_t id_cow(){
  int success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;

   success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength,1000);
  if (success) {
      uint32_t cardid = uid[0];
      cardid <<= 8;
      cardid |= uid[1];
      cardid <<= 8;
      cardid |= uid[2];
      cardid <<= 8;
      cardid |= uid[3];

     return cardid;
  }
}};
//****************************************//
 //initialising static variables
  std::vector<Cow> Cow::herd;
  float Cow::startWeight;
  float Cow::finishWeight;
  float Cow::total;
  volatile int Cow::flags;
  int Cow::starttime = false;
  int Cow::finish = false;
  uint32_t Cow::id;
  //Create Cow objects

  Cow Kiki(3332309523, "Kiki Kijek", "Female", 7, 760);
  Cow Ozwald(3056166419, "Ozwald Cenhelm","Male", 12, 1200);
//***********************************************//
void setup(void) {
  Timer1.initialize();
  Timer1.attachInterrupt(internetStuff);
  //debug
  Cow::herd.push_back(Ozwald);
  Cow::herd.push_back(Kiki);
  Serial.begin(9600);


  Ethernet.begin(mac);
  server.begin();
  Serial.print(" Server is at ");
  Serial.println(Ethernet.localIP());

  nfc.begin();
  nfc.SAMConfig();

  scale.set_scale(calibration_factor); //This value obtained through black magic
  scale.tare(); //Assumes no weight on startup, tares the scale

}

void loop(void) {
  Cow::RFID_read();

}

void internetStuff() {

  EthernetClient client = server.available();  // try to get client

  if (client) {  // got client?

      boolean currentLineIsBlank = true;

      while (client.connected()) {
          if (client.available()) {   // client data available to read
              char c = client.read(); // read 1 byte (character) from client
              // last line of client request is blank and ends with \n
              // respond to client only after last line received

              if (c == '\n' && currentLineIsBlank) {

                  // send a standard http response header
                  client.println("HTTP/1.1 200 OK");
                  client.println("Content-Type: text/html");
                  client.println("Connection: close");
                  client.println();

                  // send web page
                  client.println("<meta http-equiv=\"refresh\" content=\"5\"/>");     // refresh the page automatically every 5 sec
                  client.println();
                  client.println("<!DOCTYPE html>");
                  client.println("<html>");
                  client.println("<body style=\"background-color:powderblue;\">");
                  client.println("<head>");
                  client.println("<title> COWABUNGA </title>");
                  client.println("</head>");
                  client.println("<body>");
                  client.println("<font size=\"5\" face = \"Courier\" color=\"black\" background-color: #d8da3d >");
                  client.println("<center> <H1>COWABUNGA CATTLE RANCH</H1> </center>");
                  client.println("<br>");
                  client.println("<center></p> <p> Date: Wednesday April 4 2018<center>");
                  client.println("<br> ");
                  client.println("<style>");
                  client.println("<style type=\"text/css\">body {font-family: Georgia; color: purple; background-color: #d8da3d}");
                  client.println("</style>");
                  client.print("<center> </p> <p> Kiki Kijek has eaten:<center>");client.print((Cow::herd.back()).getConsumption());
                  client.println("g");
                  client.println("<center> </p> <p> Ozwald Cenhelm has eaten: </center>");client.print((Cow::herd.front()).getConsumption());
                  client.println("g");
                  client.println("</font>");
                  client.println("</body>");
                  client.println("</body>");
                  client.println("</html>");
                  break;
              }

              // every line of text received from the client ends with \r\n
              if (c == '\n') {
                  // last character on line of received text
                  // starting new line with next character read
                  currentLineIsBlank = true;
              }
              else if (c != '\r') {
                  // a text character was received from client
                  currentLineIsBlank = false;
              }

          } // end if (client.available())
      } // end while (client.connected())
      delay(1);      // give the web browser time to receive the data
      client.stop(); // close the connection
  }     // end if (client)
}
