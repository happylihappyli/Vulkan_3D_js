console.log("开始加载OBJ模型...");

var modelPath = "models/cube.obj";

console.log("正在加载模型: " + modelPath);

var result = loadOBJModel(modelPath);

if (result) {
    console.log("模型加载成功！");
    
    var vertexCount = getVertexCount();
    var faceCount = getFaceCount();
    
    console.log("顶点数量: " + vertexCount);
    console.log("面数量: " + faceCount);
    
    console.log("模型应该已经在窗口中显示了");
} else {
    console.log("模型加载失败！");
}

console.log("脚本执行完毕");
