#include "loop.h"

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

