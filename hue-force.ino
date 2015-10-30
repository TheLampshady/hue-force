#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#include <queue>

using namespace std;

typedef struct {
  int8_t x;
  int8_t y;
  int8_t z;
} Acceleration;

class HueForce {

 public:

  // Constructor: number of LEDs, pin number, LED type
  HueForce(int8_t force, int numPixels);
  HueForce(int8_t force, int numPixels, int8_t lightPin, int8_t accelPine);
  ~HueForce();
  
  void run(float);
  void setLimit(int8_t force);

 private:
  std::queue<Acceleration*> lightWake;
  int 
    numPixels;
  
  float
    modifier,
    delta,
    brightness;
  
  Adafruit_NeoPixel pixels;
  Adafruit_ADXL345_Unified accel;
  
  int8_t lightPin;
  
  void initLightWake(int8_t count);
  void mergeResults(Acceleration axis);
  void drawForce(Acceleration axis);
  void drawBrightness(int bright);
  void displayStatus(void);
  Acceleration getResults(void);
  
};

HueForce::HueForce(int8_t force, int count, int8_t pin) : delta(0){
    modifier = 255.0 / force
    numPixels = count;
    lightPin = pin;

    accel = Adafruit_ADXL345_Unified(12345);
    accel.begin();
    
    pixels = Adafruit_NeoPixel(numPixels, lightPin, NEO_GRB + NEO_KHZ800);
    pixels.begin();
    
    setLimit(force);
    initLightWake(5);
}

HueForce::~HueForce(){
    if(accel)   delete accel;
    if(pixels)  delete pixels;

}

HueForce::setLimit(int8_t force){
    switch(force) {
        case 2 : accel.setRange(ADXL345_RANGE_2_G);
        case 4 : accel.setRange(ADXL345_RANGE_2_G);
        case 8 : accel.setRange(ADXL345_RANGE_2_G);
        case 16 : accel.setRange(ADXL345_RANGE_2_G);
    }
}

HueForce::initLightWake(int8_t count){
    Acceleration *axis;
    
    for (int i = 0; i < count; ++i){
        axis = new Acceleration;
        axis.x = 0;
        axis.y = 0;
        axis.z = 0;
        
        lightWake.push(axis);
    }
}

HueForce::getResults(){
    Acceleration *axis;
    sensors_event_t event; 
    accel.getEvent(&event);
    
    axis = new Acceleration;
    axis->x = event.acceleration.x;
    axis->y = event.acceleration.y;
    axis->z = event.acceleration.z;
    
    mergeResults(axis);
    
}

HueForce::mergeResults(Acceleration *axis){
    int x=0, y=0, z=0;
    int size;

    lightWake.push(axis);
    for (int i = 0; i < lightWake; ++i){
        axis = lightWake.pop();
        x += axis->x;
        y += axis->y;
        z += axis->z;
        lightWake.push(axis);
    }
    size = lightWake.size()
    axis = lightWake.pop();
    axis->x = x/size;
    axis->y = y/size;
    axis->z = z/size;
    
    return axis;
}

void HueForce::drawBrightness(float bright){
        if (bright > 255)
            bright = 255;
        else if (bright < 15)
            bright = 15;
                
        if (bright < brightness)
            brightness = brightness - delta;
        else
            brightness = bright;
        
        pixels.setBrightness(brightness)
}

void HueForce::drawForce(Acceleration *axis) {
    uint8_t red, blue, green;
    uint32_t color;
    
    red = abs(int(axis->x * modifier));
    blue = abs(int(axis->x * modifier));
    green = abs(int(axis->x * modifier));
    color = pixels.Color(red, blue, green);
            
    for(int i=0; i < numPixels; i++){  
        pixels.setPixelColor(i, color); // Moderately bright green color
        pixels.show(); // This sends the updated pixel color to the hardware.
    }
    
    drawBrightness(red+green+blue);
    pixels.show();
}

void HueForce::run(void) {
    Acceleration *axis;
    axis = getResults();
    
    drawForce(axis);
}




int delayval = 500; // delay for half a second

void setup() {
  Serial.begin(9600);
  HueForce hueForce = HueForce(8, 32, 15)
}


void loop() {

  hueForce.run()
  delay(delayval); // Delay for a period of time (in milliseconds).
}

