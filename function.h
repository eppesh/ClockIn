// ���ƣ�function.h
// ��Ȩ����������
// ���ߣ�Sean (eppesh@163.com)
// ������
// ʱ�䣺04/01/2022
// ˵�������ù��ܺ���

#ifndef FUNCTION_H
#define FUNCTION_H

#include <ctime>
#include <windows.h>

namespace sean
{
class Function
{
public:
    static unsigned int GetNowDate();                               // ��ȡ��ǰ���� 19970101
    static unsigned int GetDayInMonth(unsigned int n);              // ��ȡ��ǰ�·ݵĵ�n��
    static unsigned char GetDayOfWeek();							// ��ȡ��ǰ�����Ǳ��ܵĵڼ��� 1��7
    static unsigned char GetDayOfWeek(unsigned int date);			// ��ȡָ�������Ǳ��ܵĵڼ��� 1��7
    static unsigned char GetDayOfMonth();							// ��ȡ��ǰ�����Ǳ��µĵڼ��� 1��31
    static unsigned char GetDayOfMonth(unsigned int date);			// ��ȡָ�������Ǳ��µĵڼ��� 1��31
};
} // namespace

#endif