/* This is an example of code for https://www.tinkercad.com.
 *
 * The library code was injoined to the demo code, since the
 * tinkercad doesn't have the ability to include external libraries.
 *
 * Use `make split` to split the code into  *.cpp and *.h files.
 * Don't remove the @file and @raw tags - they are necessary for
 * automatic file splitting.
 */

//

// @file loop.h
/* Loop
 * An asynchronous delay simulation library for microcontrollers.
 *
 *
 */
#ifndef _LOOP_H
#define _LOOP_H

#include <limits.h>

// Loop counts down the set time interval and reports its readiness.
//
// Required:
//      - limits.h
//
// Examples:
//      Loop l0 = Loop(2000);                //  2k millis (millis is default)
//      Loop l1 = Loop(5000, Loop::MICROS);  //  5k micros
//      Loop l2 = Loop(100, Loop::MILLIS);   // 100 millis
//      Loop l3 = Loop(1, Loop::SECONDS);    //   1 seconds
//	    ...
//	    if (s0.isReady()) { /* do something... */ }
//	    if (s1.isReady()) { /* do something... */ }
//      ...
class Loop {
   private:
    unsigned long timestamp = 0;  // the time of last readiness
    unsigned long interval = 0;   // the interval by microseconds
    unsigned int counter = 1;     // iterations counter

   public:
    // Units of time to set the interval.
    enum TimeUnits { MICROS,
                     MILLIS,
                     SECONDS };

    // Constructor.
    // @param i the interval by integer value.
    // @param u the units of time to set the interval.
    Loop(unsigned long i = 0, TimeUnits u = MILLIS);

    // Re-set a custom interval in microseconds/milliseconds.
    // Example:
    //      s0.setInterval(1000, Loop::MICROS);
    //      s1.setInterval(100, Loop::MILLIS);
    unsigned long setInterval(unsigned long i, TimeUnits u = MILLIS);

    // Returns true if the set time has expired
    // and executes refreshTime.
    bool isReady();

    // Set the countdown time to the current one.
    void refreshTime();

    // Reset the counter to the given value.
    void resetCounter(unsigned int v = 1);

    // Returns the current counter value.
    int getCounter();

    // Returns true if this is an even iteration index.
    bool isEven();

    // Returns true if this is an odd iteration index.
    bool isOdd();

    // Returns true if i is corresponds to the current iteration.
    // For example, returns true if this is the 10th iteration:
    //      l.isIter(10)
    bool isIter(int i);
};
#endif  // _LOOP_H

// @file loop.cpp
// @raw #include "loop.h"

// Constructor.
Loop::Loop(unsigned long i, TimeUnits u) {
    setInterval(i, u);
}

// Sets a custom interval by microseconds/milliseconds.
unsigned long Loop::setInterval(unsigned long i, TimeUnits u) {
    // Set the interval by microseconds always.
    interval = i * (u == MICROS ? 1 : (u == MILLIS ? 1000 : 1000000));
    refreshTime();
    resetCounter();

    return interval;
}

// Returns true if the interval is over.
// Addresses an issue when the microcontroller running for
// a long time and the micros method resets to zero.
// Also automatically performs refreshTime.
bool Loop::isReady() {
    bool result = false;
    unsigned long delta = micros() - timestamp;

    if (delta < 0) {
        // The micros method has been refreshed.
        // Note: include 'limits.h' to use ULONG_MAX.
        delta = ULONG_MAX - timestamp + (-1 * delta);
    }

    result = delta >= interval ? true : false;
    if (result)
        refreshTime();

    return result;
}

// Manual update countdown time.
// They are used in blocks that require long calculations,
// for example:
//      Loop s = Loop(250);     // 250 millis
//	    ...
//	    if (s.isReady()) {      // Auto refresh here.
//          /* long task ... */ // The time is probably
//                              // greater than the interval.
//          s.refreshTime();    // Manual refresh here.
//      }
void Loop::refreshTime() {
    timestamp = micros();

    // Reset to zero when the stack overflows.
    if (counter + 1 >= UINT_MAX) {
        counter = isEven() ? 1 : 2;
    } else {
        counter++;
    }
}

// Reset the counter to the given value.
void Loop::resetCounter(unsigned int v) {
    counter = v;
}

// Get curret iteration index.
int Loop::getCounter() {
    return counter;
}

// Returns true if this is an even iteration index.
bool Loop::isEven() {
    return counter % 2 == 0 ? true : false;
}

// Returns true if this is an odd iteration index.
bool Loop::isOdd() {
    return !isEven();
}

// Returns true if i is corresponds to the current iteration.
bool Loop::isIter(int i) {
    return i % 2 == 0 ? true : false;
}

// @file main.ino
// @raw #include "loop.h"

#define LED_RED 13
#define LED_GREEN 12
#define LED_YELLOW 11

Loop spanLedRed, spanLedGreen, spanLedYellow;

void setup() {
    Serial.begin(9600);

    spanLedRed = Loop(1, Loop::SECONDS);         // every second
    spanLedGreen = Loop(300, Loop::MILLIS);      // 300 milliseconds
    spanLedYellow = Loop(500000, Loop::MICROS);  // 500k microseconds

    spanLedRed.resetCounter(65000 - 5);

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
}

void loop() {
    if (spanLedRed.isReady()) {
        digitalWrite(LED_RED, spanLedRed.isEven() ? HIGH : LOW);

        Serial.print("Counter: ");
        Serial.println(spanLedRed.getCounter());
        Serial.println(UINT_MAX);
    }

    if (spanLedGreen.isReady()) {
        digitalWrite(LED_GREEN, spanLedGreen.isEven() ? HIGH : LOW);
    }

    if (spanLedYellow.isReady()) {
        digitalWrite(LED_YELLOW, spanLedYellow.isEven() ? HIGH : LOW);
    }
}