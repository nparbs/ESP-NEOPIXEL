//NanoLeaf Project
//Nick Parbs
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <ESP8266WiFi.h>

//Setup Pixel Strip
#define PIN 2 //D4 Pin on Wemos D1 Mini
#define LEDS 54
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800); //number of pixels in strip, Arduino pin number, pixel type flags

//Wifi Parameters
const char* ssid = "MySpectrumWiFib8-2G";
const char* password = "jollyclock887";
//const char* ssid = "nparbs";
//const char* password = "nparbs123";

WiFiServer server(80);

//Vars
int triangles = 9;
int anim = 0;
String animation = "Select an Animation";
int animSpeed = 50;

//Arrays
byte ColorSet1[] = {80, 90, 105, 120, 140, 170, 200, 215, 235, 255};
byte ColorSet2[] = {155, 185, 190, 200, 210, 220, 230, 240, 2450}; //vaporwave
byte ColorSet3[] = {145, 155, 190, 200}; //vaporwave simple
byte ColorsWarm[] = {0, 5, 10, 15, 7, 13, 17, 255};
byte ColorsCool[] = {125, 140, 155, 170, 185};
byte OrderSet[] = {3, 7, 5, 1, 4, 8, 2, 9, 6};
byte OrderSet2[] = {8, 1, 6, 4, 7, 3, 9, 2, 5};
byte OrderSet3[] = {2, 8, 5, 1, 6, 3, 9, 4, 7};

//SETUP --------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  //Main setup
  Serial.begin(115200);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);  //WIFI Connect

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  server.begin();  // Start the server
  Serial.println("Server started");
  Serial.println(WiFi.localIP());  // Print the IP address
  colorWipeTri(strip.Color(0, 0, 255), 100); // Blue

  Serial.println("Started!"); //End of WIFI Connect

}

//MAIN ----------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {

  webLoop(); //Check web requests

  switch (anim) { //Choose animation
    case 0:
      Serial.println("case0");
      //colorWipe(strip.Color(0, 0, 0, 0), 50);
      //colorWipe3(strip.Color(0, 256, 0, 0), 50);
      break;
    case 1:
      animation = "Rainbow 1";
      Serial.println("case1 - rainbow");
      rainbow(20);
      break;
    case 2:
      animation = "Vaporwave 1";
      Serial.println("case2 - vaporwave");
      triFadeSet(ColorSet2[random(0, sizeof(ColorSet2))], 1);
      colorFadeTri(ColorSet2[random(0, sizeof(ColorSet2))], 3, 4);
      colorFadeTri(ColorSet2[random(0, sizeof(ColorSet2))], 3, 8);
      colorFadeTri(ColorSet2[random(0, sizeof(ColorSet2))], 3, 2);
      colorFadeTri(ColorSet2[random(0, sizeof(ColorSet2))], 3, 6);
      break;
    case 22:
      animation = "Vaporwave 2";
      Serial.println("case22 - vaporwave2");
      colorFadeTri(ColorSet2[random(0, sizeof(ColorSet2))], 1, random(0, 9));
      break;
    case 3:
      animation = "Random Triangles";
      Serial.println("case3 - random");
      colorFadeTri(Wheel(random(1, 255)), 1, random(1, 10));
      break;
    case 4:
      animation = "Cool";
      Serial.println("case4 - cool");
      triFadeSet(ColorsCool[random(0, sizeof(ColorsCool))], 1);
      colorFadeTri(ColorsCool[random(0, sizeof(ColorsCool))], 5, random(0, 9));
      colorFadeTri(ColorsCool[random(0, sizeof(ColorsCool))], 5, random(0, 9));
      colorFadeTri(ColorsCool[random(0, sizeof(ColorsCool))], 2, random(0, 9));
      break;
    case 5:
      animation = "Warm";
      Serial.println("case5 - warm");
      triFadeSet(ColorsWarm[random(0, sizeof(ColorsWarm))], 1);
      colorFadeTri(ColorsWarm[random(0, sizeof(ColorsWarm))], 5, random(0, 9));
      colorFadeTri(ColorsWarm[random(0, sizeof(ColorsWarm))], 5, random(0, 9));
      colorFadeTri(ColorsWarm[random(0, sizeof(ColorsWarm))], 5, random(0, 9));
      break;
    case 6:
      animation = "Rainbow 2";
      Serial.println("case6 - Tri Fade Rainbow");
      triFadeNextFull(25, 1);
      break;
  }

  delay(0);
}

