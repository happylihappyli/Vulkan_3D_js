# -*- coding: utf-8 -*-
"""
移除所有头文件中的UTF-8 BOM标记
"""
import os
import glob

def remove_bom_from_file(filepath):
    """移除文件中的BOM标记"""
    try:
        with open(filepath, 'rb') as f:
            content = f.read()
        
        # 检查是否有BOM标记 (EF BB BF)
        if content.startswith(b'\xef\xbb\xbf'):
            # 移除BOM标记
            content = content[3:]
            
            # 写回文件
            with open(filepath, 'wb') as f:
                f.write(content)
            
            print(f"已移除BOM标记: {filepath}")
            return True
        else:
            print(f"无BOM标记: {filepath}")
            return False
    except Exception as e:
        print(f"处理文件失败 {filepath}: {e}")
        return False

def main():
    """主函数"""
    print("开始移除文件中的BOM标记...")
    
    # 查找所有头文件和源文件
    header_files = glob.glob('include/*.h')
    source_files = glob.glob('src/*.cpp')
    all_files = header_files + source_files
    
    print(f"找到 {len(all_files)} 个文件 (头文件: {len(header_files)}, 源文件: {len(source_files)})")
    
    removed_count = 0
    for filepath in all_files:
        if remove_bom_from_file(filepath):
            removed_count += 1
    
    print(f"\n完成！共移除了 {removed_count} 个文件的BOM标记")

if __name__ == '__main__':
    main()
