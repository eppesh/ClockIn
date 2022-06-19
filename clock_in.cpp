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
    rect_btn_health_ = { 280, 40, 280 + 120, 40 + 40 };
    rect_btn_english_ = { 280, 90, 280 + 120, 90 + 40 };
    rect_btn_program_ = { 280, 140, 280 + 120, 140 + 40 };
    rect_btn_read_ = { 280, 190, 280 + 120, 190 + 40 };
    rect_btn_reset_ = { 30, 270, 30 + 80, 310 };
    rect_btn_prev_ = { grid_x_, grid_y_ - grid_step_, grid_x_ + grid_block_size_, grid_y_ };
    rect_btn_next_ = { grid_x_ + 5 * grid_step_, grid_y_ - grid_step_, grid_x_ + 5 * grid_step_ + grid_block_size_, grid_y_ };
    rect_month_ = { grid_x_ + 2 * grid_step_, grid_y_ - grid_step_, grid_x_ + 3 * grid_step_ + grid_block_size_, grid_y_ };
    rect_btn_makeup_ = { 30, 590, 30 + 30, 590 + 30 };

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

    grid_.resize(grid_rows_, std::vector<int>(grid_columns_, 0));
    months_ = { "","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
    
    now_date_ = sean::Function::GetNowDate();
    display_month_ = now_date_ / 100;

    coordinate_ = { 0,0 };
    GetGridCoord();             // 得到当天的坐标

    // 从数据库中读取当月的历史数据
    GetGridData(now_date_ / 10000, now_date_ / 100 % 100);
}

