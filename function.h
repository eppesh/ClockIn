// 名称：function.h
// 版权：仅供自用
// 作者：Sean (eppesh@163.com)
// 环境：
// 时间：04/01/2022
// 说明：常用功能函数

#ifndef FUNCTION_H
#define FUNCTION_H

#include <ctime>
#include <windows.h>

namespace sean
{
class Function
{
public:
    static unsigned int GetNowDate();                               // 获取当前日期 19970101
    static unsigned char GetDayOfWeek();							// 获取当前日期是本周的第几天 1～7
    static unsigned char GetDayOfWeek(unsigned int date);			// 获取指定日期是本周的第几天 1～7
    static unsigned char GetDayOfMonth();							// 获取当前日期是本月的第几天 1～31
    static unsigned char GetDayOfMonth(unsigned int date);			// 获取指定日期是本月的第几天 1～31
    static unsigned int GetMonthDays(unsigned int year, unsigned int month);    // 获取指定的年月中的天数

    static unsigned int CalculateBit(int n);                        // 计算 n 的16进制中有几个1(4个字节中有几个1)
};
} // namespace

#endif