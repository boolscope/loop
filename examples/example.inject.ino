/* This is an example of code for https://www.tinkercad.com.
 *
 * The library code was attached to the demo code because
 * tinkercad doesn't have the ability to include external lib.
 *
 * Use `make split` to split the code into  *.cpp and *.h files.
 * Don't remove the @file and @raw tags - they are necessary for
 * automatic file splitting.
 */

// @file loop.h
/** An asynchronous delay simulation library.
 */
#ifndef _LOOP_H
#define _LOOP_H

#include <limits.h>

class Loop {
   private:
    // The number of times the loop object has been active.
    unsigned long count = 0;

    // The time after which the loop object becomes active.
    unsigned long interval = 0;

    // The last time the loop object was active or
    // when the object was initialized.
    unsigned long timestamp = 0;

   public:
    /** Constructor.
     * Allows to set the interval when creating an object.
     * @param[in] interval delay time in milliseconds.
     */
    Loop(unsigned long interval = 0);
    ~Loop() = default;

    /** Sets the time after which the loop object becomes active.
     * @param[in] interval delay time in milliseconds.
     * @return void
     */
    void setInterval(unsigned long interval);

    /** Returns the time after which the loop object becomes ready.
     * @return delay time in milliseconds.
     */
    unsigned long getInterval();

    /** Reset timestamp.
     * Assigns the current millis() value to the timestamp value.
     * @return void
     */
    void resetTime();

    /** Readiness check.
     * @param[in] reset if true, the timestamp indicator will be reset
     * automatically when the object reaches the ready state.
     *
     * @retval true if the set interval is exhausted and the object is ready;
     * @retval false otherwise.
     *
     * It is convenient for quick operations:
     * @code
     * if (loop.isReady(true)) {
     *     // a fast code here ...
     * }
     * @endcode
     *
     * But for complex code that requires a long time for its execution,
     * better in manual mode reset of the timestamp:
     * @code
     * if (loop.isReady()) {
     *   // a slow code here ...
     *   loop.resetTime();
     * }
     * @endcode
     */
    bool isReady(bool reset = false);

    /** Returns the number of times the loop object has been active.
     * @return number of times the loop object has been ready.
     */
    unsigned long getCount();

    /** Resets the count to zero.
     * @return void
     */
    void resetCount();

    /** Returns true if this is an even state of isReady.
     * The value depends on the value of the counter and
     * always returns false if the counter is 0.
     * @retval true if the counter is even.
     * @retval false otherwise.
     */
    bool isEven();

    /** Returns true if this is an odd state of isReady.
     * The value depends on the value of the counter and
     * always returns false if the counter is 0.
     * @retval true if the counter is odd.
     * @retval false otherwise.
     */
    bool isOdd();

    /** Checks if readiness has been reached at least once.
     * @retval true if the counter is 0 - indicates that
     * the object was never ready.
     * @retval false otherwise.
     */
    bool isNever();
};

#endif  // _LOOP_H

// @file loop.cpp
// @raw #include "loop.h"

// Constructor.
Loop::Loop(unsigned long interval) {
    setInterval(interval);
}

// Sets the time after which the loop object becomes active.
void Loop::setInterval(unsigned long interval) {
    this->interval = interval;
    this->resetTime();
}

// Returns the time after which the loop object becomes active.
unsigned long Loop::getInterval() {
    return this->interval;
}

// Assigns the current millis value to the timestamp value.
void Loop::resetTime() {
    this->timestamp = millis();
}

// Returns true if the loop object is active.
// Takes an optional reset argument. If it's true, when the object
// reaches the ready state, the timestamp indicator will be reset
// automatically. It is convenient for quick operations:
// if (loop.isReady(true)) { /* a fast code here ... */ }
//
// But for complex code that requires a long time for its execution,
// we need to manually reset of the timestamp:
// if (loop.isReady()) {
//   /* a long code here ... */
//   loop.resetTime();
// }
bool Loop::isReady(bool reset) {
    // The millis method resets to zero when the ULONG_MAX range is reached.
    // Therefore, if the last fixation of time was close to the moment of
    // resetting the delta can get a negative value with a very large range
    // close to -1 * ULONG_MAX.
    long delta = millis() - this->timestamp < 0
                     ? ULONG_MAX - this->timestamp + millis()
                     : millis() - this->timestamp;

    // If the loop object is active, then the count is incremented
    // and if reset is true - resets the timestamp.
    if (delta >= this->interval) {
        this->count++;
        if (reset)
            this->resetTime();

        return true;
    }

    return false;
}

// Returns the number of times the loop object has been active.
unsigned long Loop::getCount() {
    return this->count;
}

// Resets the count to zero.
void Loop::resetCount() {
    this->count = 0;
}

// Returns true if this is an even state of isReady.
// The value depends on the value of the counter and
// always returns false if the counter is 0.
bool Loop::isEven() {
    return this->count % 2 == 0 && this->count != 0;
}

