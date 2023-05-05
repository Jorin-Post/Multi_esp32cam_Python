#include "esp_camera.h"
#include <WiFi.h>
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems

#define CAMERA_MODEL_AI_THINKER 
//#define CAMERA_MODEL_WROVER_KIT 
//#define CAMERA_MODEL_ESP_EYE 
//#define CAMERA_MODEL_M5STACK_PSRAM 
//#define CAMERA_MODEL_M5STACK_V2_PSRAM 
//#define CAMERA_MODEL_M5STACK_WIDE
//#define CAMERA_MODEL_M5STACK_ESP32CAM 
//#define CAMERA_MODEL_TTGO_T_JOURNAL 

#include "camera_pins.h"

const char* ssid = "*****"; // Enter wifi name
const char* password = "*****"; // Enter Wifi ww
// When needed change server port for every esp32cam when uplouding and add them to python script
const int ServNum = 80; 
void startCameraServer(int Servnum);

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 5000000; //default = 10 mhz
  config.pixel_format = PIXFORMAT_JPEG;
  
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if(psramFound()){
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_HVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }


  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  if (s->id.PID == OV5640_PID) {
    // initial sensors are flipped vertically and colors are a bit saturated
    s->set_vflip(s, 1); // flip it back
    s->set_framesize(s, FRAMESIZE_HVGA);
  }
  
  delay(2000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer(ServNum);

  Serial.print("Copy URL in Python: http://");
  Serial.println(WiFi.localIP());

  Serial.print("Open in browser to test init cam: http://");
  Serial.print(WiFi.localIP());Serial.print(":");Serial.print(ServNum+1);Serial.println("/stream");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10000);
}
