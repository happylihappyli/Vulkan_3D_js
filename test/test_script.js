// QuickJS JavaScript 测试脚本
// 测试基本的 JavaScript 功能

console.log("=== QuickJS JavaScript 测试开始 ===");

// 测试基本算术运算
var x = 10;
var y = 20;
var result = x + y;
console.log("基本算术运算: " + x + " + " + y + " = " + result);

// 测试字符串操作
var str1 = "Hello";
var str2 = "World";
var combined = str1 + " " + str2;
console.log("字符串拼接: " + combined);

// 测试数组操作
var arr = [1, 2, 3, 4, 5];
var sum = 0;
for (var i = 0; i < arr.length; i++) {
    sum += arr[i];
}
console.log("数组求和: " + sum);

// 测试函数定义
function add(a, b) {
    return a + b;
}
var funcResult = add(100, 200);
console.log("函数调用: add(100, 200) = " + funcResult);

// 测试对象操作
var obj = {
    name: "测试对象",
    value: 42,
    method: function() {
        return "对象方法被调用";
    }
};
console.log("对象属性: " + obj.name + ", " + obj.value);
console.log("对象方法: " + obj.method());

// 测试条件语句
var testValue = 15;
if (testValue > 10) {
    console.log("条件测试: " + testValue + " > 10 (真)");
} else {
    console.log("条件测试: " + testValue + " > 10 (假)");
}

// 测试循环
console.log("循环测试:");
for (var j = 1; j <= 3; j++) {
    console.log("  迭代 " + j);
}

console.log("=== QuickJS JavaScript 测试完成 ===");
