// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)
// Modified: Yingjie Qiao

#pragma once

#include <chrono>

namespace muduo {

using std::string;
using std::chrono::system_clock;

typedef std::chrono::nanoseconds Nanosecond;
typedef std::chrono::microseconds Microsecond;
typedef std::chrono::milliseconds Millisecond;
typedef std::chrono::seconds Second;
typedef std::chrono::minutes Minute;
typedef std::chrono::hours Hour;
typedef std::chrono::time_point<system_clock, Microsecond> Timestamp;

namespace clock {

inline Timestamp now() { return std::chrono::time_point_cast<Microsecond>(system_clock::now()); }
inline Timestamp addTime(Timestamp timestamp, double seconds) {
    return timestamp + std::chrono::duration<double, std::chrono::seconds>(seconds);
}

inline string toFormattedString(const Timestamp& t) {
    char buf[64] = {0};
    time_t seconds = static_cast<time_t>(t.time_since_epoch().count() / (1000 * 1000));
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);

    int microseconds = static_cast<int>(t.time_since_epoch().count() % (1000 * 1000));
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d", tm_time.tm_year + 1900,
             tm_time.tm_mon + 1, tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             microseconds);

    return buf;
}

inline string toString(const Timestamp& t) {
    char buf[32] = {0};
    int64_t seconds = t.time_since_epoch().count() / (1000 * 1000);
    int64_t microseconds = t.time_since_epoch().count() % (1000 * 1000);
    snprintf(buf, sizeof(buf) - 1,
             "%"
             "ld"
             ".%06"
             "ld"
             "",
             seconds, microseconds);
    return buf;
}

}  // namespace clock

}  // namespace muduo