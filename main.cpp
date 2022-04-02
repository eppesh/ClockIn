// 名称：打卡
// 版权：仅供学习
// 作者：Sean (eppesh@163.com)
// 环境：VS2019;EasyX_20220116
// 时间：03/31/2022
// 说明：自用打卡工具

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