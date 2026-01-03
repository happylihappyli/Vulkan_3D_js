// 按钮设置测试脚本
// 测试创建和设置GUI按钮

console.log("=== 按钮设置测试开始 ===");

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

try {
    for (let i = 0; i < Math.min(buttons.length, positions.length); i++) {
        let result = setButtonPosition(buttons[i], positions[i].x, positions[i].y);
        console.log("  按钮" + (i + 1) + "位置: (" + positions[i].x + ", " + positions[i].y + ") - " + (result ? "成功" : "失败"));
    }
} catch (e) {
    console.error("✗ 测试3出错: " + e.message + ", 类型: " + e.name);
}

// 测试4: 设置按钮大小
console.log("\n测试4: 设置按钮大小");
let sizes = [
    { width: 100, height: 30 },
    { width: 120, height: 35 },
    { width: 100, height: 30 },
    { width: 100, height: 30 }
];

try {
    for (let i = 0; i < Math.min(buttons.length, sizes.length); i++) {
        let result = setButtonSize(buttons[i], sizes[i].width, sizes[i].height);
        console.log("  按钮" + (i + 1) + "大小: " + sizes[i].width + "x" + sizes[i].height + " - " + (result ? "成功" : "失败"));
    }
} catch (e) {
    console.error("✗ 测试4出错: " + e.message + ", 类型: " + e.name);
}

// 测试5: 设置按钮颜色
console.log("\n测试5: 设置按钮颜色");
let colors = [
    { r: 0.2, g: 0.6, b: 0.8 }, // 蓝色
    { r: 0.8, g: 0.6, b: 0.2 }, // 橙色
    { r: 0.2, g: 0.8, b: 0.4 }, // 绿色
    { r: 0.8, g: 0.2, b: 0.4 }  // 红色
];

try {
    for (let i = 0; i < Math.min(buttons.length, colors.length); i++) {
        let result = setButtonColor(buttons[i], colors[i].r, colors[i].g, colors[i].b);
        console.log("  按钮" + (i + 1) + "颜色: (" + colors[i].r + ", " + colors[i].g + ", " + colors[i].b + ") - " + (result ? "成功" : "失败"));
    }
} catch (e) {
    console.error("✗ 测试5出错: " + e.message + ", 类型: " + e.name);
}

// 测试6: 设置按钮可见性
console.log("\n测试6: 设置按钮可见性");
try {
    for (let i = 0; i < buttons.length; i++) {
        let result = setButtonVisible(buttons[i], true);
        console.log("  按钮" + (i + 1) + "可见性: true - " + (result ? "成功" : "失败"));
    }
} catch (e) {
    console.error("✗ 测试6出错: " + e.message + ", 类型: " + e.name);
}

// 测试7: 设置按钮启用状态
console.log("\n测试7: 设置按钮启用状态");
try {
    for (let i = 0; i < buttons.length; i++) {
        let result = setButtonEnabled(buttons[i], true);
        console.log("  按钮" + (i + 1) + "启用状态: true - " + (result ? "成功" : "失败"));
    }
} catch (e) {
    console.error("✗ 测试7出错: " + e.message + ", 类型: " + e.name);
}

// 测试8: 测试按钮点击事件
console.log("\n测试8: 测试按钮点击事件");
try {
    function onButtonClick(buttonId) {
        console.log("按钮被点击: " + buttonId);
    }
    
    for (let i = 0; i < buttons.length; i++) {
        let result = setButtonClickHandler(buttons[i], onButtonClick);
        console.log("  按钮" + (i + 1) + "点击事件: " + (result ? "已设置" : "设置失败"));
    }
} catch (e) {
    console.error("✗ 测试8出错: " + e.message + ", 类型: " + e.name);
}

// 测试9: 创建带图标的按钮
console.log("\n测试9: 创建带图标的按钮");
try {
    let iconButton = createButton("📁 打开文件");
    if (iconButton) {
        console.log("✓ 图标按钮创建成功: " + iconButton);
    } else {
        console.log("✗ 图标按钮创建失败");
    }
} catch (e) {
    console.error("✗ 测试9出错: " + e.message + ", 类型: " + e.name);
}

// 测试10: 创建工具栏按钮
console.log("\n测试10: 创建工具栏按钮");
try {
    let toolbarButtons = ["⬆️", "⬇️", "⬅️", "➡️", "🔄", "⏸️", "▶️"];
    for (let i = 0; i < toolbarButtons.length; i++) {
        let btn = createButton(toolbarButtons[i]);
        console.log("  工具栏按钮" + (i + 1) + ": " + toolbarButtons[i] + " - " + (btn ? "成功" : "失败"));
    }
} catch (e) {
    console.error("✗ 测试10出错: " + e.message + ", 类型: " + e.name);
}

// 测试11: 删除按钮
console.log("\n测试11: 删除按钮");
try {
    if (buttons.length > 0) {
        let result = deleteButton(buttons[0]);
        console.log("  删除第一个按钮: " + (result ? "成功" : "失败"));
    }
} catch (e) {
    console.error("✗ 测试11出错: " + e.message + ", 类型: " + e.name);
}

// 注释掉清除所有按钮的代码，让按钮保持显示
// 测试12: 清除所有按钮
// console.log("\n测试12: 清除所有按钮");
// let result = clearAllButtons();
// if (result) {
//     console.log("✓ 所有按钮已清除");
// } else {
//     console.log("✗ 清除按钮失败");
// }

console.log("\n=== 按钮设置测试完成 ===");
console.log("✓ 所有按钮测试已执行");
console.log("提示: 可以在GUI窗口中查看按钮效果");
console.log("提示: 程序将保持运行，请关闭窗口退出");

// 保持程序运行，让用户能看到按钮界面
console.log("\n程序正在运行中，请关闭窗口退出...");
