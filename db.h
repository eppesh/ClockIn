// 名称：db.h
// 版权：仅供学习
// 作者：Sean (eppesh@163.com)
// 环境：VS2019; LevelDB
// 时间：04/02/2022
// 说明：数据库

#ifndef DB_H_
#define DB_H_

#include <iostream>
#include <string>

#include "leveldb/db.h"
#include <graphics.h>

namespace clock_in
{
// 每个网格块的信息
// eg: 20220402 网格坐标(6,0) 当天完成的任务数4
struct GridInfo
{
    int date;                               // 网格对应的日期
    int x;                                  // 坐标点
    int y;                                  // 坐标点
    int tasks;                              // 该对应日期的任务完成数量
    GridInfo()
    {
        date = 0;
        x = 0;
        y = 0;
        tasks = 0;
    }
};

class DB
{
public:
    DB();
    ~DB();
    bool Put(GridInfo &grid_info);          // 将网格信息写入数据库
    bool Get(GridInfo &grid_info);          // 按日期获取网格信息
    bool GetFirstData(GridInfo &grid_info); // 获取数据库中第一条记录
    
private:
    leveldb::DB *db_;
    leveldb::Options options_;
    
};
}

#endif