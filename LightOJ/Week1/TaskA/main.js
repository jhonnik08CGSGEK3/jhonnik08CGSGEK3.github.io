function findColor(x, y) {
    if (x % 2 == y % 2)
        return 1;
    return 0;
}

function findBish(x1, y1, x2, y2) {
    if (findColor(x1, y1) == findColor(x2, y2)) {
        if (x1 - x2 == y1 - y2)
            return 1;
        if (x1 != x2 && y1 != y2)
            return 2;
        return 0;
    }
    return "impossible";
}
console.log("Case 1: ", findBish(1, 1, 10, 10));
console.log("Case 2: ", findBish(1, 1, 10, 11));
console.log("Case 3: ", findBish(1, 1, 5, 3));