

[toc]

`Clock In` is a simple tool for personal clocking in.

It is mainly developed in C++. It uses the [EeayX](https://easyx.cn/) to draw the UI and store and access the historical data with the help of [LevelDB](https://github.com/google/leveldb). 

The original intention of this tool is mainly for self-use and, by the way, to learn how to use LevelDB.

Authors: Sean (eppesh@163.com)

# Features

- Clocking in;
- Showing the information of clocking in (including the current date and the historical data);

# Documentation

The documents is online and bundled with the source code.

# Building

It is currently limited to the Windows platform.

You can use `Visual Studio` to open the project file `.sln` or `.vcxproj` directly and compile and run it.

Development related:

- `C++`；
- `Visual Studio 2019`; 
- `EasyX 20220116`; 

# Usage

- Clocking in：

  - Select the corresponding task and click the button after it to clock in.
  - After clocking in, a tick will appear in front of the task, indicating that you have completed this task.

- Reset:

  You cannot clock in the same task repeatedly. If you want to change the result, you can click the `Reset` button to clear all the clocked-in tasks.

- Display historical data：

  After starting the tool, it will display the historical information, if there is any.
  
- Make-up:

  If you completed several tasks but forgot to lock in, you can make up for it by clicking the `M` button (which means `make up`). By typing into the date and tasks with the format of `20220411;0x1101`, you can clock-in the specific date again. Note that `0x1101` means that you have finished the first, second, and fourth tasks. 

# Screenshots

![Pic](https://github.com/eppesh/ClockIn/blob/main/screenshots/Screenshots.gif)

Please refer to the pictures in the `./screenshots/` directory for other screenshots.

# Changelogs

- Updated 01/04/2022
  - Add the project；
  - Finish the main interface；
- Updated 02/04/2022
  - Finish the base function of clocking in；
  - Finish the function of a calendar;
- Updated 03/04/2022
  - Add  database function and use`LevelDB` to store & access historical  information;
- Updated 04/04/2022
  - Optimize the display of historical information
  - Update`README.md`；
  - Add page flip function;
- Updated 04/05/2022
  - Add date numbers;
- Updated 04/11/2022
  - Add make-up function;

# Upcoming Changes

Some of the goals for 2022 are:

- Code optimizing; 

# Postscripts

This tool is mainly designed for self-use. There are still a lot of places to optimize. Frequently updates are not guaranteed. It may be updated if encountering bugs or new features during use.
