// LaunchPad
// Authors: David Masselink and <add your name>
// Date: March 2013

#include <pins_arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Flash.h>
#include <SD.h>
#include <TinyWebServer.h>

/****************VALUES YOU CHANGE*************/
// pin 4 is the SPI select pin for the SDcard
const int SD_CS = 4;

// pin 10 is the SPI select pin for the Ethernet
const int ETHER_CS = 10;

// Don't forget to modify the IP to an available one on your home network
byte ip[] = { 192, 168, 1, 201 };
/*********************************************/

static uint8_t mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x95, 0x08 };

boolean file_handler(TinyWebServer& web_server);
boolean index_handler(TinyWebServer& web_server);
boolean api_get_handler(TinyWebServer& web_server);

TinyWebServer::PathHandler handlers[] = {
  {"/api", TinyWebServer::GET, &api_get_handler },
  {"/", TinyWebServer::GET, &index_handler },
  // Work around Arduino's IDE preprocessor bug in handling /* inside
  // strings.
  {"/" "*", TinyWebServer::GET, &file_handler },
  {NULL},
};

const char* headers[] = {
  "Content-Length",
  NULL
};

TinyWebServer web = TinyWebServer(handlers, headers);

boolean has_filesystem = true;
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;

void send_file(TinyWebServer& web_server, const char* filename) {
  if (!filename) {
    web_server.send_error_code(404);
    web_server << F("Could not parse URL");
  } else {
    if (file.open(&root, filename, O_READ)) {
      Serial << F("Read file "); Serial.println(filename);
      web_server.send_error_code(200);
      TinyWebServer::MimeType mime_type = TinyWebServer::get_mime_type_from_filename(filename);
      web_server.send_content_type(mime_type);
      web_server.end_headers();
      web_server.send_file(file);
      file.close();
    } else {
      web_server.send_error_code(404);
      web_server.end_headers();
      web_server << F("Could not find file: ") << filename << "\n";
    }
  }
}

boolean file_handler(TinyWebServer& web_server) {
  char* filename = TinyWebServer::get_file_from_path(web_server.get_path());
  send_file(web_server, filename);
  free(filename);
  return true;
}

boolean index_handler(TinyWebServer& web_server) {
  send_file(web_server, "INDEX.HTM");
  return true;
}

boolean api_get_handler(TinyWebServer& web_server){
  web_server.send_error_code(200);
  web_server.send_content_type("application/json");
  web_server.end_headers();
  //JSON opening bracket and analog array start
  web_server << F("{\"analogChannels\":{");
  for(int i = 0; i < 6; i++){
    int sensorReading = analogRead(i);
    int sensorReadingmV = map(sensorReading, 0, 1024, 0, 5000);
    web_server << F("\"") << i << F("\":") << sensorReadingmV;
    if (i < 5){
      //proper JSON doesn't have trailing commas so only add them
      //if we're not yet to the final analog channel
      web_server << F(", ");
    }
  }
  //Analog array end and digital array start
  web_server << F("}, \"digitalChannels\":{");
  for(int i = 0; i < 6; i++){
    int sensorReading = digitalRead(i);
    web_server << F("\"") << i << F("\":") << sensorReading;
    if (i < 5){
      //proper JSON doesn't have trailing commas so only add them
      //if we're not yet to the final analog channel
      web_server << F(", ");
    }
  }
  //Digital array end and JSON ending
  web_server << F("}}\n");
}

void setup() {
  Serial.begin(115200);
  Serial << F("Free RAM: ") << FreeRam() << "\n";

  pinMode(SS_PIN, OUTPUT);	// set the SS pin as an output
                                // (necessary to keep the board as
                                // master and not SPI slave)
  digitalWrite(SS_PIN, HIGH);   // and ensure SS is high

  // Ensure we are in a consistent state after power-up or a reset
  // button These pins are standard for the Arduino w5100 Rev 3
  // ethernet board They may need to be re-jigged for different boards
  pinMode(ETHER_CS, OUTPUT);	// Set the CS pin as an output
  digitalWrite(ETHER_CS, HIGH);	// Turn off the W5100 chip! (wait for
                                // configuration)
  pinMode(SD_CS, OUTPUT);	// Set the SDcard CS pin as an output
  digitalWrite(SD_CS, HIGH);	// Turn off the SD card! (wait for
                                // configuration)

  // initialize the SD card.
  Serial << F("Setting up SD card...\n");
  // pass over the speed and Chip select for the SD card
  if (!card.init(SPI_FULL_SPEED, SD_CS)) {
    Serial << F("card failed\n");
    has_filesystem = false;
  }
  // initialize a FAT volume.
  if (!volume.init(&card)) {
    Serial << F("vol.init failed!\n");
    has_filesystem = false;
  }
  if (!root.openRoot(&volume)) {
    Serial << F("openRoot failed");
    has_filesystem = false;
  }

  // Initialize the Ethernet.
  Serial << F("Setting up the Ethernet card...\n");
  Ethernet.begin(mac, ip);

  // Start the web server.
  Serial << F("Web server starting...\n");
  web.begin();

  Serial << F("Ready to accept HTTP requests.\n");
}

void loop() {
  if (has_filesystem) {
    web.process();
  }
}
