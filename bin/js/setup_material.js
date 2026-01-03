// 材料设置测试脚本
// 测试设置3D模型材质参数

console.log("=== 材料设置测试开始 ===");

// 测试1: 设置漫反射颜色
console.log("\n测试1: 设置漫反射颜色");
let diffuseColor = [1.0, 0.0, 0.0]; // 红色
let result = setMaterialDiffuse(diffuseColor[0], diffuseColor[1], diffuseColor[2]);
if (result) {
    console.log("✓ 漫反射颜色设置成功: 红色 (" + diffuseColor.join(", ") + ")");
} else {
    console.log("✗ 漫反射颜色设置失败");
}

// 测试2: 设置环境光颜色
console.log("\n测试2: 设置环境光颜色");
let ambientColor = [0.2, 0.2, 0.2];
result = setMaterialAmbient(ambientColor[0], ambientColor[1], ambientColor[2]);
if (result) {
    console.log("✓ 环境光颜色设置成功: (" + ambientColor.join(", ") + ")");
} else {
    console.log("✗ 环境光颜色设置失败");
}

// 测试3: 设置镜面反射颜色
console.log("\n测试3: 设置镜面反射颜色");
let specularColor = [1.0, 1.0, 1.0];
result = setMaterialSpecular(specularColor[0], specularColor[1], specularColor[2]);
if (result) {
    console.log("✓ 镜面反射颜色设置成功: (" + specularColor.join(", ") + ")");
} else {
    console.log("✗ 镜面反射颜色设置失败");
}

// 测试4: 设置光泽度
console.log("\n测试4: 设置光泽度");
let shininess = 32.0;
result = setMaterialShininess(shininess);
if (result) {
    console.log("✓ 光泽度设置成功: " + shininess);
} else {
    console.log("✗ 光泽度设置失败");
}

// 测试5: 测试不同材质颜色
console.log("\n测试5: 测试不同材质颜色");
let testColors = [
    { name: "红色", color: [1.0, 0.0, 0.0] },
    { name: "绿色", color: [0.0, 1.0, 0.0] },
    { name: "蓝色", color: [0.0, 0.0, 1.0] },
    { name: "黄色", color: [1.0, 1.0, 0.0] },
    { name: "青色", color: [0.0, 1.0, 1.0] },
    { name: "品红", color: [1.0, 0.0, 1.0] },
    { name: "白色", color: [1.0, 1.0, 1.0] },
    { name: "灰色", color: [0.5, 0.5, 0.5] }
];

for (let i = 0; i < testColors.length; i++) {
    let mat = testColors[i];
    result = setMaterialDiffuse(mat.color[0], mat.color[1], mat.color[2]);
    console.log("  " + mat.name + ": (" + mat.color.join(", ") + ") - " + (result ? "成功" : "失败"));
}

// 测试6: 测试不同光泽度
console.log("\n测试6: 测试不同光泽度");
let shininessValues = [0.0, 8.0, 16.0, 32.0, 64.0, 128.0];
for (let i = 0; i < shininessValues.length; i++) {
    let sh = shininessValues[i];
    result = setMaterialShininess(sh);
    console.log("  光泽度" + (i + 1) + ": " + sh + " - " + (result ? "成功" : "失败"));
}

// 测试7: 创建金属材质
console.log("\n测试7: 创建金属材质");
console.log("  设置银色金属材质");
setMaterialAmbient(0.2, 0.2, 0.2);
setMaterialDiffuse(0.8, 0.8, 0.8);
setMaterialSpecular(1.0, 1.0, 1.0);
setMaterialShininess(128.0);
console.log("  ✓ 银色金属材质设置完成");

console.log("  设置金色金属材质");
setMaterialAmbient(0.2, 0.15, 0.0);
setMaterialDiffuse(0.9, 0.7, 0.0);
setMaterialSpecular(1.0, 1.0, 0.8);
setMaterialShininess(128.0);
console.log("  ✓ 金色金属材质设置完成");

// 测试8: 创建塑料材质
console.log("\n测试8: 创建塑料材质");
console.log("  设置红色塑料材质");
setMaterialAmbient(0.2, 0.0, 0.0);
setMaterialDiffuse(0.8, 0.0, 0.0);
setMaterialSpecular(0.4, 0.4, 0.4);
setMaterialShininess(32.0);
console.log("  ✓ 红色塑料材质设置完成");

// 测试9: 恢复默认材质
console.log("\n测试9: 恢复默认材质");
result = resetMaterial();
if (result) {
    console.log("✓ 材质已恢复默认");
} else {
    console.log("✗ 恢复默认材质失败");
}

console.log("\n=== 材料设置测试完成 ===");
console.log("✓ 所有材质测试已执行");
console.log("提示: 可以在3D视图中观察不同材质的效果");
