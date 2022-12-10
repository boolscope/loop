#include "loop.h"

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

