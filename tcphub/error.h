#ifndef _ERROR_H_
#define _ERROR_H_

enum HUB_DEBUG {
    


#define error(level, format, ...)              \
    do {                                        \
        printf(format, ##__VA_ARGS__);          \
    } while (0)

static inline __attribute__ ((format (printf, 3, 4)))
void hub_dbg(enum HUB_DEBUG dbg_mask, const char *format, ...)
{
}

#endif // _ERROR_H_
