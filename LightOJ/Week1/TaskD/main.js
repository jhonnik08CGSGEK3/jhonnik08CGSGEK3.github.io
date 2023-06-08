function findScarecrow(n, field) {
    let res = 0;
    for (let i = 0; i < n; i++) {
        if (field[i] == '.') {
            if (i + 2 < n) {
                if (field[i + 1] == '.' || field[i + 2] == '.')
                {
                    res++;
                    i += 2;
                    continue;
                }
            }
            else if (i + 1 < n) {
                if (field[i + 1] == '.')
                {
                    res++;
                    i++;
                    continue;
                }
            }
            res++;
        }
    }
    return res;
}

console.log("Case 1: ", findScarecrow(3, ".#."));
console.log("Case 2: ", findScarecrow(11, "...##....##"));
console.log("Case 3: ", findScarecrow(11, "...#...#..."));