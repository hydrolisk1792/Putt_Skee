//Written under the authority of AE&ES (Gary Bordman) by Electcorp Industries 2014
//and Torlak Control Centre Systems.
//Program Version 1.11524.5KARI

#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define topMatrixPin 27     //Top Matrix Data Pin
#define botMatrixPin 14     //Bottom Matrix Data Pin

#define matrixWidth 32     //Define the total length of a matrix module
#define matrixHeight 8     //Define total height of a matrix module

//Define Outputs:
#define ballsLED 4     //Define balls LED Pin
#define clockLED  2     //Define Clock LED pin
//End Define Outputs

//Define Triggers:
#define points50Pin 23     //Define 50 points trigger pin
#define points40Pin 22     //Define 40 points trigger pin
#define points30Pin 21     //Define 30 points trigger pin
#define points20Pin 19     //Define 20 points trigger pin
#define points10Pin 18     //Define 10 points trigger pin
#define zeroTrigger 15     //Define 0 points trigger pin
//End Triggers

//Define Buttons:
#define startPin 5     //Define Start Button Pin
#define resetPin 17     //Define Reset Button Pin
#define clockPin 16     //Define Settings Button Pin
//End Buttons

//System Objects:
Adafruit_NeoMatrix topMatrix(matrixWidth, matrixHeight, topMatrixPin, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);
Adafruit_NeoMatrix botMatrix(matrixWidth, matrixHeight, botMatrixPin, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG, NEO_GRB + NEO_KHZ800);
//End Objects

//System Integers:
int startState = 0;     //State for Start Button
int resetState = 0;     //State for Reset Button
int clockState = 0;     //State for Settings Button
int points50State = 0;     //State for 50 points trigger
int points40State = 0;     //State for 40 points trigger
int points30State = 0;     //State for 30 points Trigger
int points20State = 0;     //State for 20 points Trigger
int points10State = 0;     //State for 10 points Trigger
int zeroTriggerState = 0;     //State for 0 trigger
int ballNum = 9;     //Saves the state of balls used in ball play
int points = 0;     //Saves current score and transfers to hi score
int hiPoints = 0;     //Saves the high score when made
int triggerDebounce = 750;     //Set Debounce time for Trigger Switches
//End System Integers

//System Timers:
unsigned long tMinus = 31000;     //Set the timer at a rational number
unsigned long timerStartTime = 0;     //Saves the millis() of start time
unsigned long prevAni = 0;     //Saves the previous state of previous animation clock millis()
const long intervalAni = 750;     //Constant integer for the flash switching interval of idle animations
const long exitTime = 5000;     //Interval at which the menu will automatically exit when idle
//End System Timers

//System Flags:
boolean aniFlag = false;     //Animation dummy flag
boolean timerFlag = true;     //Flag to keep track of game mode selected
boolean exitFlag = false;     //Flag to keep track of Exit Menu Timer
//End System Flags

//FSM:
enum states {clearAll, idle, gameStart, gamePlay, setTimer, gameOver} states = clearAll;     //Main State Machine function
//end FSM

//System Switches:
byte idleAni = 0;     //FSM for idle animations
byte gameStartSwitch = 0;     //FSM for keeping track of game start up
byte timerSwitch = 2;     //FSM for keeping track of setting the clock or nine ball mode
byte gamePlaySwitch = 0;     //FSM for keeping track of current game play
byte gameOverSwitch = 0;     //FSM for keeping track of the game over clause
byte hiScoreSwitch = 0;     //FSM for keeping track of the high score handler
//End System Switches

//RNG:
byte randR;     //Random number generator for idle animations colours (Red)
byte randG;     //Random number generator for idle animations colours (Green)
byte randB;     //Random number generator for idle animations colours (Blue)
//End RNG

//System Prototypes:
void idleAnimations(byte r, byte g, byte b);
void gameStartHandler();
void gamePlayHandler();
void scoringHandler(boolean b);
void gameOverHandler();
void matrixWrite(boolean p, byte c, byte d, String t, byte r, byte g, byte b);
void resetScreen();
void setTimerHandler();
//End System Prototypes




