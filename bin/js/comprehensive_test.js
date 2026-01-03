// 综合测试脚本
// 测试加载模型、设置灯光、材质和按钮的完整流程

console.log("=== 综合测试开始 ===");

// 第一步: 加载模型
console.log("\n=== 第一步: 加载模型 ===");
let modelPath = "models/cube.obj";
let loadResult = loadOBJModel(modelPath);

if (loadResult) {
    console.log("✓ 模型加载成功: " + modelPath);
    
    let vertexCount = getVertexCount();
    let faceCount = getFaceCount();
    console.log("  顶点数量: " + vertexCount);
    console.log("  面数量: " + faceCount);
} else {
    console.log("✗ 模型加载失败");
}

// 第二步: 设置灯光
console.log("\n=== 第二步: 设置灯光 ===");
console.log("设置主光源位置...");
setLightPosition(10.0, 10.0, 10.0);
console.log("✓ 主光源位置: (10.0, 10.0, 10.0)");

console.log("设置环境光...");
setAmbientLight(0.3, 0.3, 0.3);
console.log("✓ 环境光: (0.3, 0.3, 0.3)");

console.log("设置漫反射光...");
setDiffuseLight(1.0, 1.0, 1.0);
console.log("✓ 漫反射光: (1.0, 1.0, 1.0)");

console.log("设置镜面反射光...");
setSpecularLight(0.8, 0.8, 0.8);
console.log("✓ 镜面反射光: (0.8, 0.8, 0.8)");

console.log("设置光源强度...");
setLightIntensity(1.0);
console.log("✓ 光源强度: 1.0");

// 第三步: 设置材质
console.log("\n=== 第三步: 设置材质 ===");
console.log("设置漫反射颜色为蓝色...");
setMaterialDiffuse(0.2, 0.4, 0.8);
console.log("✓ 漫反射颜色: (0.2, 0.4, 0.8)");

console.log("设置环境光颜色...");
setMaterialAmbient(0.1, 0.2, 0.4);
console.log("✓ 环境光颜色: (0.1, 0.2, 0.4)");

console.log("设置镜面反射颜色...");
setMaterialSpecular(1.0, 1.0, 1.0);
console.log("✓ 镜面反射颜色: (1.0, 1.0, 1.0)");

console.log("设置光泽度...");
setMaterialShininess(64.0);
console.log("✓ 光泽度: 64.0");

// 第四步: 创建按钮
console.log("\n=== 第四步: 创建按钮 ===");
let button1 = createButton("加载模型");
let button2 = createButton("设置灯光");
let button3 = createButton("设置材质");
let button4 = createButton("重置场景");

console.log("✓ 创建按钮: " + button1);
console.log("✓ 创建按钮: " + button2);
console.log("✓ 创建按钮: " + button3);
console.log("✓ 创建按钮: " + button4);

// 设置按钮位置
setButtonPosition(button1, 10, 10);
setButtonPosition(button2, 10, 50);
setButtonPosition(button3, 10, 90);
setButtonPosition(button4, 10, 130);
console.log("✓ 按钮位置已设置");

// 设置按钮大小
setButtonSize(button1, 100, 30);
setButtonSize(button2, 100, 30);
setButtonSize(button3, 100, 30);
setButtonSize(button4, 100, 30);
console.log("✓ 按钮大小已设置");

// 设置按钮颜色
setButtonColor(button1, 0.2, 0.6, 0.8);
setButtonColor(button2, 0.8, 0.6, 0.2);
setButtonColor(button3, 0.2, 0.8, 0.4);
setButtonColor(button4, 0.8, 0.2, 0.4);
console.log("✓ 按钮颜色已设置");

// 第五步: 测试不同场景
console.log("\n=== 第五步: 测试不同场景 ===");

// 场景1: 白色塑料
console.log("\n场景1: 白色塑料材质");
setMaterialDiffuse(0.9, 0.9, 0.9);
setMaterialAmbient(0.2, 0.2, 0.2);
setMaterialSpecular(0.3, 0.3, 0.3);
setMaterialShininess(16.0);
console.log("✓ 白色塑料材质已设置");

// 场景2: 金属材质
console.log("\n场景2: 银色金属材质");
setMaterialDiffuse(0.8, 0.8, 0.8);
setMaterialAmbient(0.2, 0.2, 0.2);
setMaterialSpecular(1.0, 1.0, 1.0);
setMaterialShininess(128.0);
console.log("✓ 银色金属材质已设置");

// 场景3: 橡胶材质
console.log("\n场景3: 红色橡胶材质");
setMaterialDiffuse(0.8, 0.1, 0.1);
setMaterialAmbient(0.1, 0.0, 0.0);
setMaterialSpecular(0.1, 0.1, 0.1);
setMaterialShininess(4.0);
console.log("✓ 红色橡胶材质已设置");

// 第六步: 测试不同灯光位置
console.log("\n=== 第六步: 测试不同灯光位置 ===");
let lightPositions = [
    [5.0, 5.0, 5.0],
    [-5.0, 10.0, 5.0],
    [0.0, 15.0, 0.0]
];

for (let i = 0; i < lightPositions.length; i++) {
    setLightPosition(lightPositions[i][0], lightPositions[i][1], lightPositions[i][2]);
    console.log("✓ 灯光位置" + (i + 1) + ": (" + lightPositions[i].join(", ") + ")");
}

// 恢复默认灯光
setLightPosition(10.0, 10.0, 10.0);
console.log("✓ 恢复默认灯光位置");

console.log("\n=== 综合测试完成 ===");
console.log("✓ 所有测试步骤已执行");
console.log("提示: 可以在3D视图和GUI窗口中查看效果");
