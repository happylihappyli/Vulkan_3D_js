// 测试脚本 - 不使用ES6特性

console.log("=== 测试开始 ===");

// 测试1: 加载立方体模型
console.log("\n测试1: 加载立方体模型");
var success = loadOBJModel("test/cube.obj");
console.log("加载结果: " + (success ? "成功" : "失败"));

if (success) {
    // 测试2: 获取顶点数量
    console.log("\n测试2: 获取顶点数量");
    var vertexCount = getVertexCount();
    console.log("顶点数量: " + vertexCount);
    
    // 测试3: 获取面数量
    console.log("\n测试3: 获取面数量");
    var faceCount = getFaceCount();
    console.log("面数量: " + faceCount);
    
    // 测试4: 获取顶点数据
    console.log("\n测试4: 获取顶点数据");
    var vertices = getVertices();
    if (vertices) {
        console.log("顶点数据长度: " + vertices.length);
        console.log("前9个顶点坐标:");
        var i;
        for (i = 0; i < 9 && i < vertices.length; i++) {
            console.log("  vertices[" + i + "] = " + vertices[i]);
        }
    }
    
    // 测试5: 获取法线数据
    console.log("\n测试5: 获取法线数据");
    var normals = getNormals();
    if (normals) {
        console.log("法线数据长度: " + normals.length);
        if (normals.length > 0) {
            console.log("前9个法线坐标:");
            var j;
            for (j = 0; j < 9 && j < normals.length; j++) {
                console.log("  normals[" + j + "] = " + normals[j]);
            }
        }
    }
    
    console.log("\n=== 测试完成 ===");
    console.log("立方体模型加载成功，可以在3D视图中查看渲染效果");
    console.log("请查看窗口中的3D渲染结果");
} else {
    console.log("\n=== 测试失败 ===");
    console.log("无法加载立方体模型");
}
