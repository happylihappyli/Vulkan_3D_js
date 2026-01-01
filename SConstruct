# -*- coding: utf-8 -*-
# SCons构建脚本
import os
import time
from datetime import datetime

# 设置编码为UTF-8
import sys
if sys.version_info >= (3, 0):
    sys.stdout.reconfigure(encoding='utf-8')

# 开始时间
start_time = time.time()
print(f"开始编译: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")

# 环境变量
env = Environment(
    ENV = os.environ,
    CPPPATH = ['src', 'include', 'quickjs']
)

# Windows平台特定设置
if env['PLATFORM'] == 'win32':
    # MSVC编译器参数（移除UNICODE以匹配ANSI代码）
    env.Append(CCFLAGS=['/std:c++20', '/O2', '/W3', '/EHsc', '/utf-8'])  # 添加UTF-8编译选项
    # 移除错误的链接器参数
else:
    # GCC/Clang编译器参数
    env.Append(CCFLAGS=['-std=c++20', '-O2', '-Wall'])
    env.Append(LINKFLAGS=['-static'])

# 创建目录
if not os.path.exists('bin'):
    os.makedirs('bin')
if not os.path.exists('obj'):
    os.makedirs('obj')
if not os.path.exists('src'):
    os.makedirs('src')
if not os.path.exists('include'):
    os.makedirs('include')
if not os.path.exists('test'):
    os.makedirs('test')

# 设置目标路径
env['OBJPREFIX'] = 'obj/'

# Windows平台添加Win32 API库
if env['PLATFORM'] == 'win32':
    env.Append(LIBS=['user32', 'gdi32'])

# 构建目标 - Win32 API图形界面版本
win32_sources = ['src/win32_viewer.cpp']
win32_program = env.Program(target='bin/pointcloud_viewer', source=win32_sources)

# 构建目标 - OpenGL GPU加速版本（集成OBJ模型加载器和QuickJS）
opengl_sources = ['src/main.cpp', 
                 'src/camera.cpp', 'src/pointcloud.cpp', 
                 'src/model_renderer.cpp', 'src/utils.cpp', 'src/viewer.cpp',
                 'src/gl_extensions.cpp', 'src/model_loader_js.cpp',
                 'src/quickjs_engine.cpp']
