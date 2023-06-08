function findHorse(m, n) {
    if (m == 1 || n == 1 || (m == 2 && n == 2))
        return m * n;
    return Math.trunc(m * n / 2) + (m * n) % 2;
}

console.log("Case 1: ", findHorse(8, 8));
console.log("Case 2: ", findHorse(3, 7));
console.log("Case 3: ", findHorse(4, 10));
console.log("Case 4: ", findHorse(2, 2));