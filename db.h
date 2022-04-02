// ���ƣ�db.h
// ��Ȩ������ѧϰ
// ���ߣ�Sean (eppesh@163.com)
// ������VS2019; LevelDB
// ʱ�䣺04/02/2022
// ˵�������ݿ�

#ifndef DB_H_
#define DB_H_

#include <iostream>
#include <string>

#include "leveldb/db.h"
#include <graphics.h>

namespace clock_in
{
// ÿ����������Ϣ
// eg: 20220402 ��������(6,0) ������ɵ�������4
struct GridInfo
{
    std::string date;                   // �����Ӧ������
    POINT coord;                        // �����
    int tasks;                          // �ö�Ӧ���ڵ������������
};

class DB
{
public:
    DB();
    ~DB();
    bool Put(GridInfo &grid_info);      // ��������Ϣд�����ݿ�
    GridInfo Get(std::string key, bool &flag);      // �����ڻ�ȡ������Ϣ
    bool Get(GridInfo &grid_info);      // �����ڻ�ȡ������Ϣ
    
private:
    leveldb::DB *db_;
    leveldb::Options options_;
    
};
}

#endif