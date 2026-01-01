// JavaScript测试脚本 - 简单测试

log("=== 简单测试开始 ===")

// 测试1: 基本日志
log("测试1: 基本日志")

// 测试2: 数学计算
log("测试2: 数学计算")
var result
result = calculate(10, 20)
log("结果: ")
log(result)

// 测试3: 获取顶点数量
log("测试3: 获取顶点数量")
var vertexCount
vertexCount = getVertexCount()
log("顶点数量: ")
log(vertexCount)

// 测试4: 获取面数量
log("测试4: 获取面数量")
var faceCount
faceCount = getFaceCount()
log("面数量: ")
log(faceCount)

// 测试5: 加载模型
log("测试5: 加载模型")
var success
success = loadOBJModel("test/test_cube.obj")
log("加载结果: ")
log(success)

// 测试6: 再次获取顶点数量
log("测试6: 再次获取顶点数量")
vertexCount = getVertexCount()
log("顶点数量: ")
log(vertexCount)

// 测试7: 再次获取面数量
log("测试7: 再次获取面数量")
faceCount = getFaceCount()
log("面数量: ")
log(faceCount)

// 测试8: 获取顶点数据
log("测试8: 获取顶点数据")
var vertices
vertices = getVertices()
log("顶点数据长度: ")
log(vertices.length)
log("前6个顶点坐标: ")
log(vertices[0])
log(vertices[1])
log(vertices[2])
log(vertices[3])
log(vertices[4])
log(vertices[5])

// 测试9: 获取法线数据
log("测试9: 获取法线数据")
var normals
normals = getNormals()
log("法线数据长度: ")
log(normals.length)
log("前6个法线坐标: ")
log(normals[0])
log(normals[1])
log(normals[2])
log(normals[3])
log(normals[4])
log(normals[5])

log("=== 测试完成 ===")
