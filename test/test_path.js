console.log("测试路径解析...");

// 测试不同的路径
var paths = [
    "bin/models/cube.obj",
    "models/cube.obj",
    "./models/cube.obj",
    "../bin/models/cube.obj"
];

for (var i = 0; i < paths.length; i++) {
    console.log("尝试路径: " + paths[i]);
    var result = loadOBJModel(paths[i]);
    if (result) {
        console.log("成功！使用路径: " + paths[i]);
        break;
    } else {
        console.log("失败");
    }
}

console.log("测试完毕");
