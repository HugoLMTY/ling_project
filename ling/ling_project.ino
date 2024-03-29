#include "pitches.h"
#include <Adafruit_SSD1306.h>
#include <splash.h>

#define relayPin 2

#define displayModePin 7
#define selectorPin 10
#define triggerPin 11
#define buzzerPin 12

#define width 128 //* OLED display width,  in pixels
#define height 64 //* OLED display height, in pixels
Adafruit_SSD1306 oled(width, height, &Wire, -1);

//* GLOBALS
const bool showStats = false; //? Log or not stuff (Serial)
const int melody[] = {        //? Notes to play (buzzer)
    NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};

//* SETTINGS
unsigned long start = 0;     //? ms, Time at game start
const int dwell = 25;        //? ms, duration of each shot
bool triggerReleased = true; //? Used to detect press and hold

//* CONFIGURATION (user editable)
int displayMode = false; //? false = HUD, true = Crosshair

unsigned long gameDuration = 30000; //? ms, game duration
int mode = 1;
//? 0 SAFE
//? 1 SEMI
//? 2 BURST
//? 3 AUTO

int interval = 15;           //? ms, interval between shoots
int burstLength = 3;         //? number of firing each burst
bool allowBurstAuto = false; //? self explain; remove press and hold detection
//* Logs a title and it's value, very ez + beautiful spacer
void clog(String title, int val, bool spacer = false) {
    Serial.print(title);
    Serial.print(": ");
    Serial.print(val);

    if (spacer) {
        Serial.print(" - ");
    }
}
void cln() {
    Serial.println();
}

void setup() {
    Serial.begin(9600);
    pinMode(relayPin, OUTPUT);

    start = millis();

    if (displayMode) {
        markop_setup();
    } else {
        oled_setup();
        gyro_setup();
    }

    tone(buzzerPin, melody[7], 100);
}

void loop() {
    const bool triggered = digitalRead(triggerPin);        //? Allow both manual and photo stuff detection
    const bool selectorChanged = digitalRead(selectorPin); //? Toggle between modes (button)
    const bool displayChanged = digitalRead(displayModePin);

    if (displayChanged) {
        displayMode = !displayMode;
    }

    if (displayChanged) {
        //? Only draw the crosshair once when the display mode is changed
        if (displayMode) {
            markop_setup();
        } else {

            oled_setup();
            gyro_setup();
        }
    }

    //? Draw the HUD at each loop
    if (!displayMode) {
        if (selectorChanged) {
            handleSelector();
        }

        if (triggered) {
            handleTrigger();
        }

        drawGyro();
        drawTimer();
    }

    //? Resets the press and hold detection once the trigger is released
    if (!triggered && !triggerReleased) {
        triggerReleased = true;
    }

    //? Logs some stuff
    if (showStats) {
        const int msPerShot = ((dwell + interval) * burstLength) + interval;
        const double cps = (double)1000 / msPerShot;
        clog("Total ms", msPerShot, true);
        clog("CPS", cps, true);
        clog("Mode", mode);

        cln();
    }
}
