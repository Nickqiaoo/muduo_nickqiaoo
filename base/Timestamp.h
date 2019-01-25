#pragma once

#include <chrono>

namespace muduo {

using std::chrono::system_clock;

typedef std::chrono::nanoseconds Nanosecond;
typedef std::chrono::microseconds Microsecond;
typedef std::chrono::milliseconds Millisecond;
typedef std::chrono::seconds Second;
typedef std::chrono::minutes Minute;
typedef std::chrono::hours Hour;
typedef std::chrono::time_point<system_clock, Microsecond> Timestamp;

namespace clock {

inline Timestamp now() { return system_clock::now(); }

}  // namespace clock

}  // namespace muduo