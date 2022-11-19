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

