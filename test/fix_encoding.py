#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
修复源文件编码，添加UTF-8 BOM标记
"""

import os
import sys

def add_utf8_bom(file_path):
    """为文件添加UTF-8 BOM标记"""
    try:
        # 读取文件内容
        with open(file_path, 'rb') as f:
            content = f.read()
        
        # 检查是否已有BOM
        if content.startswith(b'\xef\xbb\xbf'):
            print(f"文件 {file_path} 已经有BOM标记")
            return True
        
        # 添加BOM标记
        content_with_bom = b'\xef\xbb\xbf' + content
        
        # 写回文件
        with open(file_path, 'wb') as f:
            f.write(content_with_bom)
        
        print(f"已为文件 {file_path} 添加UTF-8 BOM标记")
        return True
        
    except Exception as e:
        print(f"处理文件 {file_path} 时出错: {e}")
        return False

def main():
    """主函数"""
    project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    cpp_file = os.path.join(project_root, 'src', 'main.cpp')
    
    if not os.path.exists(cpp_file):
        print(f"源文件不存在: {cpp_file}")
        return False
    
    # 添加BOM标记
    success = add_utf8_bom(cpp_file)
    
    if success:
        print("文件编码修复完成")
        
        # 检查文件编码
        try:
            with open(cpp_file, 'r', encoding='utf-8-sig') as f:
                content = f.read()
                print(f"文件编码验证成功，内容长度: {len(content)} 字符")
                
                # 检查开头是否有预期的注释
                if content.startswith('// Point Cloud Viewer'):
                    print("文件开头验证通过")
                else:
                    print("警告：文件开头格式可能有问题")
                    
        except UnicodeDecodeError as e:
            print(f"文件编码验证失败: {e}")
            return False
    
    return success

if __name__ == '__main__':
    success = main()
    if success:
        print("UTF-8 BOM添加成功")
    else:
        print("UTF-8 BOM添加失败")
        sys.exit(1)