#include <common/console.hpp>
#include <utilities/log/logfile.h>

#if defined(__WIIU__)
    #include <whb/log.h>
    #include <whb/log_cafe.h>
    #include <whb/log_console.h>
    #include <whb/log_udp.h>
#endif

LogFile::LogFile()
{
    this->file = fopen("love.log", "w");

#if defined(__WIIU__)
    WHBLogCafeInit();
    WHBLogUdpInit();
#endif
}

LogFile::~LogFile()
{
    fclose(this->file);
}

void LogFile::LogOutput(const char* func, size_t line, const char* format, ...)
{
    if (!this->file)
        return;

    std::unique_lock lock(this->mutex);

    va_list args;
    va_start(args, format);
    char buffer[255];

    vsnprintf(buffer, sizeof(buffer), format, args);

#if defined(__WIIU__)
    WHBLogPrintf(LOG_FORMAT, func, line, buffer);
#endif
    fprintf(this->file, LOG_FORMAT, func, line, buffer);
    fflush(this->file);
}