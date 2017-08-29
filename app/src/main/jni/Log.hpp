//
// Created by Brovko Roman on 29.08.17.
//

#ifndef DROIDBLASTER_LOG_HPP
#define DROIDBLASTER_LOG_HPP

class Log {
public:
    static void error(const char *pMessage, ...);
    static void warn(const char *pMessage, ...);
    static void info(const char *pMessage, ...);
    static void debug(const char *pMessage, ...);
};

#ifndef NDEBUG
    #define packt_log_debug(...) Log::debug(__VA_ARGS__)
#else
    #define packt_log_debug(...)
#endif

#endif //DROIDBLASTER_LOG_HPP
