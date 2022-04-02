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
    static unsigned int GetDayInMonth(unsigned int n);              // 获取当前月份的第n天
    static unsigned char GetDayOfWeek();							// 获取当前日期是本周的第几天 1～7
    static unsigned char GetDayOfWeek(unsigned int date);			// 获取指定日期是本周的第几天 1～7
    static unsigned char GetDayOfMonth();							// 获取当前日期是本月的第几天 1～31
    static unsigned char GetDayOfMonth(unsigned int date);			// 获取指定日期是本月的第几天 1～31
};
} // namespace

#endif