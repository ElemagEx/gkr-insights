#include <gkr/stamp.h>

#include <cstdio>

int main()
{
    auto stamp = gkr::stamp_now();

    struct tm tm;
    gkr::stamp_decompose(true, stamp, tm);

    std::printf("Now is %02d:%02d:%02d - %02d/%02d/%04d\n",
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        tm.tm_mday,
        tm.tm_mon  + 1,
        tm.tm_year + 1900
        );
    return 0;
}
