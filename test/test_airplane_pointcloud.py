#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
飞机点云测试脚本
测试1000万个点的飞机形状点云生成和显示效果
"""

import os
import subprocess
import time
import sys

def test_pointcloud_generation():
    """测试点云生成功能"""
    print("开始测试飞机点云生成...")
    
    # 编译程序
    print("1. 编译程序...")
    start_time = time.time()
    
    try:
        result = subprocess.run(["scons"], cwd="e:\\GitHub3\\cpp\\Vulkan_Test2", 
                              capture_output=True, text=True, shell=True)
        
        if result.returncode == 0:
            print("✓ 编译成功")
            print(f"编译耗时: {time.time() - start_time:.2f}秒")
        else:
            print("✗ 编译失败")
            print("错误信息:")
            print(result.stderr)
            return False
    except Exception as e:
        print(f"✗ 编译异常: {e}")
        return False
    
    # 运行程序并捕获输出
    print("\n2. 运行程序测试点云生成...")
    try:
        # 运行程序并等待几秒钟
        process = subprocess.Popen([".\\test.exe"], 
                                 cwd="e:\\GitHub3\\cpp\\Vulkan_Test2\\bin",
                                 stdout=subprocess.PIPE, 
                                 stderr=subprocess.PIPE,
                                 shell=True)
        
        # 等待程序运行一段时间
        time.sleep(5)
        
        # 终止程序
        process.terminate()
        process.wait()
        
        print("✓ 程序运行完成")
    except Exception as e:
        print(f"✗ 运行异常: {e}")
        return False
    
    # 检查日志文件
    print("\n3. 检查点云生成日志...")
    log_file = "e:\\GitHub3\\cpp\\Vulkan_Test2\\airplane_pointcloud_log.txt"
    
    if os.path.exists(log_file):
        with open(log_file, 'r', encoding='utf-8') as f:
            log_content = f.read()
        
        # 检查关键信息
        checks = [
            ("总点数: 10000000", "点云数量正确"),
            ("机身生成完成", "机身生成成功"),
            ("主翼生成完成", "机翼生成成功"),
            ("水平尾翼生成完成", "水平尾翼生成成功"),
            ("垂直尾翼生成完成", "垂直尾翼生成成功"),
            ("发动机生成完成", "发动机生成成功"),
            ("驾驶舱生成完成", "驾驶舱生成成功")
        ]
        
        all_passed = True
        for check_text, description in checks:
            if check_text in log_content:
                print(f"✓ {description}")
            else:
                print(f"✗ {description}")
                all_passed = False
        
        if all_passed:
            print("\n✓ 所有测试通过！飞机点云生成成功！")
            return True
        else:
            print("\n✗ 部分测试失败")
            return False
    else:
        print("✗ 日志文件不存在")
        return False

def main():
    """主函数"""
    print("=" * 50)
    print("飞机点云测试脚本")
    print("=" * 50)
    
    # 设置工作目录
    os.chdir("e:\\GitHub3\\cpp\\Vulkan_Test2")
    
    # 运行测试
    success = test_pointcloud_generation()
    
    print("\n" + "=" * 50)
    if success:
        print("测试结果: 成功 ✓")
        print("飞机形状的1000万个点云已成功生成！")
        print("请查看 bin\\test.exe 运行效果")
    else:
        print("测试结果: 失败 ✗")
    print("=" * 50)
    
    # 播放语音提示
    try:
        import pyttsx3
        engine = pyttsx3.init()
        if success:
            engine.say("飞机点云测试成功，请查看效果")
        else:
            engine.say("飞机点云测试失败，请检查")
        engine.runAndWait()
    except:
        print("语音提示不可用")
    
    return 0 if success else 1

if __name__ == "__main__":
    sys.exit(main())