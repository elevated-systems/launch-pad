/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x00, 0x95, 0x08 };
IPAddress ip(192,168,1, 201);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  //Now init the SD card interface...
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    //Serial.println("new client");
    String httpRequest = "";
    String httpVerb = "";
    String httpResource = "";
    int httpResourceStartIndex = 0;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        //Build the request string byte by byte
        char c = client.read();
        httpRequest = httpRequest + c;
        //But once the line ends, check to see the HTTP verb
        //and resource requested
        if (c == '\n' && (
            httpRequest.startsWith("GET") ||
            httpRequest.startsWith("POST"))){
          //Serial.println(httpRequest);
          //Trim away any potential leading whitespace
          httpRequest.trim();
          //Use some janky while loops to parse out the verb and resource
          int i = 0;
          while(httpRequest[i] != ' '){
            i++;
          }
          httpVerb = httpRequest.substring(0, i);
          //Bump the index (since we know it's looking at a space) and
          //re-assign to i as well as the startIndex var
          httpResourceStartIndex = i = i + 1;
          while(httpRequest[i] != ' '){
            i++;
          }
          httpResource = httpRequest.substring(httpResourceStartIndex, i);
          Serial.println(httpVerb);
          Serial.println(httpResource);
          httpRequest = "";
          //Done parsing
          //Now respond
          if (httpVerb == "GET" && httpResource.startsWith("/api")){
            sendBoardState(client);
            break;
          }
          else {
            //File type support for: html, js, css, gif, png, jpeg
            client.println("HTTP/1.1 200 OK");
            if (httpResource.endsWith(".htm")){
              client.println("Content-Type: text/html");
            }else if(httpResource.endsWith(".js")){
              client.println("Content-Type: text/javascript");
            }else if(httpResource.endsWith(".css")){
              client.println("Content-Type: text/css");
            }else if(httpResource.endsWith(".gif")){
              client.println("Content-Type: image/gif");
            }else if(httpResource.endsWith(".png")){
              client.println("Content-Type: image/png");
            }else if(httpResource.endsWith(".jpg")){
              client.println("Content-Type: image/jpeg");
            }else{
              client.println("Content-Type: text/plain");
            }
            client.println("Connnection: close");
            client.println();
            //Can't use a string to open a file on SD
            //so convert the string to a char array first
            char strBuf[50];
            httpResource.toCharArray(strBuf, 50);
            File dataFile = SD.open(strBuf);
            if (dataFile) {
              while (dataFile.available()) {
                client.write(dataFile.read());
              }
              dataFile.close();
            }  
            // if the file isn't open, pop up an error:
            else {
              Serial.println("error opening file");
            }
            //client.println("Why not try the API?");
            break;
          }
        }
        
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
//        if (c == '\n' && currentLineIsBlank) {
//          //Serial.println(request);
//          // send a standard http response header
//          client.println("HTTP/1.1 200 OK");
//          client.println("Content-Type: text/html");
//          client.println("Connnection: close");
//          client.println();
//          File dataFile = SD.open("TESTFILE.HTM");
//
//          // if the file is available, write to it:
//          if (dataFile) {
//            //dataFile.write("Secret text from 4:06");
//            while (dataFile.available()) {
//              client.write(dataFile.read());
//            }
//            dataFile.close();
//          }  
//          // if the file isn't open, pop up an error:
//          else {
//            Serial.println("error opening file");
//          }
//          client.println("<!DOCTYPE HTML>");
//          client.println("<html>");
//                    // add a meta refresh tag, so the browser pulls again every 1 seconds:
//          client.println("<meta http-equiv=\"refresh\" content=\"1\">");
//          // output the value of each analog input pin
//          for (int analogChannel = 0; analogChannel < 2; analogChannel++) {
//            int sensorReading = analogRead(analogChannel);
//            client.print("analog input ");
//            client.print(analogChannel);
//            client.print(" is ");
//            client.print(sensorReading);
//            client.println("<br />");
//            client.print("Which is: ");
//            int sensorReadingmV = map(sensorReading, 0, 1024, 0, 5000);
//            client.print(sensorReadingmV); //map 10bit ADC reading to 5000mV value range
//            client.print("mV");
//            if (analogChannel == 0){
//              client.print(", which is: ");
//              int sensorReadingDegK = sensorReadingmV/10;
//              client.print(sensorReadingDegK); //map that 5000mv reading into degK range per spec sheet remark: "10mv per degK absolute"
//              client.print("degK, which is: "); 
//              client.print((sensorReadingDegK-273.15)*1.8+32);
//              client.print("degF!");
//            }
//            client.println("<br />");    
//          }
//          client.println("</html>");
//          break;
//        }
//        if (c == '\n') {
//          // you're starting a new line
//          currentLineIsBlank = true;
//        } 
//        else if (c != '\r') {
//          // you've gotten a character on the current line
//          currentLineIsBlank = false;
//        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void sendBoardState(EthernetClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connnection: close");
  client.println();
  client.print("{"); //JSON opening bracket
  client.print("\"analogChannels\":["); //array opening
  for(int i = 0; i < 6; i++){
    int sensorReading = analogRead(i);
    int sensorReadingmV = map(sensorReading, 0, 1024, 0, 5000);
    client.print("{\"a");
    client.print(i);
    client.print("\":");
    client.print(sensorReadingmV);
    client.print("}");
    if (i < 5){
      //proper JSON doesn't have trailing commas so only add them
      //if we're not yet to the final analog channel
      client.print(", ");
    }
  }
  client.print("]"); //array ending
  client.print("}"); //JSON ending
  client.println();
}

