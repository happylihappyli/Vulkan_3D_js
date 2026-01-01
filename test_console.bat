@echo off
echo 正在启动点云查看器测试...
echo 程序启动后会显示console窗口，包含调试信息
echo.
cd /d "%~dp0"
bin\opengl_final_fixed.exe
echo.
echo 程序已退出
pause