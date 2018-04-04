#ifndef REPROMPIB_GLOBALCLOCK_CLASS_H_
#define REPROMPIB_GLOBALCLOCK_CLASS_H_

#include "Clock.h"

class GlobalClock: public Clock {

protected:
  Clock& local_clock;
  virtual double apply_clock_model(double timestamp) = 0;     // apply the clock model to a timestamp obtained from my local clock

public:
  GlobalClock(Clock& c);

  double get_local_time(void);
  double get_time(void);                                      // returns global time
  double convert_to_global_time(double local_timestamp);      // converts a local timestamp to a global time
  ~GlobalClock();

  virtual void print_clock_info();

};



#endif /*  REPROMPIB_GLOBALCLOCK_CLASS_H_  */
