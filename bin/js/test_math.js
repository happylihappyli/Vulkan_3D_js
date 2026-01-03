// 测试 Math 对象
console.log("=== 测试 Math 对象 ===");

console.log("\n测试 Math.min:");
try {
    let result = Math.min(4, 5);
    console.log("Math.min(4, 5) = " + result);
} catch (e) {
    console.error("Math.min 出错: " + e.message);
}

console.log("\n测试 Math.max:");
try {
    let result = Math.max(4, 5);
    console.log("Math.max(4, 5) = " + result);
} catch (e) {
    console.error("Math.max 出错: " + e.message);
}

console.log("\n测试 Math.PI:");
try {
    console.log("Math.PI = " + Math.PI);
} catch (e) {
    console.error("Math.PI 出错: " + e.message);
}

console.log("\n=== 测试完成 ===");
