console.log("开始测试3D模型加载");

// 加载OBJ模型
var result = loadOBJModel("models/cube.obj");
console.log("模型加载结果: " + result);

// 获取模型信息
var vertexCount = getVertexCount();
var faceCount = getFaceCount();

console.log("顶点数: " + vertexCount);
console.log("面数: " + faceCount);

console.log("3D模型加载测试完成");
