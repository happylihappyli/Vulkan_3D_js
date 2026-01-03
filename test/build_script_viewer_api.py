#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
构建 script_viewer 的辅助脚本 - 直接使用SCons API
"""
import os
import sys

# 设置编码为UTF-8
if sys.version_info >= (3, 0):
    sys.stdout.reconfigure(encoding='utf-8')

def main():
    print("开始构建 script_viewer...")
    
    # 设置SConstruct路径
    sconscript = 'SConstruct'
    
    # 导入SCons
    from SCons.Script import SConscript
    
    # 执行SConstruct
    try:
        # 设置目标
        targets = ['script_viewer_program']
        
        # 调用SCons
        from SCons.Script import main as scons_main
        
        # 模拟命令行参数
        sys.argv = ['scons', 'script_viewer_program']
        
        # 运行SCons
        result = scons_main()
        
        if result == 0:
            print("构建成功！")
        else:
            print(f"构建失败，错误代码: {result}")
            
        return result
    except Exception as e:
        print(f"构建过程中发生错误: {e}")
        import traceback
        traceback.print_exc()
        return 1

if __name__ == '__main__':
    sys.exit(main())
