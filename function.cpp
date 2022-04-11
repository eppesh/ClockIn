#include "function.h"

namespace sean
{
unsigned int Function::GetNowDate()
{
#ifdef _WIN32
    SYSTEMTIME systemtime;
    GetLocalTime(&systemtime);

    return systemtime.wYear * 10000 + systemtime.wMonth * 100 + systemtime.wDay;
#else
    time_t now = time(NULL);
    struct tm tmp_tm;
    struct tm *local = localtime_r(&now, &tmp_tm);

    UINT32_T date = (1900 + local->tm_year) * 10000 + (local->tm_mon + 1) * 100 + local->tm_mday;
    return date;
#endif
}

// 获取当前日期是本周的第几天 1～7; eg: 若当天日期是Tuesday则返回2;
unsigned char Function::GetDayOfWeek()
{
    time_t now = time(NULL);    
    struct tm tmp_tm;
    int wday = 0;
#ifdef _WIN32
    localtime_s(&tmp_tm, &now);  // windows平台，与linux下的localtime_r的不同只表现在参数的顺序上；
    wday = tmp_tm.tm_wday;
#else
    struct tm *local;
    local = localtime_r(&now, &tmp_tm);
    wday = local->tm_wday;
#endif

    return wday;
}

// 获取指定日期是本周的第几天 1～7; eg: 若指定日期(date:20210418)是Sunday则返回7;
unsigned char Function::GetDayOfWeek(unsigned int date)
{
    time_t now = time(NULL); // 获取当前时间（从1970年至今的秒数）    
    struct tm tmp_tm, in_tm;
    int wday = 0;
#ifdef _WIN32
    localtime_s(&in_tm, &now);
    in_tm.tm_year = date / 10000 - 1900;
    in_tm.tm_mon = (date / 100) % 100 - 1;
    in_tm.tm_mday = date % 100;
    time_t t = mktime(&in_tm);
    localtime_s(&tmp_tm, &t);
    wday = tmp_tm.tm_wday;
#else
    struct tm *local;
    local = localtime_r(&now, &tmp_tm); // 将当前时间（秒数）填充到tm结构体中
    local->tm_year = date / 10000 - 1900;
    local->tm_mon = (date / 100) % 100 - 1;
    local->tm_mday = date % 100;
    time_t t = mktime(local); // 将tm结构的时间转换为从1970年至今的秒数
    local = localtime_r(&t, &tmp_tm);
    wday = local->tm_wday;
#endif
    return wday;
}

unsigned char Function::GetDayOfMonth()
{
    time_t now = time(NULL);
    struct tm tmp_tm;
    int mday = 0;
#ifdef _WIN32
    localtime_s(&tmp_tm, &now);  // windows平台，与linux下的localtime_r的不同只表现在参数的顺序上；
    mday = tmp_tm.tm_mday;
#else
    struct tm *local;
    local = localtime_r(&now, &tmp_tm);
    mday = local->tm_mday;
#endif

    return mday;
}

unsigned char Function::GetDayOfMonth(unsigned int date)
{
    time_t now = time(NULL); // 获取当前时间（从1970年至今的秒数）
    struct tm tmp_tm, in_tm;
    int mday = 0;
#ifdef _WIN32
    localtime_s(&in_tm, &now);
    in_tm.tm_year = date / 10000 - 1900;
    in_tm.tm_mon = (date / 100) % 100 - 1;
    in_tm.tm_mday = date % 100;
    time_t t = mktime(&in_tm);
    localtime_s(&tmp_tm, &t);
    mday = tmp_tm.tm_mday;
#else
    struct tm *local;
    local = localtime_r(&now, &tmp_tm); // 将当前时间（秒数）填充到tm结构体中
    local->tm_year = date / 10000 - 1900;
    local->tm_mon = (date / 100) % 100 - 1;
    local->tm_mday = date % 100;
    time_t t = mktime(local); // 将tm结构的时间转换为从1970年至今的秒数
    local = localtime_r(&t, &tmp_tm);
    mday = local->tm_mday;
#endif
    return mday;
}

unsigned int Function::GetMonthDays(unsigned int year, unsigned int month)
{
    int result = 0;
    int days[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (month == 2)
    {
        result = (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) ? 29 : 28;
    }
    else
    {
        result = days[month - 1];
    }
    return result;
}

unsigned int Function::CalculateBit(int n)
{
    int result = 0;
    unsigned int base = 0x1;
    for (int i = 0; i < 32; ++i)
    {
        int tmp = base << i;       // 每次左移1位
        if (n & tmp)
        {
            result++;            
        }
    }
    return result;
}

std::string Function::TcharToString(TCHAR *src)
{
#ifdef UNICODE    
    // Simple C
    // const size_t size = (wcslen(src) + 1) * sizeof(wchar_t);
    // wcstombs(&buffer[0], src, size);
    // std::vector<char> buffer(size);

    // Windows API
    std::vector<char> buffer;
    int size = WideCharToMultiByte(CP_UTF8, 0, src, -1, nullptr, 0, nullptr, nullptr);
    if (size > 0)
    {
        buffer.resize(size);
        WideCharToMultiByte(CP_UTF8, 0, src, -1, static_cast<char *>(&buffer[0]), buffer.size(), nullptr, nullptr);
    }
    else
    {
        // Error handling
    }
    std::string result(&buffer[0]);
#else
    std::string result(src);
#endif
    return result;
}


}