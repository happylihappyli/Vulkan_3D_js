// 测试 setup_button.js 的点击事件部分
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

// 测试8: 测试按钮点击事件
console.log("\n测试8: 测试按钮点击事件");
function onButtonClick(buttonId) {
    console.log("按钮被点击: " + buttonId);
}

for (let i = 0; i < buttons.length; i++) {
    let result = setButtonClickHandler(buttons[i], onButtonClick);
    console.log("  按钮" + (i + 1) + "点击事件: " + (result ? "已设置" : "设置失败"));
}

console.log("\n=== 测试完成 ===");
console.log("程序正在运行，请关闭窗口退出...");