// Returns true if this is an odd state of isReady.
// The value depends on the value of the counter and
// always returns false if the counter is 0.
bool Loop::isOdd() {
    return this->count % 2 != 0 && this->count != 0;
}

// Returns true if the counter is 0 - indicates that
// the object was never ready.
bool Loop::isNever() {
    return this->count == 0;
}

// @file example.ino
// @raw #include "loop.h"

/* Attiny interface simulator for use with Arduino.
#define PB0 4
#define PB1 3
#define PB2 2
#define PB3 6
#define PB4 5
#define PB5 7
//*/

// An example of asynchronous reading of a pushbutton
// (with switch bounce processing) and independent
// flickering of an LED.

// Shift register.
// The 74HC595 shift register is used to control the segment display.
#define SH_INPUT_PIN PB3
#define SH_CLOCK_PIN PB1
#define SH_O_CLOCK_PIN PB2

// LED pin.
#define RED_LED PB0

// Buttons pin.
#define INC_BUTTON_PIN PB5
#define DEC_BUTTON_PIN PB4

// Globals.
bool ledOn = true;
bool dotOn = false;
bool incIsPressed = false;
bool decIsPressed = false;
char lastNumber = 5;
Loop ledLoop, incLoop, decLoop;
Loop incPressLoop, decPressLoop;

// Converts a number to a set of segments for a segment display.
// Controls the dot using the bool dot value.
char number(char n, bool dot) {
    char value = 0;
    char segments[11] = {
        0b11111100,  // 0
        0b01100000,  // 1
        0b11011010,  // 2
        0b11110010,  // 3
        0b01100110,  // 4
        0b10110110,  // 5
        0b10111110,  // 6
        0b11100000,  // 7
        0b11111110,  // 8
        0b11100110,  // 9
        0b10011110,  // E - ERROR
    };

    // Get segment value.
    value = n > 9 ? segments[10] : segments[n];

    // To manage bits, use the following logic, where id
    // is the bit identifier from right to left from 0 to 7,
    // and use 1ULL if number is wider than unsigned long:
    // Set a bit:  n |= 1UL << id;
    // Clear a bit: n &= ~(1UL << id);
    // Toggle a bit: n ^= 1UL << id;
    return dot ? value | 1UL << 0 : value;
}

// Displays the specified number on the segment display
// or E (error) if the result cannot be shown on the
// segment display.
void echo(char n, bool dot) {
    // Prepare CI to recive data.
    digitalWrite(SH_O_CLOCK_PIN, LOW);

    // LSBFIRST - normal order;
    // MSBFIRST - inverse order;
    shiftOut(SH_INPUT_PIN, SH_CLOCK_PIN, LSBFIRST, number(n, dot));

    // Inform CI to applay data.
    digitalWrite(SH_O_CLOCK_PIN, HIGH);
}

// Settings.
void setup() {
    // Shift register.
    pinMode(SH_INPUT_PIN, OUTPUT);
    pinMode(SH_CLOCK_PIN, OUTPUT);
    pinMode(SH_O_CLOCK_PIN, OUTPUT);

    // Async LED.
    pinMode(RED_LED, OUTPUT);
    ledLoop = Loop(1000);  // LED lights up and blinks every second

    // Buttons.
    pinMode(INC_BUTTON_PIN, INPUT);
    pinMode(DEC_BUTTON_PIN, INPUT);
    incLoop = Loop(32);  // prevent bounce for inc button
    decLoop = Loop(32);  // prevent bounce for dec button
    incPressLoop = Loop(1000);
    decPressLoop = Loop(1000);
}

// Loop.
void loop() {
    char number = lastNumber;

    // Async LED.
    // The LED is turned on and off every ledLoop interval.
    if (ledLoop.isReady(true)) {
        digitalWrite(RED_LED, (ledOn = !ledOn) ? HIGH : LOW);
    }

    // Increment button.
    // Polls the button every incLoop interval, protects against bounce.
    if (incLoop.isReady()) {
        if (digitalRead(INC_BUTTON_PIN) == HIGH) {
            if (number < 9 && (incPressLoop.isReady(true) || !incIsPressed))
                number++;
            incIsPressed = true;
        } else {
            incIsPressed = false;
        }

        // The code may have some delay, so we reset the timer manually.
        incLoop.resetTime();
    }

    // Decrement button.
    // Polls the button every decLoop interval, protects against bounce.
    if (decLoop.isReady()) {
        if (digitalRead(DEC_BUTTON_PIN) == HIGH) {
            if (number >= 0 && (decPressLoop.isReady(true) || !decIsPressed))
                number--;
            decIsPressed = true;
        } else {
            decIsPressed = false;
        }

        // The code may have some delay, so we reset the timer manually.
        decLoop.resetTime();
    }

    if (number != lastNumber || ledLoop.isNever() || dotOn) {
        dotOn = incIsPressed || decIsPressed;
        echo(number, dotOn);
        lastNumber = number;
    }
}