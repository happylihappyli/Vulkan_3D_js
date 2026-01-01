#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
快速测试UTF-8编码控制台输出
"""

import subprocess
import time
import sys
from pathlib import Path

def quick_test():
    """快速测试程序输出"""
    project_root = Path(__file__).parent.parent
    exe_path = project_root / "bin" / "test.exe"
    
    print("=== 快速UTF-8编码测试 ===")
    print(f"程序路径: {exe_path}")
    
    if not exe_path.exists():
        print(f"错误：程序不存在 {exe_path}")
        return False
    
    print("启动程序...")
    
    try:
        # 启动程序
        process = subprocess.Popen(
            [str(exe_path)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            encoding='utf-8'
        )
        
        # 等待3秒
        time.sleep(3)
        
        # 尝试获取输出
        try:
            stdout, stderr = process.communicate(timeout=2)
            print("程序输出:")
            if stdout:
                print("标准输出:")
                print(stdout)
            if stderr:
                print("错误输出:")
                print(stderr)
        except subprocess.TimeoutExpired:
            print("程序仍在运行，强制终止...")
            process.kill()
            process.wait()
            print("程序已终止")
        
        return True
        
    except Exception as e:
        print(f"测试过程中出错: {e}")
        return False

if __name__ == '__main__':
    quick_test()