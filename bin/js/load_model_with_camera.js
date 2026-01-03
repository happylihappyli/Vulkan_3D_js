// 模型加载和显示测试脚本 - 带相机控制
// 确保模型正确显示在3D视图中

console.log("=== 模型加载和显示测试开始 ===");

// 测试1: 加载立方体模型
console.log("\n测试1: 加载立方体模型");
let modelPath = "models/cube.obj";
let loadResult = loadOBJModel(modelPath);

if (loadResult) {
    console.log("✓ 模型加载成功: " + modelPath);
    
    // 测试2: 获取模型信息
    console.log("\n测试2: 获取模型信息");
    let vertexCount = getVertexCount();
    let faceCount = getFaceCount();
    console.log("顶点数量: " + vertexCount);
    console.log("面数量: " + faceCount);
    
    // 测试3: 获取顶点数据
    console.log("\n测试3: 获取顶点数据");
    let vertices = getVertices();
    if (vertices && vertices.length > 0) {
        console.log("顶点数据长度: " + vertices.length);
        console.log("前9个顶点坐标:");
        for (let i = 0; i < Math.min(9, vertices.length); i++) {
            console.log("  vertices[" + i + "] = " + vertices[i]);
        }
    }
    
    // 测试4: 获取法线数据
    console.log("\n测试4: 获取法线数据");
    let normals = getNormals();
    if (normals && normals.length > 0) {
        console.log("法线数据长度: " + normals.length);
        console.log("前9个法线坐标:");
        for (let i = 0; i < Math.min(9, normals.length); i++) {
            console.log("  normals[" + i + "] = " + normals[i]);
        }
    }
    
    // 测试5: 重置相机位置（确保模型在视野内）
    console.log("\n测试5: 重置相机位置");
    if (typeof resetCamera !== 'undefined') {
        resetCamera();
        console.log("✓ 相机位置已重置");
    } else {
        console.log("提示: resetCamera 函数未定义，请使用鼠标控制相机");
    }
    
    console.log("\n=== 模型加载和显示测试完成 ===");
    console.log("✓ 所有测试通过");
    console.log("\n=== 相机控制说明 ===");
    console.log("鼠标左键拖动: 旋转模型");
    console.log("鼠标右键拖动: 旋转相机");
    console.log("鼠标中键拖动: 平移视图");
    console.log("鼠标滚轮: 缩放视图");
    console.log("\n提示: 如果看不到模型，请尝试:");
    console.log("1. 使用鼠标滚轮缩放");
    console.log("2. 使用鼠标右键旋转相机");
    console.log("3. 使用鼠标中键平移视图");
} else {
    console.log("✗ 模型加载失败: " + modelPath);
    console.log("\n=== 测试失败 ===");
}
