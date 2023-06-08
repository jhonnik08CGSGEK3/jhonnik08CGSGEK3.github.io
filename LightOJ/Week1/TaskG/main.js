var readline = require('readline');

function find10(a) {
    let i = 1n;
    while (a / i > 10n)
        i = i * 10n;
    return i;
}

function findNum(a) {
    let n = 0n, div = 1n;
    for (let i = 1n; i < a; i++) {
        n = n * find10(i);
        n = n + i;
    }
    return n;
}

function divBy3(a, b) {
    let n = 0, num = findNum(a);
    for (let i = a; i <= b; i++) {
        if (num % 3n == 0)
           n++;
        num = num * find10(i);
        num = num + i;
    }
    return n;
}

console.log(divBy3(3, 5));