//Setup:
void setup()
{
  
  //Outputs Setup:
  pinMode(ballsLED, OUTPUT);
  pinMode(clockLED, OUTPUT);
  //End Outputs Setup
  
  //Trigger pins setup:
  pinMode(points50Pin, INPUT_PULLUP);
  pinMode(points40Pin, INPUT_PULLUP);
  pinMode(points30Pin, INPUT_PULLUP);
  pinMode(points20Pin, INPUT_PULLUP);
  pinMode(points10Pin, INPUT_PULLUP);
  pinMode(zeroTrigger, INPUT_PULLUP);
  //End Trigger Pins Setup

  //Buttons Setup:
  pinMode(startPin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  pinMode(clockPin, INPUT_PULLUP);
  //End Buttons Setup
  
  //Setup Matrix:
  topMatrix.begin();
  botMatrix.begin();
  topMatrix.setTextWrap(false);
  botMatrix.setTextWrap(false);
  topMatrix.setBrightness(10);
  botMatrix.setBrightness(10);
  topMatrix.setTextSize(1);
  botMatrix.setTextSize(1);
  topMatrix.clear();
  botMatrix.clear();
  topMatrix.show();
  botMatrix.show();
  //End Setup Matrix

  //Startup:
  topMatrix.fillScreen(0);
  topMatrix.setTextColor(topMatrix.Color(0, 0, 255));
  topMatrix.setCursor(1, 0);
  topMatrix.print("AE&ES");
  topMatrix.show();
  botMatrix.fillScreen(0);
  botMatrix.setTextColor(topMatrix.Color(0, 0, 255));
  botMatrix.setCursor(1, 0);
  botMatrix.print("E.C.I.");
  botMatrix.show();
  delay(2000);

  for(int i = 4; i > 0; i--)
  {
    topMatrix.fillScreen(0);
    topMatrix.setTextColor(topMatrix.Color(255, 0, 0));
    topMatrix.setCursor(1, 0);
    topMatrix.print("PUTT");
    topMatrix.show();

    botMatrix.fillScreen(0);
    botMatrix.setTextColor(topMatrix.Color(0, 0, 255));
    botMatrix.setCursor(1, 0);
    botMatrix.print(" SKEE");
    botMatrix.show();
    delay(750);
    topMatrix.clear();
    topMatrix.show();
    botMatrix.clear();
    botMatrix.show();
    delay(750);
  }
  //End Startup

  randomSeed(analogRead(33));
}
//End Setup




//Loop:
void loop()
{
  //Declerations:
  static unsigned long exitMillis = millis();
  static unsigned long resetMillis = millis();
  static unsigned long points10Millis = millis();
  static unsigned long clockMillis = millis();
  static unsigned long startMillis = millis();
  unsigned long currentAni = millis();
  startState = digitalRead(startPin);
  resetState = digitalRead(resetPin);
  clockState = digitalRead(clockPin);
  points50State = digitalRead(points50Pin);
  points40State = digitalRead(points40Pin);
  points30State = digitalRead(points30Pin);
  points20State = digitalRead(points20Pin);
  points10State = digitalRead(points10Pin);
  zeroTriggerState = digitalRead(zeroTrigger);
  //End Declerations

  //Sign Indicator for either ball play or timer play:
  if(!timerFlag)
  {
    digitalWrite(ballsLED, HIGH);
    digitalWrite(clockLED, LOW);
  }
  else
  {
    digitalWrite(ballsLED, LOW);
    digitalWrite(clockLED, HIGH);
  }
  //End Sign Indicators

  if(millis() - exitMillis > exitTime)
      {
        if(exitFlag)
        {
          states = clearAll;
        }
      }
  
  switch(states)
  {
    case clearAll:
      idleAni = 0;
      gameStartSwitch = 0;
      ballNum = 9;
      gamePlaySwitch = 0;
      gameOverSwitch = 0;
      hiScoreSwitch = 0;
      exitFlag = false;
      exitMillis = millis();
      delay(1);
      states = idle;
      break;

    case idle:
      idleAnimations(randR, randG, randB);

      if(startState == LOW)
      {
        if(millis() - startMillis > 250)
        {
          states = gameStart;
        }
        startMillis = millis();
      }
      
      if(clockState == LOW)
      {
        if(millis() - clockMillis > 250)
        {
          if(!exitFlag)
          {
            exitFlag = true;
          }
          exitMillis = millis();
          states = setTimer;
        }
        clockMillis = millis();
      }
      
      if(currentAni - prevAni >= intervalAni)
      {
        prevAni = currentAni;
        if(!aniFlag)
        {
          aniFlag = true;
          idleAni ++;
          randR = random(100, 255);
          randG = random(100, 255);
          randB = random(100, 255);
          if(idleAni == 6)
          {
            idleAni = 0;
          }
        }
        else
        {
          aniFlag = false;
        }
      }
      break;

    case gameStart:
      gameStartHandler();
      break;

    case gamePlay:
      gamePlayHandler();
      break;

    case setTimer:
      setTimerHandler();
      
      if(clockState == LOW)
      {
        if(millis() - clockMillis > 250)
        {
          if(!exitFlag)
          {
            exitFlag = true;
          }
          exitMillis = millis();
          timerSwitch ++;
          if(timerSwitch == 6)
          {
            timerSwitch = 0;
          }
        }
        clockMillis = millis();
      }
      break;

    case gameOver:
      gameOverHandler();
      break;

    default:
      states = clearAll;
      return;
  }
}
//End Loop