opengl_env = env.Clone()
if env['PLATFORM'] == 'win32':
    opengl_env.Append(LIBS=['user32', 'gdi32', 'opengl32', 'glu32'])
    # 添加编译选项
    opengl_env.Append(CCFLAGS=['/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION=\"2024-01-13\"', '/DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/libunicode.c',
        'quickjs/cutils.c',
        'quickjs/dtoa.c',
        'quickjs/pthread_compat.c',
        'quickjs/sys_time_compat.c',
        'quickjs/dirent_compat.c',
        'quickjs/compat.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_sources:
        # 为C文件创建单独的环境，使用C11标准
        c_env = opengl_env.Clone()
        c_env['CCFLAGS'] = ['/std:c11', '/O2', '/W3', '/utf-8', '/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS']
        obj = c_env.Object(source=source)
        opengl_sources.append(obj)
    opengl_program = opengl_env.Program(target='bin/test', source=opengl_sources)
else:
    opengl_env.Append(LIBS=['m'])
    # 添加QuickJS源文件
    quickjs_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/cutils.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_sources:
        obj = opengl_env.Object(source=source)
        opengl_sources.append(obj)
    opengl_program = opengl_env.Program(target='bin/test', source=opengl_sources)

# 构建目标 - UTF-8控制台测试程序
console_test_sources = ['src/console_test.cpp']
console_test_env = env.Clone()
if env['PLATFORM'] == 'win32':
    console_test_env.Append(LIBS=['user32'])
console_test_program = console_test_env.Program(target='bin/console_test', source=console_test_sources)

# 构建目标 - 直接中文输出测试程序
direct_chinese_sources = ['test/direct_chinese_test.cpp']
direct_chinese_env = env.Clone()
if env['PLATFORM'] == 'win32':
    direct_chinese_env.Append(LIBS=['user32'])
direct_chinese_program = direct_chinese_env.Program(target='bin/chinese_test', source=direct_chinese_sources)

# 构建目标 - QuickJS最小测试程序
quickjs_minimal_test_sources = ['test/quickjs_minimal_test.cpp']
quickjs_minimal_test_env = env.Clone()
if env['PLATFORM'] == 'win32':
    quickjs_minimal_test_env.Append(LIBS=['user32'])
    quickjs_minimal_test_env.Append(CCFLAGS=['/DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_minimal_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/libunicode.c',
        'quickjs/cutils.c',
        'quickjs/dtoa.c',
        'quickjs/pthread_compat.c',
        'quickjs/sys_time_compat.c',
        'quickjs/dirent_compat.c',
        'quickjs/compat.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_minimal_sources:
        c_env = quickjs_minimal_test_env.Clone()
        c_env['CCFLAGS'] = ['/std:c11', '/O2', '/W3', '/utf-8', '/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS']
        obj = c_env.Object(source=source)
        quickjs_minimal_test_sources.append(obj)
else:
    quickjs_minimal_test_env.Append(LIBS=['m'])
    quickjs_minimal_test_env.Append(CCFLAGS=['-DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_minimal_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/cutils.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_minimal_sources:
        obj = quickjs_minimal_test_env.Object(source=source)
        quickjs_minimal_test_sources.append(obj)
quickjs_minimal_test_program = quickjs_minimal_test_env.Program(target='bin/quickjs_minimal_test', source=quickjs_minimal_test_sources)

# 构建目标 - QuickJS简单测试程序
quickjs_simple_test_sources = ['test/quickjs_simple_test.cpp']
quickjs_simple_test_env = env.Clone()
if env['PLATFORM'] == 'win32':
    quickjs_simple_test_env.Append(LIBS=['user32'])
    quickjs_simple_test_env.Append(CCFLAGS=['/DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_simple_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/libunicode.c',
        'quickjs/cutils.c',
        'quickjs/dtoa.c',
        'quickjs/pthread_compat.c',
        'quickjs/sys_time_compat.c',
        'quickjs/dirent_compat.c',
        'quickjs/compat.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_simple_sources:
        c_env = quickjs_simple_test_env.Clone()
        c_env['CCFLAGS'] = ['/std:c11', '/O2', '/W3', '/utf-8', '/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS']
        obj = c_env.Object(source=source)
        quickjs_simple_test_sources.append(obj)
else:
    quickjs_simple_test_env.Append(LIBS=['m'])
    quickjs_simple_test_env.Append(CCFLAGS=['-DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_simple_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/cutils.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_simple_sources:
        obj = quickjs_simple_test_env.Object(source=source)
        quickjs_simple_test_sources.append(obj)
quickjs_simple_test_program = quickjs_simple_test_env.Program(target='bin/quickjs_simple_test', source=quickjs_simple_test_sources)

# 构建目标 - QuickJS上下文测试程序
quickjs_context_test_sources = ['test/quickjs_context_test.cpp']
quickjs_context_test_env = env.Clone()
if env['PLATFORM'] == 'win32':
    quickjs_context_test_env.Append(LIBS=['user32'])
    quickjs_context_test_env.Append(CCFLAGS=['/DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_context_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/libunicode.c',
        'quickjs/cutils.c',
        'quickjs/dtoa.c',
        'quickjs/pthread_compat.c',
        'quickjs/sys_time_compat.c',
        'quickjs/dirent_compat.c',
        'quickjs/compat.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_context_sources:
        c_env = quickjs_context_test_env.Clone()
        c_env['CCFLAGS'] = ['/std:c11', '/O2', '/W3', '/utf-8', '/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS']
        obj = c_env.Object(source=source)
        quickjs_context_test_sources.append(obj)
else:
    quickjs_context_test_env.Append(LIBS=['m'])
    quickjs_context_test_env.Append(CCFLAGS=['-DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_context_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/cutils.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_context_sources:
        obj = quickjs_context_test_env.Object(source=source)
        quickjs_context_test_sources.append(obj)
quickjs_context_test_program = quickjs_context_test_env.Program(target='bin/quickjs_context_test', source=quickjs_context_test_sources)

# 构建目标 - QuickJS分步测试程序
quickjs_step_test_sources = ['test/quickjs_step_test.cpp']
quickjs_step_test_env = env.Clone()
if env['PLATFORM'] == 'win32':
    quickjs_step_test_env.Append(LIBS=['user32'])
    quickjs_step_test_env.Append(CCFLAGS=['/DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_step_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/libunicode.c',
        'quickjs/cutils.c',
        'quickjs/dtoa.c',
        'quickjs/pthread_compat.c',
        'quickjs/sys_time_compat.c',
        'quickjs/dirent_compat.c',
        'quickjs/compat.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_step_sources:
        c_env = quickjs_step_test_env.Clone()
        c_env['CCFLAGS'] = ['/std:c11', '/O2', '/W3', '/utf-8', '/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS']
        obj = c_env.Object(source=source)
        quickjs_step_test_sources.append(obj)
else:
    quickjs_step_test_env.Append(LIBS=['m'])
    quickjs_step_test_env.Append(CCFLAGS=['-DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_step_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/cutils.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_step_sources:
        obj = quickjs_step_test_env.Object(source=source)
        quickjs_step_test_sources.append(obj)
quickjs_step_test_program = quickjs_step_test_env.Program(target='bin/quickjs_step_test', source=quickjs_step_test_sources)

# 构建目标 - QuickJS Atom测试程序
quickjs_atom_test_sources = ['test/quickjs_atom_test.cpp']
quickjs_atom_test_env = env.Clone()
if env['PLATFORM'] == 'win32':
    quickjs_atom_test_env.Append(LIBS=['user32'])
    quickjs_atom_test_env.Append(CCFLAGS=['/DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_atom_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/libunicode.c',
        'quickjs/cutils.c',
        'quickjs/dtoa.c',
        'quickjs/pthread_compat.c',
        'quickjs/sys_time_compat.c',
        'quickjs/dirent_compat.c',
        'quickjs/compat.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_atom_sources:
        c_env = quickjs_atom_test_env.Clone()
        c_env['CCFLAGS'] = ['/std:c11', '/O2', '/W3', '/utf-8', '/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS']
        obj = c_env.Object(source=source)
        quickjs_atom_test_sources.append(obj)
else:
    quickjs_atom_test_env.Append(LIBS=['m'])
    quickjs_atom_test_env.Append(CCFLAGS=['-DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_atom_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/cutils.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_atom_sources:
        obj = quickjs_atom_test_env.Object(source=source)
        quickjs_atom_test_sources.append(obj)
quickjs_atom_test_program = quickjs_atom_test_env.Program(target='bin/quickjs_atom_test', source=quickjs_atom_test_sources)

# 构建目标 - QuickJS模型加载测试程序
quickjs_model_test_sources = ['test/quickjs_model_test.cpp', 'src/quickjs_vulkan_bridge.cpp', 'src/model_loader_js.cpp']
quickjs_model_test_env = env.Clone()
if env['PLATFORM'] == 'win32':
    quickjs_model_test_env.Append(LIBS=['user32'])
    quickjs_model_test_env.Append(CCFLAGS=['/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_model_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/libunicode.c',
        'quickjs/cutils.c',
        'quickjs/dtoa.c',
        'quickjs/pthread_compat.c',
        'quickjs/sys_time_compat.c',
        'quickjs/dirent_compat.c',
        'quickjs/compat.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_model_sources:
        c_env = quickjs_model_test_env.Clone()
        c_env['CCFLAGS'] = ['/std:c11', '/O2', '/W3', '/utf-8', '/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS']
        obj = c_env.Object(source=source)
        quickjs_model_test_sources.append(obj)
else:
    quickjs_model_test_env.Append(LIBS=['m'])
    quickjs_model_test_env.Append(CCFLAGS=['-DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_model_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/cutils.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_model_sources:
        obj = quickjs_model_test_env.Object(source=source)
        quickjs_model_test_sources.append(obj)
quickjs_model_test_program = quickjs_model_test_env.Program(target='bin/quickjs_model_test', source=quickjs_model_test_sources)

# 构建目标 - QuickJS基本测试程序
quickjs_basic_test_sources = ['test/quickjs_basic_test.cpp']
quickjs_basic_test_env = env.Clone()
if env['PLATFORM'] == 'win32':
    quickjs_basic_test_env.Append(LIBS=['user32'])
    quickjs_basic_test_env.Append(CCFLAGS=['/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_basic_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/libunicode.c',
        'quickjs/cutils.c',
        'quickjs/dtoa.c',
        'quickjs/pthread_compat.c',
        'quickjs/sys_time_compat.c',
        'quickjs/dirent_compat.c',
        'quickjs/compat.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_basic_sources:
        c_env = quickjs_basic_test_env.Clone()
        c_env['CCFLAGS'] = ['/std:c11', '/O2', '/W3', '/utf-8', '/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS']
        obj = c_env.Object(source=source)
        quickjs_basic_test_sources.append(obj)
else:
    quickjs_basic_test_env.Append(LIBS=['m'])
    quickjs_basic_test_env.Append(CCFLAGS=['-DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_basic_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/cutils.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_basic_sources:
        obj = quickjs_basic_test_env.Object(source=source)
        quickjs_basic_test_sources.append(obj)
quickjs_basic_test_program = quickjs_basic_test_env.Program(target='bin/quickjs_basic_test', source=quickjs_basic_test_sources)

# 构建目标 - QuickJS测试程序
quickjs_test_sources = ['test/quickjs_test.cpp']
quickjs_test_env = env.Clone()
if env['PLATFORM'] == 'win32':
    quickjs_test_env.Append(LIBS=['user32'])
    quickjs_test_env.Append(CCFLAGS=['/DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/libunicode.c',
        'quickjs/cutils.c',
        'quickjs/dtoa.c',
        'quickjs/pthread_compat.c',
        'quickjs/sys_time_compat.c',
        'quickjs/dirent_compat.c',
        'quickjs/compat.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_sources:
        c_env = quickjs_test_env.Clone()
        c_env['CCFLAGS'] = ['/std:c11', '/O2', '/W3', '/utf-8', '/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS']
        obj = c_env.Object(source=source)
        quickjs_test_sources.append(obj)
else:
    quickjs_test_env.Append(LIBS=['m'])
    quickjs_test_env.Append(CCFLAGS=['-DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    quickjs_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/cutils.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in quickjs_sources:
        obj = quickjs_test_env.Object(source=source)
        quickjs_test_sources.append(obj)
quickjs_test_program = quickjs_test_env.Program(target='bin/quickjs_test', source=quickjs_test_sources)

# 构建目标 - Vulkan GPU加速版本
vulkan_sources = ['src/vulkan_viewer.cpp']
vulkan_env = env.Clone()
if env['PLATFORM'] == 'win32':
    vulkan_env.Append(LIBS=['user32', 'gdi32', 'vulkan-1'])
vulkan_program = vulkan_env.Program(target='bin/vulkan_pointcloud_viewer', source=vulkan_sources)

# 构建目标 - JavaScript脚本查看器（带UI界面）
script_viewer_sources = [
    'src/script_viewer_main.cpp', 
    'src/script_viewer_app.cpp', 
    'src/quickjs_vulkan_bridge.cpp', 
    'src/model_loader_js.cpp',
    'src/model_renderer_3d.cpp',
    'C:/Users/happyli/.conan2/p/imgui12fefc76fc6c6/p/res/bindings/imgui_impl_glfw.cpp',
    'C:/Users/happyli/.conan2/p/imgui12fefc76fc6c6/p/res/bindings/imgui_impl_opengl3.cpp'
]
script_viewer_env = env.Clone()
script_viewer_env['OBJPREFIX'] = 'obj/script_viewer_'
if env['PLATFORM'] == 'win32':
    # 添加GLFW、ImGui和GLAD的include路径
    script_viewer_env.Append(CPPPATH=[
        'C:/Users/happyli/.conan2/p/glfw7e74c5794304a/p/include',
        'C:/Users/happyli/.conan2/p/imgui12fefc76fc6c6/p/include',
        'C:/Users/happyli/.conan2/p/imgui12fefc76fc6c6/p/res/bindings',
        'C:/Users/happyli/.conan2/p/glad6f74e29c693f3/p/include'
    ])
    # 添加GLFW、ImGui和GLAD的库路径
    script_viewer_env.Append(LIBPATH=[
        'C:/Users/happyli/.conan2/p/glfw7e74c5794304a/p/lib',
        'C:/Users/happyli/.conan2/p/imgui12fefc76fc6c6/p/lib',
        'C:/Users/happyli/.conan2/p/glad6f74e29c693f3/p/lib'
    ])
    # 添加库文件
    script_viewer_env.Append(LIBS=['user32', 'gdi32', 'opengl32', 'glu32', 'comdlg32', 'shell32', 'shlwapi', 'glfw3', 'imgui', 'glad', 'legacy_stdio_definitions', 'ucrt', 'msvcrt'])
    script_viewer_env.Append(CCFLAGS=['/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS', '/DGLFW_INCLUDE_NONE'])
    # 添加QuickJS源文件
    script_viewer_quickjs_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/libunicode.c',
        'quickjs/cutils.c',
        'quickjs/dtoa.c',
        'quickjs/pthread_compat.c',
        'quickjs/sys_time_compat.c',
        'quickjs/dirent_compat.c',
        'quickjs/compat.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in script_viewer_quickjs_sources:
        c_env = script_viewer_env.Clone()
        c_env['CCFLAGS'] = ['/std:c11', '/O2', '/W3', '/utf-8', '/D_CRT_SECURE_NO_WARNINGS', '/DCONFIG_VERSION="2024-01-13"', '/DENABLE_QUICKJS']
        obj = c_env.Object(source=source)
        script_viewer_sources.append(obj)
else:
    script_viewer_env.Append(LIBS=['m'])
    script_viewer_env.Append(CCFLAGS=['-DENABLE_QUICKJS'])
    # 添加QuickJS源文件
    script_viewer_quickjs_sources = [
        'quickjs/quickjs.c',
        'quickjs/quickjs-libc.c',
        'quickjs/libregexp.c',
        'quickjs/cutils.c'
    ]
    # 为每个QuickJS源文件设置C编译器
    for source in script_viewer_quickjs_sources:
        c_env = script_viewer_env.Clone()
        c_env['OBJPREFIX'] = 'obj/script_viewer_'
        obj = c_env.Object(source=source)
        script_viewer_sources.append(obj)
script_viewer_program = script_viewer_env.Program(target='bin/script_viewer', source=script_viewer_sources)

# 结束时间
end_time = time.time()
print(f"编译完成: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
print(f"耗时: {end_time - start_time:.2f} 秒")

# 默认目标 - 构建所有程序
if 'script_viewer_program' in locals():
    Default(script_viewer_program)
elif 'quickjs_basic_test_program' in locals():
    Default(quickjs_basic_test_program)
elif 'quickjs_model_test_program' in locals():
    Default(quickjs_model_test_program)
elif 'quickjs_atom_test_program' in locals():
    Default(quickjs_atom_test_program)
elif 'quickjs_step_test_program' in locals():
    Default(quickjs_step_test_program)
elif 'quickjs_context_test_program' in locals():
    Default(quickjs_context_test_program)
elif 'quickjs_minimal_test_program' in locals():
    Default(quickjs_minimal_test_program)
elif 'quickjs_simple_test_program' in locals():
    Default(quickjs_simple_test_program)
elif 'quickjs_test_program' in locals():
    Default(quickjs_test_program)
elif 'vulkan_program' in locals():
    Default(vulkan_program)
elif 'opengl_program' in locals():
    Default(opengl_program)
elif 'console_test_program' in locals():
    Default(console_test_program)
elif 'win32_program' in locals():
    Default(win32_program)
else:
    print("没有找到有效的构建目标")