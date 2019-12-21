//Wifi Jazz
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

bool wifi_timout = 0;
const char* ssid = "SSID";
const char* password = "Password";

//Neopixel jazz
#include <NeoPixelBus.h>
const uint16_t PixelCount = 38; 
const uint8_t PixelPin = 19; 

int colorSaturation = 50;
int Brightness = 50;
int R = 0;
int G = 0;
int B = 0;
int Pulse = 1600; //Half the time between pulsations
RgbColor black(0);

//Button Jazz
float Button1_total = 0;
int smoothing = 50;
const int debounce = 5;
float Button1[debounce];
bool PWR = 0;

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> ring(PixelCount, PixelPin);


void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed!");
    delay(5000);
    //ESP.restart();
  }
  OTA_init();
     touch_pad_init();
    touchSetCycles(0x6000 ,0x6000);
   // touch_pad_set_cnt_mode(0, TOUCH_PAD_SLOPE_7, TOUCH_PAD_TIE_OPT_HIGH);
    ring.Begin();
    ring.Show();
}   
 

void loop() {
  if (millis()<600000){ArduinoOTA.handle();}
  else if (wifi_timout==0){
    ArduinoOTA.end(); 
    wifi_timout = 1;
    WiFi.mode(WIFI_OFF);
    btStop();
    }

  
  if (button1_capture()==1){  
    ring.ClearTo(black);ring.Show();
    PWR = !PWR;
    }
  if (millis()<1500){PWR = 0;}
  if (ring.CanShow()&&PWR==1){Light(0);}
  delay(10);
}

void OTA_init(){
   ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

int button1_capture(){
    for (int i=0; i<(debounce-1);i++){
    Button1[i]=Button1[i+1];
  }
    Button1[debounce-1] = (touchRead(T0));
    float current = MaxArray(Button1);
    float Button1_smooth = Button1_total/smoothing;

//  float current = AveArray(Button1);
  Serial.print(Button1_smooth);Serial.print("   ");Serial.print(current);Serial.print("    ");Serial.println(Button1[debounce-1]);
  if (current < (0.85*Button1_smooth)){
    Button1_total = 0; //works as a debounce
    return 1;
  }else{
    Button1_total = current +Button1_total -Button1_smooth;
  }
  return 0;
}

float MaxArray(float MaxMe[]){
  float mxm = MaxMe[0];
  float mnm = MaxMe[0];
  for (int i=0; i<debounce; i++) {
    if (MaxMe[i]>mxm) {
      mxm = MaxMe[i];
    }
  }
return mxm;
}

float AveArray(float AveMe[]){
  float total = 0;
  float ave = 0;
  for (int i=0; i<debounce; i++) {
      total+=AveMe[i];
  }
  ave = total/debounce;
return ave;
}

void Light(bool flag){
  ring.ClearTo(black);
  
  //Calculate pulse status 
  if (flag==1){
    int timer = (millis() % (Pulse*2));
    Brightness = 80+40*(abs(timer-Pulse))/Pulse;
  }else{
    Brightness = 100;
  }
  R = Brightness;
  G = Brightness* 0.6;
  B = Brightness* 0.6;
  

  for (int i = 0; i<=38; i++){
  ring.SetPixelColor(i, RgbColor(R,G,B));
  }
 
  
  //remove requirement for 0% if statement
    ring.Show();
}


void Portion(int lights){
  lights = lights/3;
    ring.ClearTo(black);
    Serial.println(lights);
    for (int i = 0; i<=lights; i++){
  ring.SetPixelColor(i, RgbColor(50,50,50));
      ring.Show();
  }
}