//Web Loop --------------------------------------------------------------------------------------------------------------------------------------------------
void webLoop() {

  WiFiClient client = server.available(); // Check if a client has connected
  //Serial.print(">");
  delay(1);
  if (!client) {
    return;
  }

  Serial.println("new client");
  while (!client.available()) { // Wait until the client sends some data
    delay(5);
    Serial.print("waiting...");
  }

  String req = client.readStringUntil('\r'); // Read the first line of the request
  Serial.println("req = " + req);

  // Match the request
  if (req.indexOf("") != -1) {
    Serial.println("request submitted");
    //Serial.println(client.remoteIP());
    req.replace("GET /?anim=", "");
    Serial.println("req anim = " + req);
    if (req.toInt() > 0) {
      Serial.println("Request approved");
      anim = req.toInt();
      Serial.println("SWITCH-----------");
      //colorWipeTri(strip.Color(0, 0, 0, 0), 30);

    } else {
      Serial.println("Request error");
    }
  } else {
    Serial.println("invalid request");
    //client.stop();
    return;
  }

  client.flush();
  // Prepare the response
  String s = "<!DOCTYPE html><html lang='en'><meta http-equiv='refresh' content='35'><head><title>LED Animation Selection</title><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'><link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css'><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script><script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.16.0/umd/popper.min.js'></script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/4.4.1/js/bootstrap.min.js'></script></head>";
  s += "<body><div class='container' style='margin-top:30px'><div class='row'><h2>LED Animation Selection</h2></div><div class='row'><form method='get'><div class='btn-group'>";
  s += "<button type='submit' formmethod='get' class='btn btn-success' name='anim' value='1'>Rainbow</button>";
  s += "<button type='submit' formmethod='get' class='btn btn-info' name='anim' value='2'>VaporWave1</button>";
  s += "<button type='submit' formmethod='get' class='btn btn-info' name='anim' value='22'>VaporWave2</button>";
  s += "<button type='submit' formmethod='get' class='btn btn-secondary' name='anim' value='3'>Random Colors</button>";
  s += "<button type='submit' formmethod='get' class='btn btn-primary' name='anim' value='4'>Cool</button>";
  s += "<button type='submit' formmethod='get' class='btn btn-danger' name='anim' value='5'>Warm</button>";
  s += "<button type='submit' formmethod='get' class='btn btn-info' name='anim' value='6'>Rainbow2</button>";
  s += "</div></form></div><div class='row'><p>Current Animation: ";
  s += animation;
  s += "</p></div></div></body></html>";

  // Send the response to the client`
  client.print(s);
  delay(10);
  Serial.println("Client disonnected - End of Web Loop");
  client.stop();
}

//ANIMATIONS-----------------------------------------------------------------------------------------------------------------------------------------------------
void colorWipe(uint32_t c, uint8_t wait) { // Fill the dots one after the other with a color
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorWipeTri(uint32_t c, uint8_t wait) { // Fill the triangles one after the other with a color
  int t = 0;
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    t++;
    if (t == 5) {
      strip.show();
      delay(wait);
      t = 0;
    }
  }
}

void colorTri(uint32_t c, uint8_t wait, int tri) { // Fill a triangle with a color
  int t = 0;
  int s =  (tri - 1) * 6;
  Serial.println(s);
  for (uint16_t i = s; i < s + 6; i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(wait);

}

void colorFadeTri(byte WheelPos, uint8_t wait, int tri) { // Fade color of triangle
  int t = 0;
  int s = (tri - 1) * 6;
  //Serial.println(s);
  uint8_t r;
  uint8_t g;
  uint8_t b;

  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    r = 255 - WheelPos * 3;
    g = 0;
    b = WheelPos * 3;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    r = 0;
    g = WheelPos * 3;
    b = 255 - WheelPos * 3;
  } else {
    WheelPos -= 170;
    r = WheelPos * 3;
    g = 255 - WheelPos * 3;
    b = 0;
  }

  uint8_t curr_r, curr_g, curr_b;

  uint32_t curr_col = strip.getPixelColor(s); // get the current colour
  curr_b = curr_col & 0xFF; curr_g = (curr_col >> 8) & 0xFF; curr_r = (curr_col >> 16) & 0xFF;  // separate into RGB components

  while ((curr_r != r) || (curr_g != g) || (curr_b != b)) { // while the curr color is not yet the target color
    if (curr_r < r) curr_r++; else if (curr_r > r) curr_r--;  // increment or decrement the old color values
    if (curr_g < g) curr_g++; else if (curr_g > g) curr_g--;
    if (curr_b < b) curr_b++; else if (curr_b > b) curr_b--;
    for (uint16_t i = s; i < s + 6; i++) {
      strip.setPixelColor(i, curr_r, curr_g, curr_b);  // set the color
      delay(wait);  // add a delay if its too fast
    }
    strip.show();

  }
  delay(wait);
  webLoop();
}

void triFadeNext(uint32_t c, uint8_t wait) { //Fade all triangles in order
  for (uint16_t i = 1; i <= triangles; i++) {
    colorFadeTri(c, wait, i);
    delay(wait);
  }
}

void triFadeNextFull(uint32_t s, uint8_t wait) { //Fade all triangles in order + color change every sweep
  for (uint16_t a = 0; a <= 255; a = a + s) {
    for (uint16_t i = 1; i <= triangles; i++) {
      colorFadeTri(a, wait, i);
      //delay(wait);
    }
  }
}

void triFadeSet(uint32_t c, uint8_t wait) { // Fade all triangles in set order
  for (uint16_t i = 0; i < sizeof(OrderSet); i++) {
    colorFadeTri(c, wait, OrderSet[i]);
    delay(wait);
  }
}

void rainbow(uint8_t wait) { // Rainbow Fade
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    webLoop();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) { // Slightly different, this makes the rainbow equally distributed throughout
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void theaterChase(uint32_t c, uint8_t wait) { //Theatre-style crawling lights.
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(uint8_t wait) { //Theatre-style crawling lights with rainbow effect
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

uint32_t Wheel(byte WheelPos) { // Input a value 0 to 255 to get a color value.
  WheelPos = 255 - WheelPos; // The colours are a transition r - g - b - back to r.
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
