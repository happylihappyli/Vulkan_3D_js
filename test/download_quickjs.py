#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
下载并集成QuickJS源码到项目
"""

import os
import urllib.request
import zipfile
import shutil

def download_quickjs():
    """下载QuickJS源码"""
    print("开始下载QuickJS源码...")
    
    # QuickJS官方GitHub仓库
    quickjs_url = "https://codeload.github.com/bellard/quickjs/zip/refs/heads/master"
    zip_file = "quickjs-master.zip"
    
    try:
        # 下载源码
        urllib.request.urlretrieve(quickjs_url, zip_file)
        print("QuickJS源码下载完成")
        
        # 解压源码
        with zipfile.ZipFile(zip_file, 'r') as zip_ref:
            zip_ref.extractall(".")
        print("QuickJS源码解压完成")
        
        # 创建quickjs目录
        quickjs_dir = "quickjs"
        if os.path.exists(quickjs_dir):
            shutil.rmtree(quickjs_dir)
        os.makedirs(quickjs_dir)
        
        # 复制必要的源码文件
        src_files = [
            "quickjs-master/quickjs.c",
            "quickjs-master/quickjs.h", 
            "quickjs-master/quickjs-libc.c",
            "quickjs-master/quickjs-libc.h",
            "quickjs-master/cutils.c",
            "quickjs-master/cutils.h",
            "quickjs-master/libregexp.c",
            "quickjs-master/libregexp.h",
            "quickjs-master/libunicode.c",
            "quickjs-master/libunicode.h"
        ]
        
        for src_file in src_files:
            if os.path.exists(src_file):
                shutil.copy2(src_file, quickjs_dir)
                print(f"复制文件: {src_file}")
        
        # 清理临时文件
        if os.path.exists(zip_file):
            os.remove(zip_file)
        if os.path.exists("quickjs-master"):
            shutil.rmtree("quickjs-master")
            
        print("QuickJS源码集成完成")
        return True
        
    except Exception as e:
        print(f"下载QuickJS源码失败: {e}")
        return False

def create_quickjs_wrapper():
    """创建QuickJS C++包装器"""
    print("创建QuickJS C++包装器...")
    
    wrapper_content = '''// QuickJS C++包装器
// 提供C++友好的JavaScript引擎接口

#ifndef QUICKJS_WRAPPER_H
#define QUICKJS_WRAPPER_H

#include <string>
#include <vector>
#include <memory>

// QuickJS头文件
#include "quickjs.h"
#include "quickjs-libc.h"

class QuickJSEngine {
private:
    JSRuntime* m_runtime;
    JSContext* m_context;
    
public:
    QuickJSEngine();
    ~QuickJSEngine();
    
    // 初始化JavaScript引擎
    bool initialize();
    
    // 执行JavaScript代码
    bool executeScript(const std::string& script);
    
    // 执行JavaScript文件
    bool executeFile(const std::string& filename);
    
    // 注册C++函数到JavaScript环境
    bool registerFunction(const std::string& name, JSCFunction* func, int argc);
    
    // 获取JavaScript执行结果
    std::string getResult();
    
    // 检查JavaScript执行是否出错
    bool hasError() const;
    
    // 获取错误信息
    std::string getError() const;
};

// 3D模型加载相关的JavaScript绑定
class ModelLoaderJS {
private:
    std::unique_ptr<QuickJSEngine> m_engine;
    
public:
    ModelLoaderJS();
    
    // 加载OBJ模型文件
    bool loadOBJModel(const std::string& filename);
    
    // 获取顶点数据
    std::vector<float> getVertices() const;
    
    // 获取法线数据
    std::vector<float> getNormals() const;
    
    // 获取纹理坐标
    std::vector<float> getTexCoords() const;
    
    // 获取面索引
    std::vector<unsigned int> getIndices() const;
};

#endif // QUICKJS_WRAPPER_H
'''
    
    with open("include/quickjs_wrapper.h", "w", encoding="utf-8") as f:
        f.write(wrapper_content)
    
    print("QuickJS C++包装器创建完成")

if __name__ == "__main__":
    # 确保目录存在
    os.makedirs("quickjs", exist_ok=True)
    os.makedirs("include", exist_ok=True)
    
    if download_quickjs():
        create_quickjs_wrapper()
        print("QuickJS集成任务完成")
    else:
        print("QuickJS集成失败")
