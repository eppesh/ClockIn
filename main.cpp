// ���ƣ���
// ��Ȩ������ѧϰ
// ���ߣ�Sean (eppesh@163.com)
// ������VS2019;EasyX_20220116
// ʱ�䣺03/31/2022
// ˵�������ô򿨹���

#include <iostream>
#include <thread>

#include "clock_in.h"

using namespace clock_in;

int main()
{
    UI ui;
    ui.Draw();

    std::thread listener(&UI::Run, &ui);
    listener.detach();

    //while (true)
    {
        
    }

    system("pause");
    closegraph();
    return 0;
}