@echo off
chcp 65001 >nul
echo 正在测试UTF-8编码的点云查看器...
echo.
cd /d "%~dp0"
bin\opengl_final_fixed.exe
echo.
echo 程序已退出，测试完成！
pause