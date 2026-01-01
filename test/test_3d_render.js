// 测试3D渲染功能 - 加载立方体模型并保持窗口打开

console.log("=== 3D渲染功能测试开始 ===");

// 测试1: 加载立方体模型
console.log("\n测试1: 加载立方体模型");
let success = loadOBJModel("test/cube.obj");
console.log("加载结果: " + (success ? "成功" : "失败"));

if (success) {
    // 测试2: 获取顶点数量
    console.log("\n测试2: 获取顶点数量");
    let vertexCount = getVertexCount();
    console.log("顶点数量: " + vertexCount);
    
    // 测试3: 获取面数量
    console.log("\n测试3: 获取面数量");
    let faceCount = getFaceCount();
    console.log("面数量: " + faceCount);
    
    // 测试4: 获取顶点数据
    console.log("\n测试4: 获取顶点数据");
    let vertices = getVertices();
    if (vertices) {
        console.log("顶点数据长度: " + vertices.length);
        console.log("前9个顶点坐标:");
        for (let i = 0; i < Math.min(9, vertices.length); i++) {
            console.log("  vertices[" + i + "] = " + vertices[i]);
        }
    }
    
    // 测试5: 获取法线数据
    console.log("\n测试5: 获取法线数据");
    let normals = getNormals();
    if (normals) {
        console.log("法线数据长度: " + normals.length);
        if (normals.length > 0) {
            console.log("前9个法线坐标:");
            for (let i = 0; i < Math.min(9, normals.length); i++) {
                console.log("  normals[" + i + "] = " + normals[i]);
            }
        }
    }
    
    console.log("\n=== 3D渲染功能测试完成 ===");
    console.log("立方体模型加载成功，可以在3D视图中查看渲染效果");
    console.log("请查看窗口中的3D渲染结果");
} else {
    console.log("\n=== 测试失败 ===");
    console.log("无法加载立方体模型");
}
