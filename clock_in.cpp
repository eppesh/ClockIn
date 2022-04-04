#include "clock_in.h"

namespace clock_in
{
UI::UI() :grid_rows_(7), grid_columns_(6), grid_block_size_(30), grid_step_(30 + 10),
grid_x_(20 + 150), grid_y_(260 + 40), week_index_(0), finished_tasks_(0),
is_health_finished_(false), is_english_finished_(false), is_program_finished_(false),
is_read_finished_(false)
{
    Init();
}

UI::~UI()
{

}

void UI::Init()
{
    //sean::Function::CalculateBit(3);  // Test

    rect_btn_health_ = { 280, 40, 280 + 120, 40 + 40 };
    rect_btn_english_ = { 280, 90, 280 + 120, 90 + 40 };
    rect_btn_program_ = { 280, 140, 280 + 120, 140 + 40 };
    rect_btn_read_ = { 280, 190, 280 + 120, 190 + 40 };
    rect_btn_reset_ = { 30, 270, 30 + 80, 310 };

    grid_.resize(grid_rows_, std::vector<int>(grid_columns_, 0));
    
    coordinate_ = { 0,0 };
    GetGridCoord();             // 得到当天的坐标

    now_date_ = sean::Function::GetNowDate();
    // 从数据库中获取当前月份截止到今天之间的所有网格信息
    int first_date = sean::Function::GetDayInMonth(1);  // 该月的第1天    
    // 获取历史日期的网格信息
    
    for (int i = first_date; i <= now_date_; ++i)
    {
        GridInfo grid_info;
        grid_info.date = i;
        if (db_.Get(grid_info))
        {
            grid_[grid_info.x][grid_info.y] = grid_info.tasks;
            // 如果是当天打卡后重启,需恢复已打卡的状态
            if (i == now_date_)
            {
                finished_tasks_ = grid_info.tasks;
                // 还需要知道是哪些事项打过卡了：TODO: 用位来表示
                // 打上勾+不能重复打卡
                if (grid_info.tasks & 0x1000)
                {
                    is_health_finished_ = true;
                }
                if (grid_info.tasks & 0x0100)
                {
                    is_english_finished_ = true;
                }
                if (grid_info.tasks & 0x0010)
                {
                    is_program_finished_ = true;
                }
                if (grid_info.tasks & 0x0001)
                {
                    is_read_finished_ = true;
                }
            }
        }
    }


    // 网格中的灰色小方块:使用由浅到深的颜色的IMAGE来表示
    img_block0_.Resize(grid_block_size_, grid_block_size_);
    SetWorkingImage(&img_block0_);
    setfillcolor(RGB(235, 237, 240));
    solidrectangle(0, 0, img_block0_.getwidth() - 1, img_block0_.getheight() - 1);
    img_block1_.Resize(grid_block_size_, grid_block_size_);
    SetWorkingImage(&img_block1_);
    setfillcolor(RGB(155, 233, 168));
    solidrectangle(0, 0, img_block1_.getwidth() - 1, img_block1_.getheight() - 1);
    img_block2_.Resize(grid_block_size_, grid_block_size_);
    SetWorkingImage(&img_block2_);
    setfillcolor(RGB(64, 196, 99));
    solidrectangle(0, 0, img_block2_.getwidth() - 1, img_block2_.getheight() - 1);
    img_block3_.Resize(grid_block_size_, grid_block_size_);
    SetWorkingImage(&img_block3_);
    setfillcolor(RGB(48, 161, 78));
    solidrectangle(0, 0, img_block3_.getwidth() - 1, img_block3_.getheight() - 1);
    img_block4_.Resize(grid_block_size_, grid_block_size_);
    SetWorkingImage(&img_block4_);
    setfillcolor(RGB(33, 110, 57));
    solidrectangle(0, 0, img_block4_.getwidth() - 1, img_block4_.getheight() - 1);
    SetWorkingImage(NULL);

}



void UI::Draw()
{
    initgraph(460, 650, EW_SHOWCONSOLE);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    cleardevice();

    // 打卡区域
    setfillcolor(RGB(242, 242, 241));
    solidroundrect(20, 30, 440, 240, 10, 10);
    // 任务名称
    settextstyle(32, 0, _T("Times New Roman"));
    settextcolor(0x9eaebb);
    RECT task_health = { 60, 40, 60 + 200, 40 + 40 };
    drawtext(_T("Health"), &task_health, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    setlinestyle(PS_DASH);
    setlinecolor(LIGHTGRAY);
    line(60, 90, 60 + 180, 90);
    RECT task_en = { 60, 90, 60 + 200, 90 + 40 };
    drawtext(_T("English"), &task_en, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    line(60, 140, 60 + 180, 140);
    RECT task_program = { 60, 140, 60 + 200, 140 + 40 };
    drawtext(_T("Programming"), &task_program, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    line(60, 190, 60 + 180, 190);
    RECT task_read = { 60, 190, 10 + 280, 190 + 40 };
    drawtext(_T("Reading"), &task_read, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    
    // 打卡按钮
    setfillcolor(RGB(18, 150, 219));
    settextcolor(WHITE);
    solidroundrect(rect_btn_health_.left, rect_btn_health_.top, rect_btn_health_.right, rect_btn_health_.bottom, 8, 8);
    drawtext(_T("Clock In"), &rect_btn_health_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    solidroundrect(rect_btn_english_.left, rect_btn_english_.top, rect_btn_english_.right, rect_btn_english_.bottom, 8, 8);
    drawtext(_T("Clock In"), &rect_btn_english_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    solidroundrect(rect_btn_program_.left, rect_btn_program_.top, rect_btn_program_.right, rect_btn_program_.bottom, 8, 8);
    drawtext(_T("Clock In"), &rect_btn_program_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    solidroundrect(rect_btn_read_.left, rect_btn_read_.top, rect_btn_read_.right, rect_btn_read_.bottom, 8, 8);
    drawtext(_T("Clock In"), &rect_btn_read_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    
    // 重置按钮
    setfillcolor(RGB(235, 237, 240));
    solidroundrect(rect_btn_reset_.left, rect_btn_reset_.top, rect_btn_reset_.right, rect_btn_reset_.bottom, 8, 8);
    drawtext(_T("Reset"), &rect_btn_reset_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 统计内容显示区域
    setfillcolor(WHITE);
    setlinecolor(RGB(238, 238, 242));
    roundrect(20, 260, 440, 630, 10, 10);
    

    // 绘制网格    
    DrawGrid();

    // 网格左侧的星期提示
    /*setfillcolor(RGB(18, 150, 219));
    solidroundrect(280, 40, 280 + 120, 40 + 40, 8, 8);*/
    settextcolor(BLACK);
    RECT rect_month = { grid_x_ + 2 * grid_step_, grid_y_ - grid_step_, grid_x_ + 3 * grid_step_ + grid_block_size_, grid_y_ };
    drawtext(_T("Mar"), &rect_month, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    RECT rect_monday = { 60, grid_y_ + grid_step_, 60 + 60, grid_y_ + grid_step_ + grid_block_size_ };
    drawtext(_T("Mon"), &rect_monday, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    RECT rect_wednesday = { 60, grid_y_ + 3 * grid_step_, 60 + 60, grid_y_ + 3 * grid_step_ + grid_block_size_ };
    drawtext(_T("Wed"), &rect_wednesday, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    RECT rect_friday = { 60, grid_y_ + 5 * grid_step_, 60 + 60, grid_y_ + 5 * grid_step_ + grid_block_size_ };
    drawtext(_T("Fri"), &rect_friday, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        
    // 颜色说明(即打卡事项越多颜色越深)
    int test_x = grid_x_ + 10;
    int test_y = grid_y_ + 7 * grid_step_ + grid_block_size_ - 12;
    outtextxy(test_x - 60, test_y, _T("Less"));
    putimage(test_x, test_y, &img_block0_);
    putimage(test_x + grid_block_size_ + 10, test_y, &img_block1_);
    putimage(test_x + 2 * grid_block_size_ + 20, test_y, &img_block2_);
    putimage(test_x + 3 * grid_block_size_ + 30, test_y, &img_block3_);
    putimage(test_x + 4 * grid_block_size_ + 40, test_y, &img_block4_);
    outtextxy(test_x + 5 * grid_block_size_ + 50, test_y, _T("More"));

    // 若当天已部分打过卡,则显示
    if (is_health_finished_)
    {
        outtextxy(30, 50, _T("√"));
    }
    if (is_english_finished_)
    {
        outtextxy(30, 100, _T("√"));
    }
    if (is_program_finished_)
    {
        outtextxy(30, 150, _T("√"));
    }
    if (is_read_finished_)
    {
        outtextxy(30, 200, _T("√"));
    }
}

void UI::Run()
{
    ExMessage msg;
    while (true)
    {
        msg = getmessage(EM_MOUSE | EM_KEY);

        POINT mouse = { msg.x,msg.y };

        switch (msg.message)
        {
        case WM_LBUTTONDOWN:
        {
            if (PtInRect(&rect_btn_health_, mouse))
            {
                // TODO:  跨天要重置,暂时不支持
                if (!is_health_finished_)
                {
                    // 打卡响应
                    /*TCHAR health_notes[1024] = { 0 };
                    InputBox(health_notes, 1024, _T("Input the daily report about 'Health'."));*/
                    /*finished_tasks_++;
                    finished_tasks_ %= 5;*/
                    finished_tasks_ |= 0x1000;
                    is_health_finished_ = true;
                    outtextxy(30, 50, _T("√"));
                    //date_ = _wtoi(health_notes);

                    //POINT coord = GetCoordFromDate(date_);
                    /*grid_[coord.x][coord.y] = finished_tasks_;
                    DrawGrid();*/
                }
                
            }
            else if (PtInRect(&rect_btn_english_, mouse))
            {
                if (!is_english_finished_)
                {
                    /*TCHAR english_notes[1024] = { 0 };
                    InputBox(english_notes, 1024, _T("Input the daily report about 'English'."));*/
                    /*finished_tasks_++;
                    finished_tasks_ %= 5;*/
                    finished_tasks_ |= 0x0100;
                    is_english_finished_ = true;
                    outtextxy(30, 100, _T("√"));

                    /*POINT coord = GetCoordFromDate(date_);
                    grid_[coord.x][coord.y] = finished_tasks_;
                    DrawGrid();*/
                }
            }
            else if (PtInRect(&rect_btn_program_, mouse))
            {
                if (!is_program_finished_)
                {
                    /*TCHAR program_notes[1024] = { 0 };
                    InputBox(program_notes, 1024, _T("Input the daily report about 'Programming'."));*/
                    /*finished_tasks_++;
                    finished_tasks_ %= 5;*/
                    finished_tasks_ |= 0x0010;
                    is_program_finished_ = true;
                    outtextxy(30, 150, _T("√"));

                    /*POINT coord = GetCoordFromDate(date_);
                    grid_[coord.x][coord.y] = finished_tasks_;
                    DrawGrid();*/
                }
            }
            else if (PtInRect(&rect_btn_read_, mouse))
            {
                if (!is_read_finished_)
                {
                    /*TCHAR read_notes[1024] = { 0 };
                    InputBox(read_notes, 1024, _T("Input the daily report about 'Reading'."));*/
                    /*finished_tasks_++;
                    finished_tasks_ %= 5;*/
                    finished_tasks_ |= 0x0001;
                    is_read_finished_ = true;
                    outtextxy(30, 200, _T("√"));

                    /*POINT coord = GetCoordFromDate(date_);
                    grid_[coord.x][coord.y] = finished_tasks_;
                    DrawGrid();*/
                    /*DB db;
                    db.Test();*/
                }
            }
            else if (PtInRect(&rect_btn_reset_, mouse))
            {
                if (is_health_finished_ || is_english_finished_ || is_program_finished_ || is_read_finished_)
                {
                    is_health_finished_ = false;
                    is_english_finished_ = false;
                    is_program_finished_ = false;
                    is_read_finished_ = false;
                    finished_tasks_ = 0;
                    setfillcolor(RGB(242, 242, 241));
                    solidrectangle(28, 50, 60, 235);
                }
            }
            grid_[coordinate_.x][coordinate_.y] = finished_tasks_;
            DrawGrid();
            // 将数据更新到数据库
            GridInfo grid_info;
            grid_info.date = now_date_;
            grid_info.x = coordinate_.x;
            grid_info.y = coordinate_.y;
            grid_info.tasks = finished_tasks_;
            db_.Put(grid_info);
            break;
        }
        default:
            break;
        }
        Sleep(10);
    }
}

void UI::DrawGrid()
{
    setorigin(grid_x_, grid_y_);
    // 按时从上到下,从左到右的顺序遍历并绘制
    for (int i = 0; i < grid_rows_; ++i)
    {
        for (int j = 0; j < grid_columns_; ++j)
        {
            switch (sean::Function::CalculateBit(grid_[i][j]))
            {
            case 0:
            {
                //putimage(grid_x_ + grid_step_ * j, grid_y_ + grid_step_ * i, &img_block0_);
                putimage(grid_step_ * j, grid_step_ * i, &img_block0_);
                break;
            }
            case 1:
            {
                putimage(grid_step_ * j, grid_step_ * i, &img_block1_);
                break;
            }
            case 2:
            {
                putimage(grid_step_ * j, grid_step_ * i, &img_block2_);
                break;
            }
            case 3:
            {
                putimage(grid_step_ * j, grid_step_ * i, &img_block3_);
                break;
            }
            case 4:
            {
                putimage(grid_step_ * j, grid_step_ * i, &img_block4_);
                break;
            }
            default:
                break;
            }
        }
    }
    setorigin(0, 0);
}

void UI::GetGridCoord()
{
    // 获取当前日期,并判断当前日期是这个月的第几天
    int today = sean::Function::GetNowDate();
    //int mday = sean::Function::GetDayOfMonth();
    // 获取当前月份的第一天,并判断它是星期几
    int first_day_in_month = sean::Function::GetDayInMonth(1);
    int begin_index = sean::Function::GetDayOfWeek(first_day_in_month);
    int wday = sean::Function::GetDayOfWeek(today);
    
    int offset = today - first_day_in_month;    // 当前日期相对于本月第一天的偏移
    
    coordinate_.y = (begin_index + offset) / 7;        // 从该月第一天的星期开始偏移
    coordinate_.x = wday;
    std::cout << "Today: " << today << "; First: " << first_day_in_month
        << "; wday: " << wday << "; offset: " << offset << std::endl;
    std::cout << "(" << coordinate_.x << "," << coordinate_.y << ")" << std::endl;
}


void UI::GetGridCoord(int date)
{
    // 获取当前日期,并判断当前日期是这个月的第几天
    int today = date;// sean::Function::GetNowDate();
    //int mday = sean::Function::GetDayOfMonth();
    // 获取当前月份的第一天,并判断它是星期几
    int first_day_in_month = sean::Function::GetDayInMonth(1);
    int begin_index = sean::Function::GetDayOfWeek(first_day_in_month);
    int wday = sean::Function::GetDayOfWeek(today);

    int offset = today - first_day_in_month;    // 当前日期相对于本月第一天的偏移

    coordinate_.y = (begin_index + offset) / 7;        // 从该月第一天的星期开始偏移
    coordinate_.x = wday;

    std::cout << "-Today: " << today << "; First: " << first_day_in_month
        << "; wday: " << wday << "; offset: " << offset << std::endl;
    std::cout << "(" << coordinate_.x << "," << coordinate_.y << ")" << std::endl;

    ///*SYSTEMTIME system_time;
    //GetLocalTime(&system_time);
    //int today = system_time.wYear * 10000 + system_time.wMonth * 100 + system_time.wDay;
    //if (today != date)
    //{
    //    return coordinate_;
    //}*/
    //// 获取当前日期是本周的第几天(周日是0,周一是1)
    //time_t now = time(NULL); 
    //struct tm tmp_tm, in_tm;
    //int wday = 0;
    //localtime_s(&in_tm, &now);
    //in_tm.tm_year = date / 10000 - 1900;
    //in_tm.tm_mon = (date / 100) % 100 - 1;
    //in_tm.tm_mday = date % 100;
    //time_t t = mktime(&in_tm);
    //localtime_s(&tmp_tm, &t);
    //wday = tmp_tm.tm_wday;
    //std::cout << "wday:" << wday << std::endl;

    ////POINT coordinate;
    //coordinate_.x = wday;                    // 星期几就是网格中的第几行
    //int column_left = 7 - wday - 1;         // 本周还剩余几天(不包含当天);如:今天周5,则还剩余1天;周日当作一周的第1天
    ////int week_index_ = 0;                           // 本月第几个星期列;0表示第1个
    //coordinate_.y = week_index_;
    //if (column_left == 0)
    //{
    //    std::cout << "week_index:" << week_index_ << std::endl;
    //    // 切换到下个星期时,星期列+1;1个月最多6个星期,故模上6
    //    week_index_++;
    //    week_index_ %= 6;
    //}
    //
}


} // namespace 