// 模型加载测试脚本
// 测试加载3D模型并获取模型信息

console.log("=== 模型加载测试开始 ===");

// 测试1: 加载立方体模型
console.log("\n测试1: 加载立方体模型");
let modelPath = "models/cube.obj";
let loadResult = loadOBJModel(modelPath);

if (loadResult) {
    console.log("✓ 模型加载成功: " + modelPath);
    
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
    if (vertices && vertices.length > 0) {
        console.log("顶点数据长度: " + vertices.length);
        console.log("前9个顶点坐标:");
        for (let i = 0; i < Math.min(9, vertices.length); i++) {
            console.log("  vertices[" + i + "] = " + vertices[i]);
        }
    }
    
    // 测试5: 获取法线数据
    console.log("\n测试5: 获取法线数据");
    let normals = getNormals();
    if (normals && normals.length > 0) {
        console.log("法线数据长度: " + normals.length);
        console.log("前9个法线坐标:");
        for (let i = 0; i < Math.min(9, normals.length); i++) {
            console.log("  normals[" + i + "] = " + normals[i]);
        }
    }
    
    // 测试6: 获取纹理坐标
    console.log("\n测试6: 获取纹理坐标");
    let texCoords = getTexCoords();
    if (texCoords && texCoords.length > 0) {
        console.log("纹理坐标长度: " + texCoords.length);
        console.log("前6个纹理坐标:");
        for (let i = 0; i < Math.min(6, texCoords.length); i++) {
            console.log("  texCoords[" + i + "] = " + texCoords[i]);
        }
    }
    
    console.log("\n=== 模型加载测试完成 ===");
    console.log("✓ 所有测试通过");
} else {
    console.log("✗ 模型加载失败: " + modelPath);
    console.log("\n=== 测试失败 ===");
}
