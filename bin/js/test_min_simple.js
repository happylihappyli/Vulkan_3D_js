// 测试 Math.min
console.log("开始测试 Math.min");

let a = 4;
let b = 5;

console.log("a = " + a);
console.log("b = " + b);

try {
    let result = Math.min(a, b);
    console.log("Math.min(" + a + ", " + b + ") = " + result);
} catch (e) {
    console.log("Math.min 出错: " + e.message);
}

console.log("测试完成");
