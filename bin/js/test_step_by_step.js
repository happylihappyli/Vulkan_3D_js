// 逐步测试 setup_button.js 的功能
console.log("=== 逐步测试按钮功能 ===");

// 测试1: 创建简单按钮
console.log("\n测试1: 创建简单按钮");
try {
    let button1 = createButton("测试按钮1");
    if (button1) {
        console.log("✓ 按钮创建成功: " + button1);
    } else {
        console.log("✗ 按钮创建失败");
    }
} catch (e) {
    console.error("✗ 测试1出错: " + e.message + ", 类型: " + e.name);
}

// 测试2: 创建多个按钮
console.log("\n测试2: 创建多个按钮");
let buttonNames = ["加载模型", "设置灯光", "设置材质", "重置场景"];
let buttons = [];

try {
    for (let i = 0; i < buttonNames.length; i++) {
        let btn = createButton(buttonNames[i]);
        buttons.push(btn);
        console.log("  创建按钮" + (i + 1) + ": " + buttonNames[i] + " - " + (btn ? "成功" : "失败"));
    }
    console.log("buttons 数组长度: " + buttons.length);
} catch (e) {
    console.error("✗ 测试2出错: " + e.message + ", 类型: " + e.name);
}

// 测试3: 设置按钮位置
console.log("\n测试3: 设置按钮位置");
let positions = [
    { x: 10, y: 10 },
    { x: 10, y: 50 },
    { x: 10, y: 90 },
    { x: 10, y: 130 }
];

console.log("buttons.length: " + buttons.length);
console.log("positions.length: " + positions.length);
console.log("Math.min(buttons.length, positions.length): " + Math.min(buttons.length, positions.length));

try {
    for (let i = 0; i < Math.min(buttons.length, positions.length); i++) {
        console.log("  处理按钮 " + i + ", ID: " + buttons[i]);
        let result = setButtonPosition(buttons[i], positions[i].x, positions[i].y);
        console.log("  按钮" + (i + 1) + "位置: (" + positions[i].x + ", " + positions[i].y + ") - " + (result ? "成功" : "失败"));
    }
} catch (e) {
    console.error("✗ 测试3出错: " + e.message + ", 类型: " + e.name);
}

console.log("\n=== 测试完成 ===");
