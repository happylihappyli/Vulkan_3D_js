// 测试 setup_button.js 的中间部分
console.log("=== 开始测试 ===");

// 测试2: 创建多个按钮
console.log("\n测试2: 创建多个按钮");
let buttonNames = ["加载模型", "设置灯光", "设置材质", "重置场景"];
let buttons = [];

for (let i = 0; i < buttonNames.length; i++) {
    let btn = createButton(buttonNames[i]);
    buttons.push(btn);
    console.log("  创建按钮" + (i + 1) + ": " + buttonNames[i] + " - " + (btn ? "成功" : "失败"));
}

// 测试4: 设置按钮大小
console.log("\n测试4: 设置按钮大小");
let sizes = [
    { width: 100, height: 30 },
    { width: 120, height: 35 },
    { width: 100, height: 30 },
    { width: 100, height: 30 }
];

for (let i = 0; i < Math.min(buttons.length, sizes.length); i++) {
    let result = setButtonSize(buttons[i], sizes[i].width, sizes[i].height);
    console.log("  按钮" + (i + 1) + "大小: " + sizes[i].width + "x" + sizes[i].height + " - " + (result ? "成功" : "失败"));
}

// 测试5: 设置按钮颜色
console.log("\n测试5: 设置按钮颜色");
let colors = [
    { r: 0.2, g: 0.6, b: 0.8 },
    { r: 0.8, g: 0.6, b: 0.2 },
    { r: 0.2, g: 0.8, b: 0.4 },
    { r: 0.8, g: 0.2, b: 0.4 }
];

for (let i = 0; i < Math.min(buttons.length, colors.length); i++) {
    let result = setButtonColor(buttons[i], colors[i].r, colors[i].g, colors[i].b);
    console.log("  按钮" + (i + 1) + "颜色: (" + colors[i].r + ", " + colors[i].g + ", " + colors[i].b + ") - " + (result ? "成功" : "失败"));
}

// 测试6: 设置按钮可见性
console.log("\n测试6: 设置按钮可见性");
for (let i = 0; i < buttons.length; i++) {
    let result = setButtonVisible(buttons[i], true);
    console.log("  按钮" + (i + 1) + "可见性: true - " + (result ? "成功" : "失败"));
}

// 测试7: 设置按钮启用状态
console.log("\n测试7: 设置按钮启用状态");
for (let i = 0; i < buttons.length; i++) {
    let result = setButtonEnabled(buttons[i], true);
    console.log("  按钮" + (i + 1) + "启用状态: true - " + (result ? "成功" : "失败"));
}

console.log("\n=== 测试完成 ===");
console.log("程序正在运行，请关闭窗口退出...");
