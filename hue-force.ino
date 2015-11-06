#include <QueueArray.h>

#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

//#include <QueueArray.h>
#include <math.h>

#define LIGHT_PIN 15
#define LIGHT_COUNT 32
#define G_FORCE 8
#define DELAY_VALUE 10  // Delay for a period of time (in milliseconds).
#define DEBUG false

// ----------- ACCEL AXIS STRUCT ------------
typedef struct ANode{
  int x;
  int y;
  int z;
  struct ANode * n;
} Acceleration;

// ----------- QUEUE ------------
class AxisQueue
{
private:
  Acceleration * front;                       // pointer to front of Queue
  Acceleration * rear;                        // pointer to rear of Queue
                        // current number of items in Queue


public:
  AxisQueue(int amount);                 // create queue with a qs limit
  ~AxisQueue();
  void addAxis(Acceleration * axis);
  int * getAverage(void);
  void printQueue(void);
};

AxisQueue::AxisQueue(int amount){
    Acceleration * tmp;
  
    front = NULL;
    rear = NULL;
    
    for(int x = 0; x < amount; x++){
      tmp = new Acceleration;
      *tmp = {1,1,1,NULL};
      tmp->n = front;
      front = tmp;
      if (x == 0) rear = front;
    }
  
}

void AxisQueue::addAxis(Acceleration * axis){
    rear -> n = front;
    rear = front;
    front = front -> n;
    rear -> n = NULL;

    rear->x = axis->x;
    rear->y = axis->y;
    rear->z = axis->z;
}

int * AxisQueue::getAverage(){
    Acceleration * iter = front;
    int * avg = new (int[3]){0,0,0};
    int counter = 0;

    while (iter != NULL){
          avg[0] += iter->x;
          avg[1] += iter->y;
          avg[2] += iter->z;
          iter = iter->n;
          counter++;
    }
    avg[0] = int(max(min(avg[0]/counter, 255), 0));
    avg[1] = int(max(min(avg[1]/counter, 255), 0));
    avg[2] = int(max(min(avg[2]/counter, 255), 0));
    return avg;
}

void AxisQueue::printQueue(){
    Acceleration * iter = front;
    Serial.println("Queue Status!");
    while (iter != NULL){
        Serial.print("X: ");Serial.print(iter->x);Serial.print(" ");
        Serial.print("Y: ");Serial.print(iter->y);Serial.print(" ");
        Serial.print("Z: ");Serial.println(iter->z);
        iter = iter->n;
    }
    Serial.println("");
}


// --------------- HueForce ----------------

class HueForce {

 public:
  Acceleration*
    axis;
  
  //QueueArray <Acceleration*>
  AxisQueue * lightWake;

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
  
  void mergeResults(void);
  void setAxis(void);
  
  void drawForce(void);
  void drawBrightness(float bright);
  
};

HueForce::HueForce(int force, int pixelCount, int pin) : delta(2), totalBrightness(0){
    modifier = float(255.0 / force);
    lighCount = pixelCount;
    lightPin = pin;
    lightWake = new AxisQueue(5);
    
    axis = new Acceleration;

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

void HueForce::setAxis(){
    sensors_event_t event; 
    accel.getEvent(&event);
    
    axis->x = int(abs(event.acceleration.x * modifier / SENSORS_GRAVITY_STANDARD));
    axis->y = int(abs(event.acceleration.y * modifier / SENSORS_GRAVITY_STANDARD));
    axis->z = int(abs(event.acceleration.z * modifier / SENSORS_GRAVITY_STANDARD));
   
    mergeResults();
}

void HueForce::mergeResults(){
    Acceleration *iter;
    int x=0, y=0, z=0;
    int queueSize;
    
    lightWake->addAxis(axis);
    int * average = lightWake->getAverage();
    
    axis->x = average[0];
    axis->y = average[1];
    axis->z = average[2];
    delete[] average;
}

void HueForce::drawBrightness(float bright){
      float newDelta;
      bright = abs(float(pow(float(bright),1.5)/16));
      bright = (bright * 1.1) - 25.5;
      bright = max(min(bright, 255), 10);
      
      
      if (totalBrightness > 250)
          newDelta = delta * 0.05;
      else
          newDelta = delta;
          
      if (bright < totalBrightness)
        totalBrightness -= newDelta;
      else
        totalBrightness = bright ;
      
      pixels.setBrightness(totalBrightness);
}

void HueForce::drawForce() {
    int red, blue, green;
    
    red = abs(int(axis->x));
    green = abs(int(axis->y));
    blue = abs(int(axis->z));

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
  if (DEBUG)
      while (!Serial) ;
  hueForce = new HueForce(G_FORCE, LIGHT_COUNT, LIGHT_PIN);
}


void loop() {
    hueForce->start();
    if (DEBUG)
        hueForce->displayStatus(hueForce->axis);
    delay(DELAY_VALUE);
}

