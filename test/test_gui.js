// 测试脚本 - 简单的数学计算和日志输出
console.log("脚本开始执行...");

// 测试基本数学运算
let a = 10;
let b = 20;
let sum = a + b;
console.log("10 + 20 = " + sum);

// 测试函数定义
function calculate(x, y) {
    return x * y + 10;
}

let result = calculate(5, 3);
console.log("calculate(5, 3) = " + result);

// 测试数组操作
let numbers = [1, 2, 3, 4, 5];
let total = 0;
for (let i = 0; i < numbers.length; i++) {
    total += numbers[i];
}
console.log("数组总和: " + total);

// 测试对象操作
let obj = {
    name: "测试对象",
    value: 100
};
console.log("对象: " + obj.name + ", 值: " + obj.value);

console.log("脚本执行完成！");
