#include "clock_in.h"

#include <sstream>          // std::ostringstream
#include <iomanip>          // std::setfill, std::setw

namespace clock_in
{
UI::UI() :grid_rows_(7), grid_columns_(6), grid_block_size_(30), grid_step_(30 + 10),
grid_x_(20 + 150), grid_y_(260 + 10), week_index_(0), finished_tasks_(0),
is_health_finished_(false), is_english_finished_(false), is_program_finished_(false),
is_read_finished_(false), now_date_(0), display_month_(0), placeholder_(20)
{
    Init();
}

UI::~UI()
{

}

void UI::Init()
{    
    
    rect_btn_prev_ = { grid_x_, grid_y_ - grid_step_, grid_x_ + grid_block_size_, grid_y_ };
    rect_btn_next_ = { grid_x_ + 5 * grid_step_, grid_y_ - grid_step_, grid_x_ + 5 * grid_step_ + grid_block_size_, grid_y_ };
    rect_month_ = { grid_x_ + grid_step_, grid_y_ - grid_step_, grid_x_ + 4 * grid_step_ + grid_block_size_, grid_y_ };
    

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

    int hash_size = grid_rows_ * grid_columns_ + 1;
    coord_date_hash_.resize(hash_size, 0);
    grid_.resize(grid_rows_, std::vector<int>(grid_columns_, 0));
    grid_block_rect_.resize(grid_rows_, std::vector<RECT>(grid_columns_, { 0,0,0,0 }));
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
    int win_width = 460;        // 窗体宽度
    int win_height = 690;
    
    initgraph(win_width, win_height , EW_DBLCLKS/* | EW_SHOWCONSOLE*/);
    setbkcolor(WHITE);
    setbkmode(TRANSPARENT);
    cleardevice();

    // 1. 打卡区域(包括任务名称和打卡按钮Clock In)
    setfillcolor(RGB(242, 242, 241));
    clockin_area_ = { placeholder_, placeholder_ / 2, win_width - placeholder_, placeholder_ / 2 + 210 };
    task_name_area_ = { 40, 10, 240, 220 };
    solidroundrect(clockin_area_.left, clockin_area_.top, clockin_area_.right, clockin_area_.bottom, 10, 10);

    setorigin(placeholder_, placeholder_ / 2);

    // 1.1 任务名称    
    settextstyle(32, 0, _T("Times New Roman"));
    settextcolor(0x9eaebb);
    task_health_ = { 40, 10, 40 + 200, 10 + 40 };
    drawtext(_T("Health"), &task_health_, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    setlinestyle(PS_DASH);
    setlinecolor(LIGHTGRAY);
    line(40, 10 + 47, 40 + 180, 10 + 50);
    task_english_ = { 40, 60, 40 + 200, 60 + 40 };
    drawtext(_T("English"), &task_english_, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    line(40, 10 + 97, 40 + 180, 110);
    task_program_ = { 40, 110, 40 + 200, 110 + 40 };
    drawtext(_T("Programming"), &task_program_, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    line(40, 10 + 147, 40 + 180, 160);
    task_read_ = { 40, 160, 40 + 200, 160 + 40 };
    drawtext(_T("Reading"), &task_read_, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    
    // 1.2 打卡按钮
    rect_btn_health_ = { 280, 10, 280 + 120, 10 + 40 };
    rect_btn_english_ = { 280, 60, 280 + 120, 60 + 40 };
    rect_btn_program_ = { 280, 110, 280 + 120, 110 + 40 };
    rect_btn_read_ = { 280, 160, 280 + 120, 160 + 40 };
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
    
    setorigin(0, 0);
    //////////////////////////////////////////////////////////////////////////
 
    // 2. 统计内容展示区域(由上到下自左到右依次包括：月份、星期、网格内容和颜色说明等)
    setfillcolor(WHITE);
    setlinecolor(RGB(238, 238, 242));
    display_area_ = { placeholder_, 260 - 30, win_width - placeholder_, 630 - 30 };
    roundrect(display_area_.left, display_area_.top, display_area_.right, display_area_.bottom, 10, 10);
    

    // 绘制网格    
    DrawGrid();

    // 绘制网格左侧的星期和网格上面的月份提示
    settextcolor(BLACK);
    TCHAR current_month[9] = { 0 };
    std::string tmp = std::to_string(now_date_ / 10000) + " " + months_[now_date_ / 100 % 100] + "\0";
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)tmp.c_str(), -1, current_month, 9);
    drawtext(current_month, &rect_month_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    drawtext(_T("<"), &rect_btn_prev_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    drawtext(_T(">"), &rect_btn_next_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    RECT rect_monday = { 60, grid_y_ + grid_step_, 60 + 60, grid_y_ + grid_step_ + grid_block_size_ };
    drawtext(_T("Mon"), &rect_monday, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    RECT rect_wednesday = { 60, grid_y_ + 3 * grid_step_, 60 + 60, grid_y_ + 3 * grid_step_ + grid_block_size_ };
    drawtext(_T("Wed"), &rect_wednesday, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    RECT rect_friday = { 60, grid_y_ + 5 * grid_step_, 60 + 60, grid_y_ + 5 * grid_step_ + grid_block_size_ };
    drawtext(_T("Fri"), &rect_friday, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    
    // 绘制颜色说明(即打卡事项越多颜色越深)
    int test_x = grid_x_ + 10;
    int test_y = grid_y_ + 7 * grid_step_ + grid_block_size_ - 10 - 10;
    outtextxy(test_x - 60, test_y, _T("Less"));
    putimage(test_x, test_y, &img_block0_);
    putimage(test_x + grid_block_size_ + 10, test_y, &img_block1_);
    putimage(test_x + 2 * grid_block_size_ + 20, test_y, &img_block2_);
    putimage(test_x + 3 * grid_block_size_ + 30, test_y, &img_block3_);
    putimage(test_x + 4 * grid_block_size_ + 40, test_y, &img_block4_);
    outtextxy(test_x + 5 * grid_block_size_ + 50, test_y, _T("More"));

    //////////////////////////////////////////////////////////////////////////

    // 3. 其他功能区域(包括重置、补打卡等功能按钮）    
    setfillcolor(WHITE);
    setlinecolor(RGB(238, 238, 242));
    roundrect(placeholder_, 610, win_width - placeholder_, 610 + 30, 10, 10);

    rect_btn_reset_ = { 30, 610, 30 + 100, 610 + 30 };
    rect_btn_makeup_ = { 30 + 80 + 30, 610, 140 + 100, 610 + 30 };

    // 重置按钮
    setfillcolor(RGB(235, 237, 240));
    solidroundrect(rect_btn_reset_.left, rect_btn_reset_.top, rect_btn_reset_.right, rect_btn_reset_.bottom, 8, 8);
    drawtext(_T("Reset"), &rect_btn_reset_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 补打卡按钮
    setfillcolor(RGB(235, 237, 240));
    solidroundrect(rect_btn_makeup_.left, rect_btn_makeup_.top, rect_btn_makeup_.right, rect_btn_makeup_.bottom, 8, 8);
    drawtext(_T("MakeUp"), &rect_btn_makeup_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 4. 提示信息区域(相当于状态栏,可展示一些提示信息)    
    setfillcolor(WHITE);
    setlinecolor(RGB(238, 238, 242));
    roundrect(placeholder_, 650, win_width - placeholder_, 650 + 30, 10, 10);

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
                    outtextxy(30, placeholder_ + 10, _T("√"));
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
                    outtextxy(30, placeholder_ + 60, _T("√"));
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
                    outtextxy(30, placeholder_ + 110, _T("√"));
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
                    outtextxy(30, placeholder_ + 160, _T("√"));
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
                    solidrectangle(28, 30, 60, 205);    // 对勾区域
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
                    
                    ShowMonthGrid(display_month_);
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
                    
                    ShowMonthGrid(display_month_);
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
        case WM_RBUTTONUP:  // 右键单击在底部状态区域显示当天打卡信息        
        case WM_LBUTTONDBLCLK:  // 左键双击网格,可修改当前网格对应日期的打卡信息
        {
            if (PtInRect(&task_name_area_,mouse) && msg.message == WM_LBUTTONDBLCLK)         // 处理打卡区域的双击
            {
                /*
                // 双击任务名称进行修改 (因为没做持久化,故该功能目前并没实际用处)
                setorigin(placeholder_, placeholder_ / 2);
                LOGFONT old_style;
                gettextstyle(&old_style);

                settextstyle(32, 0, _T("Times New Roman"));
                settextcolor(0x9eaebb);
                setfillcolor(RGB(242, 242, 241));

                TCHAR input[32] = { 0 };
                InputBox(input, 32, _T("请输入准备打卡的任务名称（不能超过30个英文字符）."));
                // TODO: 1.暂未做输入数据的可靠性验证(如非法长度等);2.修改名称后应该持久化,以便重启后仍生效(暂未做);
                if (PtInRect(&task_health_, mouse))
                {
                    solidrectangle(task_health_.left, task_health_.top, task_health_.right, task_health_.bottom);
                    drawtext(input, &task_health_, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                }
                else if (PtInRect(&task_english_, mouse))
                {
                    solidrectangle(task_english_.left, task_english_.top, task_english_.right, task_english_.bottom);
                    drawtext(input, &task_english_, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                }
                else if (PtInRect(&task_program_, mouse))
                {
                    solidrectangle(task_program_.left, task_program_.top, task_program_.right, task_program_.bottom);
                    drawtext(input, &task_program_, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                }
                else if (PtInRect(&task_read_, mouse))
                {
                    solidrectangle(task_read_.left, task_read_.top, task_read_.right, task_read_.bottom);
                    drawtext(input, &task_read_, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                }
                setorigin(0, 0);
                settextstyle(&old_style);
                */
            }
            else if (PtInRect(&display_area_,mouse))    // 处理网格部分的单/双击
            {
                // 计算鼠标所在区域对应网格中的坐标
                int col_in_grid = -1;
                int row_in_grid = -1;
                bool is_stop = false;
                for (int i = 0; i < grid_block_rect_[0].size(); ++i)
                {
                    for (int j = 0; j < grid_block_rect_.size(); ++j)
                    {
                        if (PtInRect(&grid_block_rect_[j][i], mouse))
                        {
                            col_in_grid = i;
                            row_in_grid = j;
                            is_stop = true;
                            break;
                        }
                    }
                    if (is_stop)
                    {
                        break;
                    }
                }

                // 先清空状态展示区域信息
                RECT rect_info = { placeholder_, 650, 460 - placeholder_, 650 + 30 };
                solidrectangle(rect_info.left, rect_info.top, rect_info.right, rect_info.bottom);

                if (is_stop)
                {
                    settextcolor(BLACK);
                    // 获取网格点对应的日期
                    int index = row_in_grid * grid_columns_ + col_in_grid;
                    int date = coord_date_hash_[index];
                    if (msg.message == WM_LBUTTONDBLCLK && date >= now_date_)
                    {
                        break;
                    }
                    // 获取网格点对应日期上的任务信息
                    std::ostringstream oss;
                    oss << "0x" << std::setfill('0') << std::setw(4) << std::hex << grid_[row_in_grid][col_in_grid];
                    std::string tasks = oss.str();
                    std::string str = " " + std::to_string(date) + " - " + tasks;
                    // 展示的信息
                    TCHAR content[32] = { 0 };

                    if (msg.message == WM_RBUTTONUP)
                    {
                        // 右键单击时在底部状态区域显示对应日期及打卡信息
                        MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, content, 32);
                        drawtext(content, &rect_info, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                        break;
                    }

                    // 下面是对左键双键的处理:可修改点击的网格对应日期的打卡信息
                    std::string tmp = "修改" + str + " 的打卡信息";
                    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)tmp.c_str(), -1, content, 32);

                    TCHAR input[32] = { 0 };
                    InputBox(input, 32, _T("请按 \"0x1101\" 的格式输入待修改的打卡信息."), content);
                    std::string info = sean::Function::TcharToString(input);
                    std::regex pattern("0[xX][0-9a-fA-F]+");
                    if (std::regex_match(info, pattern))
                    {
                        int tasks = strtol(info.substr(0, 6).c_str(), nullptr, 16);
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

    // 每次绘制前将坐标与日期的哈希表清空
    memset(&coord_date_hash_[0], 0, coord_date_hash_.size() * sizeof(int));

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

                // added in 11/18/2022 绘制网格的同时获取各网格块的区域
                RECT tmp = { grid_x_ + rect.left, grid_y_ + rect.top, grid_x_ + rect.right,grid_y_ + rect.bottom };
                grid_block_rect_[j][i] = tmp;
                coord_date_hash_[j * grid_columns_ + i] = num;
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
            grid_block_rect_[i][j] = { 0,0,0,0 };
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
        outtextxy(placeholder_ + 10, placeholder_ + 10, _T("√"));
    }
    if (is_english_finished_)
    {
        outtextxy(placeholder_ + 10, placeholder_ + 60, _T("√"));
    }
    if (is_program_finished_)
    {
        outtextxy(placeholder_ + 10, placeholder_ + 110, _T("√"));
    }
    if (is_read_finished_)
    {
        outtextxy(placeholder_ + 10, placeholder_ + 160, _T("√"));
    }
    settextcolor(old);
}

void UI::ShowMonthGrid(int month)
{
    // 读取下一月的数据
    GetGridData(display_month_ / 100, display_month_ % 100);
    DrawGrid();
    // Updated in 11/17/2022: 修复切换上下月时"月份"背景及字体颜色显示异常的问题
    setfillcolor(WHITE);
    settextcolor(BLACK);
    solidrectangle(rect_month_.left, rect_month_.top, rect_month_.right, rect_month_.bottom);
    TCHAR current_month[9] = { 0 };
    std::string tmp = std::to_string(display_month_ / 100) + " " + months_[display_month_ % 100] + "\0";
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)tmp.c_str(), -1, current_month, 9);
    drawtext(current_month, &rect_month_, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    // 先清空状态展示区域信息
    RECT rect_info = { placeholder_, 650, 460 - placeholder_, 650 + 30 };
    solidrectangle(rect_info.left, rect_info.top, rect_info.right, rect_info.bottom);
}

} // namespace 