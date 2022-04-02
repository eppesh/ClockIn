// ���ƣ�clock_in.h
// ��Ȩ������ѧϰ
// ���ߣ�Sean (eppesh@163.com)
// ������VS2019;EasyX_20220116
// ʱ�䣺03/31/2022
// ˵�����������

#ifndef CLOCK_IN_H_
#define CLOCK_IN_H_

#include <iostream>
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
    void DrawGrid();                                // ������������
    void GetGridCoord();                        // ���ݵ�ǰ����ת��Ϊ�����е�����
    void GetGridCoord(int date);                // ��ָ������(20220401)ת��Ϊ�����е�����

private:
    std::vector<std::vector<int>> grid_;            // ����

    // ��������
    RECT rect_btn_health_;                          // health ��ť����
    RECT rect_btn_english_;                         // English ��ť����
    RECT rect_btn_program_;                         // program ��ť����
    RECT rect_btn_read_;                            // read ��ť����
    RECT rect_btn_reset_;                           // reset ��ť����(������������ʾ�������Ͻ�)

    // ������ز���
    int grid_rows_;                                 // ��������������
    int grid_columns_;
    int grid_block_size_;                           // �����л�ɫС������ߴ�
    int grid_step_;                                 // ���񲽳�:����ɫС������ߴ�+��ɫ��϶(10),��ÿ������������ߴ�
    int grid_x_;                                    // �������Ͻ�����
    int grid_y_;

    // �����е�ɫ��
    IMAGE img_block0_;                              // ���0�������Ӧ��ɫ��
    IMAGE img_block1_;                              // ���1�������Ӧ��ɫ��
    IMAGE img_block2_;                              // ���2�������Ӧ��ɫ��
    IMAGE img_block3_;                              // ���3�������Ӧ��ɫ��
    IMAGE img_block4_;                              // ���4�������Ӧ��ɫ��

    int week_index_;                                // ���µڼ���������;ȡֵ[0~5]
    int finished_tasks_;                            // ������ɵ��������;ȡֵ[0~4]
    bool is_health_finished_;                       // Health �Ƿ������
    bool is_english_finished_;                      // English �Ƿ������
    bool is_program_finished_;                      // Programming �Ƿ������
    bool is_read_finished_;                         // Reading �Ƿ������

    int now_date_;                                  // ��������
    POINT coordinate_;                              // �����еĵ�ǰ����(�뵱�������й�)
    DB db_;
};
} // namespace

#endif