void UI::Draw()
{
    initgraph(460, 650/*, EW_SHOWCONSOLE*/);
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

    // 补打卡按钮
    setfillcolor(RGB(235, 237, 240));
    solidroundrect(rect_btn_makeup_.left, rect_btn_makeup_.top, rect_btn_makeup_.right, rect_btn_makeup_.bottom, 8, 8);
    drawtext(_T("M"), &rect_btn_makeup_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 统计内容显示区域
    setfillcolor(WHITE);
    setlinecolor(RGB(238, 238, 242));
    roundrect(20, 260, 440, 630, 10, 10);
    

    // 绘制网格    
    DrawGrid();

    // 网格左侧的星期提示
    settextcolor(BLACK);
    TCHAR current_month[4] = { 0 };
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)months_[now_date_ / 100 % 100].c_str(), -1, current_month, 4);
    drawtext(current_month, &rect_month_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    drawtext(_T("<"), &rect_btn_prev_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T(">"), &rect_btn_next_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    RECT rect_monday = { 60, grid_y_ + grid_step_, 60 + 60, grid_y_ + grid_step_ + grid_block_size_ };
    drawtext(_T("Mon"), &rect_monday, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    RECT rect_wednesday = { 60, grid_y_ + 3 * grid_step_, 60 + 60, grid_y_ + 3 * grid_step_ + grid_block_size_ };
    drawtext(_T("Wed"), &rect_wednesday, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    RECT rect_friday = { 60, grid_y_ + 5 * grid_step_, 60 + 60, grid_y_ + 5 * grid_step_ + grid_block_size_ };
    drawtext(_T("Fri"), &rect_friday, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    
    // 颜色说明(即打卡事项越多颜色越深)
    int test_x = grid_x_ + 10;
    int test_y = grid_y_ + 7 * grid_step_ + grid_block_size_ - 10 - 10;
    outtextxy(test_x - 60, test_y, _T("Less"));
    putimage(test_x, test_y, &img_block0_);
    putimage(test_x + grid_block_size_ + 10, test_y, &img_block1_);
    putimage(test_x + 2 * grid_block_size_ + 20, test_y, &img_block2_);
    putimage(test_x + 3 * grid_block_size_ + 30, test_y, &img_block3_);
    putimage(test_x + 4 * grid_block_size_ + 40, test_y, &img_block4_);
    outtextxy(test_x + 5 * grid_block_size_ + 50, test_y, _T("More"));

    DrawUpdate();
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
                // TODO:  跨天要重置,暂时不支持(目前是每天手动打开一次软件,而不是让它一直运行)
                if (!is_health_finished_ && display_month_ == now_date_ / 100)
                {
                    // 打卡响应
                    /*TCHAR health_notes[1024] = { 0 };
                    InputBox(health_notes, 1024, _T("Input the daily report about 'Health'."));*/
                    finished_tasks_ |= 0x1000;
                    is_health_finished_ = true;
                    COLORREF old = gettextcolor();
                    settextcolor(0x9eaebb);
                    outtextxy(30, 50, _T("√"));
                    settextcolor(old);
                }
                
            }
            else if (PtInRect(&rect_btn_english_, mouse))
            {
                if (!is_english_finished_ && display_month_ == now_date_ / 100)
                {
                    /*TCHAR english_notes[1024] = { 0 };
                    InputBox(english_notes, 1024, _T("Input the daily report about 'English'."));*/
                    finished_tasks_ |= 0x0100;
                    is_english_finished_ = true;
                    COLORREF old = gettextcolor();
                    settextcolor(0x9eaebb);
                    outtextxy(30, 100, _T("√"));
                    settextcolor(old);
                }
            }
            else if (PtInRect(&rect_btn_program_, mouse))
            {
                if (!is_program_finished_ && display_month_ == now_date_ / 100)
                {
                    /*TCHAR program_notes[1024] = { 0 };
                    InputBox(program_notes, 1024, _T("Input the daily report about 'Programming'."));*/
                    finished_tasks_ |= 0x0010;
                    is_program_finished_ = true;
                    COLORREF old = gettextcolor();
                    settextcolor(0x9eaebb);
                    outtextxy(30, 150, _T("√"));
                    settextcolor(old);
                }
            }
            else if (PtInRect(&rect_btn_read_, mouse))
            {
                if (!is_read_finished_ && display_month_ == now_date_ / 100)
                {
                    /*TCHAR read_notes[1024] = { 0 };
                    InputBox(read_notes, 1024, _T("Input the daily report about 'Reading'."));*/
                    finished_tasks_ |= 0x0001;
                    is_read_finished_ = true;
                    COLORREF old = gettextcolor();
                    settextcolor(0x9eaebb);
                    outtextxy(30, 200, _T("√"));
                    settextcolor(old);
                }
            }
            else if (PtInRect(&rect_btn_reset_, mouse))
            {
                if (display_month_ == now_date_ / 100 && (is_health_finished_ || is_english_finished_ 
                    || is_program_finished_ || is_read_finished_))
                {
                    is_health_finished_ = false;
                    is_english_finished_ = false;
                    is_program_finished_ = false;
                    is_read_finished_ = false;
                    finished_tasks_ = 0;
                    grid_[coordinate_.x][coordinate_.y] = finished_tasks_;
                    DrawGrid();
                    // 将数据更新到数据库
                    GridInfo grid_info;
                    grid_info.date = now_date_;
                    grid_info.x = coordinate_.x;
                    grid_info.y = coordinate_.y;
                    grid_info.tasks = finished_tasks_;
                    db_.Put(grid_info);
                    COLORREF old = getfillcolor();
                    setfillcolor(RGB(242, 242, 241));
                    solidrectangle(28, 50, 60, 235);    // 对勾区域
                    setfillcolor(old);    // 恢复成白色背景
                }
            }
            

            if (display_month_ == now_date_ / 100 && (is_health_finished_ || is_english_finished_
                || is_program_finished_ || is_read_finished_))
            {
                grid_[coordinate_.x][coordinate_.y] = finished_tasks_;
                DrawGrid();
                // 将数据更新到数据库
                GridInfo grid_info;
                grid_info.date = now_date_;
                grid_info.x = coordinate_.x;
                grid_info.y = coordinate_.y;
                grid_info.tasks = finished_tasks_;
                db_.Put(grid_info);
            }
            break;
        }
        case WM_LBUTTONUP:
        {
            if (PtInRect(&rect_btn_prev_, mouse))
            {
                // 切换到上一个月
                GridInfo grid_info;
                db_.GetFirstData(grid_info);
                int first_date = grid_info.date;
                // 只有数据库中第一条记录的年月小于当前年月才能切换到上一月(前面需有数据才行)
                if (first_date / 100 < display_month_)
                {
                    int tmp_year = display_month_ / 100;
                    int tmp_month = display_month_ % 100;
                    tmp_month--;
                    tmp_year = (tmp_month < 1) ? (tmp_year - 1) : tmp_year;
                    tmp_month = (tmp_month == 0) ? (tmp_month + 1) : (tmp_month % 12);;
                    display_month_ = tmp_year * 100 + tmp_month;
                    // 读取上一月的数据
                    GetGridData(display_month_ / 100, display_month_ % 100);
                    DrawGrid();
                    solidrectangle(rect_month_.left, rect_month_.top, rect_month_.right, rect_month_.bottom);
                    TCHAR current_month[4] = { 0 };                    
                    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)months_[display_month_ % 100].c_str(), -1, current_month, 4);
                    drawtext(current_month, &rect_month_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
            }
            else if (PtInRect(&rect_btn_next_, mouse))
            {
                // 切换到下一个月
                // 当前显示年月不是当天所属年月时才能点击下一月
                if (display_month_ != (now_date_ / 100))
                {
                    int tmp_year = display_month_ / 100;
                    int tmp_month = display_month_ % 100;
                    tmp_month++;
                    tmp_year = (tmp_month > 12) ? (tmp_year + 1) : tmp_year;
                    tmp_month = (tmp_month == 12) ? (tmp_month % 12 + 1) : (tmp_month % 12);
                    display_month_ = tmp_year * 100 + tmp_month;
                    // 读取下一月的数据
                    GetGridData(display_month_ / 100, display_month_ % 100);
                    DrawGrid();
                    solidrectangle(rect_month_.left, rect_month_.top, rect_month_.right, rect_month_.bottom);
                    TCHAR current_month[4] = { 0 };
                    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)months_[display_month_ % 100].c_str(), -1, current_month, 4);
                    drawtext(current_month, &rect_month_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
            }
            else if (PtInRect(&rect_btn_makeup_, mouse))
            {
                // 04/11/2022; added by sean; add make-up function
                TCHAR input[32] = { 0 };
                InputBox(input, 32, _T("Input the make-up data with the format of \"20220411;0x1101\"."));
                std::string info = sean::Function::TcharToString(input);
                std::regex pattern("[0-9]{8};0[xX][0-9a-fA-F]+");
                if (std::regex_match(info, pattern))
                {
                    int date = atoi(info.substr(0, 8).c_str());
                    if (date == now_date_)
                    {
                        break;
                    }
                    int tasks = strtol(info.substr(9, 6).c_str(), nullptr, 16);
                    POINT point = GetGridCoord(date);
                    GridInfo grid;
                    grid.date = date;
                    grid.x = point.x;
                    grid.y = point.y;
                    grid.tasks = tasks;
                    db_.Put(grid);
                    if (display_month_ == (date / 100))
                    {
                        // 补打卡的月份是当前显示月,则显示补打卡的内容
                        grid_[grid.x][grid.y] = grid.tasks;
                    }
                    DrawGrid();
                }
            }
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
    LOGFONT old_style;
    gettextstyle(&old_style);
    POINT frist_day_coord = GetGridCoord(display_month_ * 100 + 1);   // 获取当前显示月份的第1天的坐标
    // 当前显示月份的最后一天
    int last_day = display_month_ * 100 + sean::Function::GetMonthDays(display_month_ / 100, display_month_ % 100);
    int num = display_month_ * 100;
    bool is_draw_num = false;   // 是否绘制数字; 避免在非法区域(如本月1日从第2个小灰格开始,则第1个小灰格为非法区域)绘制；
    // 按时从上到下,从左到右的顺序遍历并绘制        
    for (int i = 0; i < grid_columns_; ++i)
    {
        for (int j = 0; j < grid_rows_; ++j)
        {
            // 绘制网格灰色小方块
            switch (sean::Function::CalculateBit(grid_[j][i]))
            {
            case 0:
            {
                putimage(grid_step_ * i, grid_step_ * j, &img_block0_);         // 绘制灰色方块
                settextcolor(RGB(203, 201, 204));       
                break;
            }
            case 1:
            {
                putimage(grid_step_ * i, grid_step_ * j, &img_block1_);
                settextcolor(RGB(134, 165, 170));
                break;
            }
            case 2:
            {
                putimage(grid_step_ * i, grid_step_ * j, &img_block2_);
                settextcolor(RGB(239, 237, 238));
                break;
            }
            case 3:
            {
                putimage(grid_step_ * i, grid_step_ * j, &img_block3_);
                settextcolor(RGB(239, 237, 238));
                break;
            }
            case 4:
            {
                putimage(grid_step_ * i, grid_step_ * j, &img_block4_);
                settextcolor(RGB(239, 237, 238));
                break;
            }
            default:
                break;
            }
                        
            if (j == frist_day_coord.x && i == frist_day_coord.y)
            {
                is_draw_num = true;
            }

            // 绘制网格上的日期数字
            if (is_draw_num && num < last_day)
            {
                num++;
                COLORREF old_text = gettextcolor();                
                //settextcolor(WHITE);
                settextstyle(20, 0, _T("Times New Roman"));
                RECT rect = { grid_step_ * i, grid_step_ * j, grid_step_ * i + grid_block_size_, grid_step_ * j + grid_block_size_ };
                TCHAR day[3] = { 0 };
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(std::to_string(num % 100).c_str()), -1, day, 3);
                drawtext(day, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);                
            }
        }
    }

    // 在当前日期的网格外加一个矩形框用以标识
    if (now_date_ / 100 == display_month_)
    {
        COLORREF old = getlinecolor();
        setlinecolor(LIGHTGRAY);
        rectangle(grid_step_ * coordinate_.y - 3, grid_step_ * coordinate_.x - 3, grid_step_ * coordinate_.y + grid_block_size_ + 2, grid_step_ * coordinate_.x + grid_block_size_ + 2);
        setlinecolor(old);
    }
    else
    {
        COLORREF old = getlinecolor();
        setlinecolor(WHITE);
        rectangle(grid_step_ * coordinate_.y - 3, grid_step_ * coordinate_.x - 3, grid_step_ * coordinate_.y + grid_block_size_ + 2, grid_step_ * coordinate_.x + grid_block_size_ + 2);
        setlinecolor(old);
    }

    // recovery the text-style and the origin of coordinate
    settextstyle(&old_style);
    setorigin(0, 0);
}

void UI::GetGridCoord()
{
    // 获取当前日期,并判断当前日期是这个月的第几天
    //int today = sean::Function::GetNowDate();
    // 获取当前月份的第一天,并判断它是星期几
    int first_day_in_month = (now_date_ / 100) * 100 + 1;  // 该月的第1天
    int begin_index = sean::Function::GetDayOfWeek(first_day_in_month);
    int wday = sean::Function::GetDayOfWeek(now_date_);
    
    int offset = now_date_ - first_day_in_month;    // 当前日期相对于本月第一天的偏移
    
    coordinate_.y = (begin_index + offset) / 7;        // 从该月第一天的星期开始偏移
    coordinate_.x = wday;
}


POINT UI::GetGridCoord(int date)
{
    int today = date;
    // 获取当前月份的第一天,并判断它是星期几
    int first_day_in_month = (today / 100) * 100 + 1;  // 该月的第1天
    int begin_index = sean::Function::GetDayOfWeek(first_day_in_month);
    int wday = sean::Function::GetDayOfWeek(today);

    int offset = today - first_day_in_month;    // 当前日期相对于本月第一天的偏移

    POINT result = { 0,0 };
    result.y = (begin_index + offset) / 7;        // 从该月第一天的星期开始偏移
    result.x = wday;
    return result;
}

void UI::GetGridData(int year, int month)
{
    int first = year * 10000 + month * 100 + 1;        // 该月第一天
    int days = sean::Function::GetMonthDays(year, month);
    // 该月最后一天;若是当月,则最后一天是当天(因为当天以后肯定没数据)
    int last = (now_date_ / 100 % 100 == month) ? now_date_ : year * 10000 + month * 100 + days;

    // 从数据库中更新当月Grid数据前,先清空grid_
    for (int i = 0; i < grid_.size(); ++i)
    {
        for (int j = 0; j < grid_[0].size(); ++j)
        {
            grid_[i][j] = 0;
        }
    }

    for (int i = first; i <= last; ++i)
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
        } // db_.Get
    } // for
}

void UI::DrawUpdate()
{
    COLORREF old = gettextcolor();
    settextcolor(0x9eaebb);
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
    settextcolor(old);
}

} // namespace 