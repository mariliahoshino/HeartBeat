//This project is a heart with beat sensor and blink the LEDs
//Licensed under MIT
//
// Marília Zeniti Hoshino
//mailto:mariliahoshino@hotmail.com
//https://www.mariliah.dev/
//https://linktr.ee/mariliah
//https://www.linkedin.com/in/mariliahoshino/
//https://github.com/mariliahoshino
//https://mariliahoshino.wixsite.com/home
//https://www.instagram.com/mari.zeniti/



#include <Wire.h>
#include "MAX30105.h"           //MAX3010x library  // https://surtrtech.com/2019/02/05/measure-heart-pulses-bpm-and-oxygen-saturation-spo%C2%B2-with-max30102-and-arduino/
#include "heartRate.h"          //Heart rate calculating algorithm

#include <FastLED.h>             //https://github.com/FastLED/FastLED
                                //https://howtomechatronics.com/tutorials/arduino/how-to-control-ws2812b-individually-addressable-leds-using-arduino/    example 1
# define LED_PIN 12           //WS2812b is connected to this pin
# define NUM_LEDS 5           //How many LEDs 
int x=0, startbeat=0;

const int buzzerPin = 4; //mini sound recorder board with a sound heartbeat recorded
const int ledPin1 = 13; // test

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;




CRGB leds [ NUM_LEDS ] ;
void setup () {  

  delay(3000);
  // Initialize sensor
  particleSensor.begin(Wire, I2C_SPEED_FAST); //Use default I2C port, 400kHz speed
  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running

  Serial.begin(9600);
  //while (!Serial);
  Serial.println(""); 
  Serial.println("HEART by Marília"); 
  //Setup pin modes
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);

  
  FastLED. addLeds < WS2812, LED_PIN, GRB > ( leds, NUM_LEDS ) ;

}
  
 
void loop () {  
   long irValue = particleSensor.getIR();    //Reading the IR value it will permit us to know if there's a finger on the sensor or not
                                           //Also detecting a heartbeat
                                           
    if(irValue > 7000)
    {                                           //If a finger is detected
       if (checkForBeat(irValue) == true)                        //If a heart beat is detected
       {

            if(startbeat < 2)
            {
              batida_inicio();
            }
            else
            {
              batida();
            }
          

           //We sensed a beat!
            long delta = millis() - lastBeat;                   //Measure duration between two beats
            lastBeat = millis();

             beatsPerMinute = 60 / (delta / 1000.0);           //Calculating the BPM

             if (beatsPerMinute < 255 && beatsPerMinute > 20)               //To calculate the average we strore some values (4) then do some math to calculate the average
             {
                 rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
                 rateSpot %= RATE_SIZE; //Wrap variable

                  //Take average of readings
                 beatAvg = 0;
                 for (byte x = 0 ; x < RATE_SIZE ; x++)
                 beatAvg += rates[x];
                 beatAvg /= RATE_SIZE;
              }
         }

       }
       if (irValue < 7000)
       {       //If no finger is detected it inform the user and put the average BPM to 0 or it will be stored for the next measure
          beatAvg=0;
          //noTone(3);
       }
}


void batida_inicio()
{
    for(x=0; x<=255; x=x+5)
    {
      leds [ 0 ] = CRGB ( x, 0, 0 ) ;   // ( R, G, B)
      FastLED. show () ;
    }
    for(x=0; x<=255; x=x+5)
    {
      leds [ 1 ] = CRGB ( x, 0, 0 ) ;
      FastLED. show () ;
    }
    for(x=0; x<=255; x=x+5)
    {
      leds [ 2 ] = CRGB ( x, 0, 0 ) ;
      FastLED. show () ;
    }
    for(x=0; x<=255; x=x+5)
    {
      leds [ 3 ] = CRGB ( x, 0, 0 ) ;
      FastLED. show () ;
    }
    for(x=0; x<=255; x=x+5)
    {
      leds [ 4 ] = CRGB ( x, 0, 0 ) ;
      FastLED. show () ;
    }

    for(x=255; x>=0; x=x-5)
    {
      leds [ 0 ] = CRGB ( x, 0, 0 ) ;   // ( R, G, B)
      FastLED. show () ;
    }
    for(x=255; x>=0; x=x-5)
    {
      leds [ 1 ] = CRGB ( x, 0, 0 ) ;
      FastLED. show () ;
    }
    for(x=255; x>=0; x=x-5)
    {
      leds [ 2 ] = CRGB ( x, 0, 0 ) ;
      FastLED. show () ;
    }
    for(x=255; x>=0; x=x-5)
    {
      leds [ 3 ] = CRGB ( x, 0, 0 ) ;
      FastLED. show () ;
    }
    for(x=255; x>=0; x=x-5)
    {
      leds [ 4 ] = CRGB ( x, 0, 0 ) ;
      FastLED. show () ;
    }   
    startbeat++;
}



void batida()
{
  digitalWrite(ledPin1, HIGH);
  digitalWrite(buzzerPin, HIGH);  //play audio Heartbeat
   //delay(duration);
  
  for(x=0; x<=255; x=x+5)
  {
    leds [ 0 ] = CRGB ( x, 0, 0 ) ;   // ( R, G, B)
    leds [ 1 ] = CRGB ( x, 0, 0 ) ;
    leds [ 2 ] = CRGB ( x, 0, 0 ) ;
    leds [ 3 ] = CRGB ( x, 0, 0 ) ;
    leds [ 4 ] = CRGB ( x, 0, 0 ) ;
    FastLED. show () ;
  }
  
  digitalWrite(ledPin1, LOW);
  digitalWrite(buzzerPin, LOW);

  for(x=255; x>=0; x=x-5)
  {
    leds [ 0 ] = CRGB ( x, 0, 0 ) ;
    leds [ 1 ] = CRGB ( x, 0, 0 ) ;
    leds [ 2 ] = CRGB ( x, 0, 0 ) ;
    leds [ 3 ] = CRGB ( x, 0, 0 ) ;
    leds [ 4 ] = CRGB ( x, 0, 0 ) ;
    FastLED. show () ;
  }
  delay(10);
  
}
