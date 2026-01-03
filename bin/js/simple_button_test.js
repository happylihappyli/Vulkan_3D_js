// 简化版按钮测试脚本
console.log("=== 开始测试 ===");

// 测试1: 创建按钮
console.log("测试1: 创建按钮");
let btn1 = createButton("测试按钮");
console.log("按钮ID: " + btn1);

// 测试2: 设置位置
console.log("测试2: 设置位置");
let result = setButtonPosition(btn1, 10, 10);
console.log("设置位置结果: " + result);

// 测试3: 设置大小
console.log("测试3: 设置大小");
result = setButtonSize(btn1, 100, 30);
console.log("设置大小结果: " + result);

// 测试4: 设置颜色
console.log("测试4: 设置颜色");
result = setButtonColor(btn1, 0.5, 0.5, 0.5);
console.log("设置颜色结果: " + result);

// 测试5: 设置可见性
console.log("测试5: 设置可见性");
result = setButtonVisible(btn1, true);
console.log("设置可见性结果: " + result);

// 测试6: 设置启用状态
console.log("测试6: 设置启用状态");
result = setButtonEnabled(btn1, true);
console.log("设置启用状态结果: " + result);

console.log("=== 测试完成 ===");
console.log("程序正在运行，请关闭窗口退出...");
