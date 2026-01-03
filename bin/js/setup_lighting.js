// 灯光设置测试脚本
// 测试设置3D场景中的灯光参数

console.log("=== 灯光设置测试开始 ===");

// 测试1: 设置主光源位置
console.log("\n测试1: 设置主光源位置");
let lightPos = [10.0, 10.0, 10.0];
let result = setLightPosition(lightPos[0], lightPos[1], lightPos[2]);
if (result) {
    console.log("✓ 主光源位置设置成功: (" + lightPos.join(", ") + ")");
} else {
    console.log("✗ 主光源位置设置失败");
}

// 测试2: 设置环境光
console.log("\n测试2: 设置环境光");
let ambient = [0.3, 0.3, 0.3];
result = setAmbientLight(ambient[0], ambient[1], ambient[2]);
if (result) {
    console.log("✓ 环境光设置成功: (" + ambient.join(", ") + ")");
} else {
    console.log("✗ 环境光设置失败");
}

// 测试3: 设置漫反射光
console.log("\n测试3: 设置漫反射光");
let diffuse = [1.0, 1.0, 1.0];
result = setDiffuseLight(diffuse[0], diffuse[1], diffuse[2]);
if (result) {
    console.log("✓ 漫反射光设置成功: (" + diffuse.join(", ") + ")");
} else {
    console.log("✗ 漫反射光设置失败");
}

// 测试4: 设置镜面反射光
console.log("\n测试4: 设置镜面反射光");
let specular = [0.8, 0.8, 0.8];
result = setSpecularLight(specular[0], specular[1], specular[2]);
if (result) {
    console.log("✓ 镜面反射光设置成功: (" + specular.join(", ") + ")");
} else {
    console.log("✗ 镜面反射光设置失败");
}

// 测试5: 设置光源强度
console.log("\n测试5: 设置光源强度");
let intensity = 1.0;
result = setLightIntensity(intensity);
if (result) {
    console.log("✓ 光源强度设置成功: " + intensity);
} else {
    console.log("✗ 光源强度设置失败");
}

// 测试6: 测试不同灯光位置
console.log("\n测试6: 测试不同灯光位置");
let testPositions = [
    [5.0, 5.0, 5.0],
    [-5.0, 10.0, 5.0],
    [0.0, 15.0, 0.0],
    [10.0, 0.0, 10.0]
];

for (let i = 0; i < testPositions.length; i++) {
    let pos = testPositions[i];
    result = setLightPosition(pos[0], pos[1], pos[2]);
    console.log("  位置" + (i + 1) + ": (" + pos.join(", ") + ") - " + (result ? "成功" : "失败"));
}

// 测试7: 测试不同环境光强度
console.log("\n测试7: 测试不同环境光强度");
let ambientIntensities = [
    [0.1, 0.1, 0.1],
    [0.2, 0.2, 0.2],
    [0.4, 0.4, 0.4],
    [0.6, 0.6, 0.6]
];

for (let i = 0; i < ambientIntensities.length; i++) {
    let amb = ambientIntensities[i];
    result = setAmbientLight(amb[0], amb[1], amb[2]);
    console.log("  环境光" + (i + 1) + ": (" + amb.join(", ") + ") - " + (result ? "成功" : "失败"));
}

// 测试8: 恢复默认灯光设置
console.log("\n测试8: 恢复默认灯光设置");
result = resetLighting();
if (result) {
    console.log("✓ 灯光设置已恢复默认");
} else {
    console.log("✗ 恢复默认灯光设置失败");
}

console.log("\n=== 灯光设置测试完成 ===");
console.log("✓ 所有灯光测试已执行");
console.log("提示: 可以在3D视图中观察不同灯光设置的效果");
