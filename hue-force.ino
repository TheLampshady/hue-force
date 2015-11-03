#include <QueueArray.h>

#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#include <QueueArray.h>
#include <math.h>

#define LIGHT_PIN 15
#define LIGHT_COUNT 32
#define G_FORCE 8
#define DELAY_VALUE 500  // Delay for a period of time (in milliseconds).

typedef struct {
  int x;
  int y;
  int z;
} Acceleration;

class HueForce {

 public:
  Acceleration*
    axis;
    
  QueueArray <Acceleration*> 
    lightWake;

  // Constructor: number of LEDs, pin number, LED type
  HueForce(int force, int pixelCount, int lightPin);
  ~HueForce();
  
  void start(void);
  void displayStatus(Acceleration* axisPrint);

 private:
    
  int 
    lightPin,
    modifier,
    lighCount;
  
  float
    delta,
    totalBrightness;
  
  Adafruit_NeoPixel pixels;
  Adafruit_ADXL345_Unified accel;

  void setLimit(int force);
  void initLightWake(int history);
  
  void mergeResults(void);
  void setAxis(void);
  
  void drawForce(void);
  void drawBrightness(float bright);
  
};

HueForce::HueForce(int force, int pixelCount, int pin) : delta(1), totalBrightness(0){
    modifier = int(255.0 / force);
    lighCount = pixelCount;
    lightPin = pin;

    axis = new Acceleration;
    Serial.print("RUN AXIS1: "); Serial.println((int)axis);

    accel = Adafruit_ADXL345_Unified(12345);
    if(!accel.begin()){
      Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
      while(1);
    }
    
    pixels = Adafruit_NeoPixel(lighCount, lightPin, NEO_GRB + NEO_KHZ800);
    pixels.begin();
    pixels.setBrightness(0);
    pixels.show();
    
    setLimit(force);
    initLightWake(5);
    
}

HueForce::~HueForce(){
    //delete axis;
    //delete accel;
    //delete pixels;

}


void HueForce::setLimit(int force){
    switch(force) {
        case 2 : accel.setRange(ADXL345_RANGE_2_G);
        case 4 : accel.setRange(ADXL345_RANGE_4_G);
        case 8 : accel.setRange(ADXL345_RANGE_8_G);
        case 16 : accel.setRange(ADXL345_RANGE_16_G);
    }
}

void HueForce::initLightWake(int history){
    Acceleration *initAxis;

    for (int i = 0; i < history; ++i){
        initAxis = new Acceleration;
        initAxis->x = 1;
        initAxis->y = 1;
        initAxis->z = 1;
        
        lightWake.push(initAxis);
    }
}

void HueForce::setAxis(){
    sensors_event_t event; 
    accel.getEvent(&event);
    
    axis->x = int(abs(event.acceleration.x / SENSORS_GRAVITY_STANDARD));
    axis->y = int(abs(event.acceleration.y / SENSORS_GRAVITY_STANDARD));
    axis->z = int(abs(event.acceleration.z / SENSORS_GRAVITY_STANDARD));
   
    //mergeResults();
}

void HueForce::mergeResults(){
    Acceleration *iter;
    int x=0, y=0, z=0;
    int queueSize;
    
    lightWake.push(axis);
    queueSize = lightWake.count();

    for (int i = 0; i < queueSize; i++){
        iter = lightWake.pop();
        x += iter->x;
        y += iter->y;
        z += iter->z;
        lightWake.push(iter);
    }

    axis = lightWake.pop();
    
    axis->x = int(max(min(x/queueSize, 255), 0));
    axis->y = int(max(min(y/queueSize, 255), 0));
    axis->z = int(max(min(z/queueSize, 255), 0));
}

void HueForce::drawBrightness(float bright){
        bright = abs(float(pow(float(bright),1.5)/16));
        bright = max(min(bright, 255), 15);

        if (totalBrightness > 250)
            delta = delta*0.05 ;
        else if (totalBrightness > 240)
            delta = delta;
            
        if (bright < totalBrightness)
          totalBrightness -= delta;
        else
          totalBrightness = bright ;

        pixels.setBrightness(totalBrightness);
}

void HueForce::drawForce() {
    int red, blue, green;
    
    red = abs(int(axis->x * modifier));
    green = abs(int(axis->y * modifier));
    blue = abs(int(axis->z * modifier));

    for(int i = 0; i < lighCount; i++){
        pixels.setPixelColor(i, pixels.Color(red, green, blue));
        pixels.show(); // This sends the updated pixel color to the hardware.
    }

    drawBrightness(red+green+blue);
    
    pixels.show();

}
void HueForce::displayStatus(Acceleration* axisPrint) {
  Serial.print("X: ");Serial.print(axisPrint->x);Serial.print(" ");
  Serial.print("Y: ");Serial.print(axisPrint->y);Serial.print(" ");
  Serial.print("Z: ");Serial.print(axisPrint->z);Serial.print(" ");
  Serial.print("B: ");Serial.println(totalBrightness);
}

void HueForce::start(void) {
    setAxis();
    drawForce();
}

HueForce* hueForce;

void setup() {
  Serial.begin(9600);
  while (!Serial) ;
  hueForce = new HueForce(G_FORCE, LIGHT_COUNT, LIGHT_PIN);
}


void loop() {
  hueForce->start();
  hueForce->displayStatus(hueForce->axis);
  //delay(DELAY_VALUE);
  delay(5000);
}

