# Loop

The Loop library provides the simplest tools for implementing asynchronous delays and timeouts in the runtime environment of microcontrollers.

## Idea

The delay() function, which is used to create timeouts, is a blocking function, that suspends the execution of the entire program for a specified time.

To simulate a timeout, we can use the built-in functions micros() or millis(), for example:

```cpp
int interval = 1000;
unsigned long timestamp = 0;
 
void setup() {
    timestamp = millis();
}
 
void loop() {
    if (millis() > timestamp + interval){
        // ... perform some action
        timestamp = millis();
    }
}
```

This library is a wrapper for the millis() function, for example:

```cpp
#include "loop.h"

Loop loop;
 
void setup() {
    loop.setInterval(1000);
}
 
void loop() {
    if (loop.isReady(true)){
        // ... perform some action
    }
}
```

In addition, the library allows you to determine, for example, whether the ready state was reached at least once `isNever()` method; whether it is an even/odd state of readiness of `isEven()` and `isOdd()` methods; the number of times the state of readiness was reached `getCount()` method, etc.

## Examples

### Segment Display with Async LED

In this example, there is an asynchronous LED that flashes with its frequency and a display segment that changes its value according to the presses of the Inc and Dec buttons. The Inc or Dec button can be pressed and the value will change every second and this is not reflected in the flicker frequency of the asynchronous LED.

Circuits with a demonstration is available at the link: [tinkercad](https://www.tinkercad.com/things/e4hsA8Pp3fF-segment-display-with-async-led)

*P.s. Pay attention to **Simulator time**, it may change during the simulation and may not correspond to real time.*


## Making changes to the code

Make changes to examples/example.inject.into and test it on tinkercad. Run make split && make doc and then create a pool request.