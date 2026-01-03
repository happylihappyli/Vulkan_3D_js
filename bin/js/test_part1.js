// 测试 setup_button.js 的前半部分
console.log("=== 开始测试 ===");

// 测试1: 创建简单按钮
console.log("\n测试1: 创建简单按钮");
let button1 = createButton("测试按钮1");
console.log("✓ 按钮创建成功: " + button1);

// 测试2: 创建多个按钮
console.log("\n测试2: 创建多个按钮");
let buttonNames = ["加载模型", "设置灯光", "设置材质", "重置场景"];
let buttons = [];

for (let i = 0; i < buttonNames.length; i++) {
    let btn = createButton(buttonNames[i]);
    buttons.push(btn);
    console.log("  创建按钮" + (i + 1) + ": " + buttonNames[i] + " - " + (btn ? "成功" : "失败"));
}

// 测试3: 设置按钮位置
console.log("\n测试3: 设置按钮位置");
let positions = [
    { x: 10, y: 10 },
    { x: 10, y: 50 },
    { x: 10, y: 90 },
    { x: 10, y: 130 }
];

for (let i = 0; i < Math.min(buttons.length, positions.length); i++) {
    let result = setButtonPosition(buttons[i], positions[i].x, positions[i].y);
    console.log("  按钮" + (i + 1) + "位置: (" + positions[i].x + ", " + positions[i].y + ") - " + (result ? "成功" : "失败"));
}

console.log("\n=== 测试完成 ===");
console.log("程序正在运行，请关闭窗口退出...");
