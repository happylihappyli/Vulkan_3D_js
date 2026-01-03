#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
构建 script_viewer 的辅助脚本
"""
import os
import sys
import subprocess

# 设置编码为UTF-8
if sys.version_info >= (3, 0):
    sys.stdout.reconfigure(encoding='utf-8')

def main():
    print("开始构建 script_viewer...")
    
    # 使用 scons 直接构建
    cmd = ['scons', 'script_viewer_program']
    
    print(f"执行命令: {' '.join(cmd)}")
    
    result = subprocess.run(cmd, capture_output=True, text=True)
    
    print(result.stdout)
    if result.stderr:
        print(result.stderr, file=sys.stderr)
    
    if result.returncode == 0:
        print("构建成功！")
        return 0
    else:
        print(f"构建失败，错误代码: {result.returncode}")
        return 1

if __name__ == '__main__':
    sys.exit(main())
