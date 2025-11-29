/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "time.hpp"

static double_t time_struct_get_freq();

time_struct::time_struct() : timestamp() {
    get_timestamp();
    inv_freq = time_struct_get_freq();
}

double_t time_struct::calc_time() {
#ifdef _WIN32
    LARGE_INTEGER timestamp;
    if (!QueryPerformanceCounter(&timestamp))
        timestamp.QuadPart = 0;

    return (double_t)(timestamp.QuadPart - this->timestamp.QuadPart) * inv_freq;
#else
    clock_t timestamp = clock();
    if (timestamp == -1)
        timestamp = 0;

    return (double_t)(timestamp - this->timestamp) * inv_freq;
#endif
}

double_t time_struct::calc_time_get_timestamp() {
#ifdef _WIN32
    LARGE_INTEGER timestamp;
    if (!QueryPerformanceCounter(&timestamp))
        timestamp.QuadPart = 0;

    double_t time = (double_t)(timestamp.QuadPart - this->timestamp.QuadPart) * inv_freq;
    this->timestamp = timestamp;
    return time;
#else
    clock_t timestamp = clock();
    if (timestamp == -1)
        timestamp = 0;

    double_t time = (double_t)(timestamp - this->timestamp) * inv_freq;
    this->timestamp = timestamp;
    return time;
#endif
}

int64_t time_struct::calc_time_int() {
#ifdef _WIN32
    LARGE_INTEGER timestamp;
    if (!QueryPerformanceCounter(&timestamp))
        timestamp.QuadPart = 0;

    return (int64_t)((double_t)(timestamp.QuadPart - this->timestamp.QuadPart) * inv_freq * 1000.0);
#else
    return (int64_t)((double_t)(timestamp - this->timestamp) * inv_freq * 1000.0);
#endif
}

int64_t time_struct::calc_time_int_get_timestamp() {
#ifdef _WIN32
    LARGE_INTEGER timestamp;
    if (!QueryPerformanceCounter(&timestamp))
        timestamp.QuadPart = 0;

    int64_t time = (int64_t)((double_t)(timestamp.QuadPart - this->timestamp.QuadPart) * inv_freq * 1000.0);
    this->timestamp = timestamp;
    return time;
#else
    clock_t timestamp = clock();
    if (timestamp == -1)
        timestamp = 0;

    int64_t time = (int64_t)((double_t)(timestamp - this->timestamp) * inv_freq * 1000.0);
    this->timestamp = timestamp;
    return time;
#endif
}

void time_struct::get_timestamp() {
#ifdef _WIN32
    if (!QueryPerformanceCounter(&timestamp))
        timestamp.QuadPart = 0;
#else
    timestamp = clock();
    if (timestamp == -1)
        timestamp = 0;
#endif
}

static double_t time_struct_get_freq() {
#ifdef _WIN32
    LARGE_INTEGER freq;
    if (QueryPerformanceFrequency(&freq))
        return 1000.0 / (double_t)freq.LowPart;
    return 0.0;
#else
    return 1000.0 / CLOCKS_PER_SEC;
#endif
}
