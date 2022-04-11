// 名称：clock_in.h
// 版权：仅供学习
// 作者：Sean (eppesh@163.com)
// 环境：VS2019;EasyX_20220116
// 时间：03/31/2022
// 说明：打卡相关类

#ifndef CLOCK_IN_H_
#define CLOCK_IN_H_

#include <iostream>
#include <regex>
#include <vector>

#include <graphics.h>

#include "db.h"
#include "function.h"

//#pragma comment(lib,"msimg32.lib")

namespace clock_in
{

class UI
{
public:
    UI();
    ~UI();
    void Draw();

    void Run();
    void Init();

private:
    void DrawGrid();                                // 绘制网格内容
    void GetGridCoord();                            // 根据当前日期转化为网格中的坐标
    POINT GetGridCoord(int date);                   // 获取指定日期(20220401)对应的网格中的坐标
    void GetGridData(int year, int month);          // 获取指定年月的网格数据
    void DrawUpdate();                              // 绘制一些需要重复更新的内容(如:点击之后要绘制等情况)
private:
    std::vector<std::vector<int>> grid_;            // 网格

    // 矩形区域
    RECT rect_btn_health_;                          // health 按钮区域
    RECT rect_btn_english_;                         // English 按钮区域
    RECT rect_btn_program_;                         // program 按钮区域
    RECT rect_btn_read_;                            // read 按钮区域
    RECT rect_btn_reset_;                           // reset 按钮区域(隐藏于网格显示区的左上角)
    RECT rect_btn_prev_;                            // previous 按钮区域(上一个月)
    RECT rect_btn_next_;                            // next 按钮区域(下一个月)
    RECT rect_month_;                               // 显示月份的区域
    RECT rect_btn_makeup_;                          // 补打卡按钮区域

    // 网格相关参数
    int grid_rows_;                                 // 网格行数与列数
    int grid_columns_;
    int grid_block_size_;                           // 网格中灰色小正方块尺寸
    int grid_step_;                                 // 网格步长:即灰色小正方块尺寸+白色空隙(10),是每个网格的真正尺寸
    int grid_x_;                                    // 网格左上角坐标
    int grid_y_;

    // 网格中的色块
    IMAGE img_block0_;                              // 完成0项任务对应的色块
    IMAGE img_block1_;                              // 完成1项任务对应的色块
    IMAGE img_block2_;                              // 完成2项任务对应的色块
    IMAGE img_block3_;                              // 完成3项任务对应的色块
    IMAGE img_block4_;                              // 完成4项任务对应的色块

    int week_index_;                                // 本月第几个星期列;取值[0~5]
    int finished_tasks_;                            // 当天完成的任务个数;取值[0~4]
    bool is_health_finished_;                       // Health 是否已完成
    bool is_english_finished_;                      // English 是否已完成
    bool is_program_finished_;                      // Programming 是否已完成
    bool is_read_finished_;                         // Reading 是否已完成

    int now_date_;                                  // 当天日期
    POINT coordinate_;                              // 网格中的当前坐标(与当天日期有关)
    DB db_;                                         // 数据库
    std::vector<std::string> months_;               // 月份对应关系; 1=Jan;2=Feb;
    int display_month_;                             // 当前正在显示的年月份 202204
};
} // namespace

#endif