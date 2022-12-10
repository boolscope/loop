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

