// 测试 setButtonPosition 函数
console.log("=== 测试 setButtonPosition 函数 ===");

// 创建一个按钮
console.log("\n创建按钮...");
let buttonId = createButton("测试按钮");
console.log("按钮ID: " + buttonId);

// 测试设置位置
console.log("\n测试设置位置...");
try {
    let result = setButtonPosition(buttonId, 100, 200);
    console.log("设置位置结果: " + (result ? "成功" : "失败"));
} catch (e) {
    console.error("设置位置时出错: " + e.message + ", 类型: " + e.name);
}

console.log("\n=== 测试完成 ===");
