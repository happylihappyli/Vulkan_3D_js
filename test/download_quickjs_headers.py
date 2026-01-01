# -*- coding: utf-8 -*-
"""
下载QuickJS缺失的头文件
"""
import os
import urllib.request

def download_file(url, dest_path):
    """下载文件到指定路径"""
    try:
        print(f"正在下载: {url}")
        urllib.request.urlretrieve(url, dest_path)
        print(f"下载完成: {dest_path}")
        return True
    except Exception as e:
        print(f"下载失败: {e}")
        return False

def main():
    """主函数"""
    print("开始下载QuickJS缺失的头文件...")
    
    # QuickJS GitHub仓库的原始文件链接
    base_url = "https://raw.githubusercontent.com/bellard/quickjs/master/"
    
    # 需要下载的头文件
    files_to_download = [
        "list.h",
        "dtoa.h",
        "quickjs-opcode.h",
        "libbf.h"
    ]
    
    # 创建quickjs目录
    quickjs_dir = "quickjs"
    if not os.path.exists(quickjs_dir):
        os.makedirs(quickjs_dir)
    
    # 下载每个文件
    success_count = 0
    for filename in files_to_download:
        url = base_url + filename
        dest_path = os.path.join(quickjs_dir, filename)
        if download_file(url, dest_path):
            success_count += 1
    
    print(f"\n完成！成功下载 {success_count}/{len(files_to_download)} 个文件")

if __name__ == '__main__':
    main()
