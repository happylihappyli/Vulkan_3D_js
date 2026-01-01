// 测试加载带材质的模型
print("开始加载带材质的模型...");

// 加载 colored_cube.obj
// 路径是相对于可执行文件或工作目录的
// 假设 script_viewer.exe 在 bin/ 下运行，工作目录是项目根目录
var modelPath = "assets/colored_cube.obj";

if (loadOBJModel(modelPath)) {
    print("模型加载成功: " + modelPath);
} else {
    print("模型加载失败: " + modelPath);
}
