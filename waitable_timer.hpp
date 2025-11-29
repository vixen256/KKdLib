/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#pragma once

#include "default.hpp"

struct waitable_timer {
#ifdef _WIN32
    HANDLE handle;
#else
    int fd;
#endif

    inline waitable_timer() {
#ifdef _WIN32
        handle = CreateWaitableTimerW(0, 0, 0);
#else
        fd = timerfd_create(CLOCK_MONOTONIC, 0);
#endif
    }

    inline ~waitable_timer() {
#ifdef _WIN32
        if (handle) {
            CloseHandle(handle);
            handle = 0;
        }
#else
        if (fd > -1) {
            close(fd);
            fd = 0;
        }
#endif
    }

    inline void sleep(int64_t msec) {
        if (msec <= 0.0)
            return;

#ifdef _WIN32
        if (handle) {
            LARGE_INTEGER t;
            t.QuadPart = (LONGLONG)(msec * -10000);
            SetWaitableTimer(handle, &t, 0, 0, 0, 0);
            WaitForSingleObject(handle, INFINITE);
        }
        else {
            DWORD msec_dw = (DWORD)msec;
            if (msec_dw)
                Sleep(msec_dw);
        }
#else
        if (fd > -1) {
            struct itimerspec wait_time;
            wait_time.it_value.tv_sec = msec / 1000;
            wait_time.it_value.tv_nsec = msec % 1000 * 1000000;
            wait_time.it_interval.tv_sec = 0;
            wait_time.it_interval.tv_nsec = 0;
            timerfd_settime(fd, 0, &wait_time, nullptr);

            struct pollfd fds;
            fds.fd = fd;
            fds.events = POLLIN;
            fds.revents = 0;
            poll(&fds, 1, -1);
        } else {
            struct timespec wait_time;
            wait_time.tv_sec = msec / 1000;
            wait_time.tv_nsec = msec % 1000 * 1000000;
            nanosleep(&wait_time, nullptr);
        }
#endif
    }

    inline void sleep_float(double_t msec) {
        if (msec <= 0.0)
            return;

#ifdef _WIN32
        if (handle) {
            LARGE_INTEGER t;
            t.QuadPart = (LONGLONG)round(msec * -10000.0);
            SetWaitableTimer(handle, &t, 0, 0, 0, 0);
            WaitForSingleObject(handle, INFINITE);
        }
        else {
            DWORD msec_dw = (DWORD)round(msec);
            if (msec_dw)
                Sleep(msec_dw);
        }
#else
        if (fd > -1) {
            struct itimerspec wait_time;
            wait_time.it_value.tv_sec = msec / 1000;
            wait_time.it_value.tv_nsec = fmod(msec, 1000) * 1000000;
            wait_time.it_interval.tv_sec = 0;
            wait_time.it_interval.tv_nsec = 0;
            timerfd_settime(fd, 0, &wait_time, nullptr);

            struct pollfd fds;
            fds.fd = fd;
            fds.events = POLLIN;
            fds.revents = 0;
            poll(&fds, 1, -1);
        } else {
            struct timespec wait_time;
            wait_time.tv_sec = msec / 1000;
            wait_time.tv_nsec = fmod(msec, 1000) * 1000000;
            nanosleep(&wait_time, nullptr);
        }
#endif
    }
};
