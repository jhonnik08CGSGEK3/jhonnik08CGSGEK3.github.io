// Pi math constant
const PI = 3.14159265358979323846;

// Degrees to radians conversion
function D2R(A) {
    return A * (PI / 180.0);
}
function Degree2Radian(a) {
    return D2R(a);
}

// Radians to degrees conversion
function R2D(A) {
    return A * (180.0 / PI);
}
function Radian2Degree(a) {
    return R2D(a);
}

class _vec2 {
    constructor(x, y) {
        if (x == undefined && y == undefined)
            this.x = 0, this.y = 0;
        else if (y == undefined)
            this.x = x, this.y = x;
        else
            this.x = x, this.y = y;
    }
}

export function vec2(...args) {
    return new _vec2(...args);
}

class _vec4 {
    constructor(x, y, z, w) {
        if (x == undefined && y == undefined && z == undefined && w == undefined)
            this.x = 0, this.y = 0, this.z = 0, this.w = 0;
        else if (y == undefined && z == undefined && w == undefined)
            this.x = x, this.y = x, this.z = x, this.w = x;
        else
            this.x = x, this.y = y, this.z = z, this.w = w;
    }
}

export function vec4(...args) {
    return new _vec4(...args);
}

class _vec3 {
    constructor(x, y, z) {
        if (x == undefined && y == undefined && z == undefined)
            this.x = 0, this.y = 0, this.z = 0;
        else if (y == undefined && z == undefined)
            this.x = x, this.y = x, this.z = x;
        else
            this.x = x, this.y = y, this.z = z;
    }

    add(v) {
        return new _vec3(this.x + v.x, this.y + v.y, this.z + v.z);
    } // End of 'add' function

    sub(v) {
        return new _vec3(this.x - v.x, this.y - v.y, this.z - v.z);
    } // End of 'sub' function

    mulNum(num) {
        return new _vec3(this.x * num, this.y * num, this.z * num);
    } // End of 'mulNum' function

    divNum(num) {
        if (num != 0)
            return new _vec3(this.x / num, this.y / num, this.z / num);
        return this;
    } // End of 'divNum' function

    neg() {
        return new _vec3(-this.x, -this.y, -this.z);
    } // End of 'neg' function

    dot(v) {
        return this.x * v.x + this.y * v.y + this.z * v.z;
    } // End of 'dot' function

    cross(v) {
        return new _vec3(this.y * v.z - this.z * v.y,
                         this.z * v.x - this.x * v.z,
                         this.x * v.y - this.y * v.x);
    } // End of 'cross' function

    len2() {
        return this.dot(this);
    } // End of 'len2' function

    len() {
        let len = this.len2();
        if (len == 1 || len == 0)
            return len;
        return Math.sqrt(len);
    } // End of 'len' function

    normalize() {
        let len = this.len2();
        if (len == 1 || len == 0)
            return this;
        return this.divNum(Math.sqrt(len));
    } // End of 'normalize' function

    pointTransform(M) {
        return new _vec3((this.x * M.A[0][0] + this.y * M.A[1][0] + this.z * M.A[2][0] + M.A[3][0]),
                         (this.x * M.A[0][1] + this.y * M.A[1][1] + this.z * M.A[2][1] + M.A[3][1]),
                         (this.x * M.A[0][2] + this.y * M.A[1][2] + this.z * M.A[2][2] + M.A[3][2]));
    } // End of 'pointTransform' function

    vecTransform(M) {
        return new _vec3((this.x * M.A[0][0] + this.y * M.A[1][0] + this.z * M.A[2][0]),
                         (this.x * M.A[0][1] + this.y * M.A[1][1] + this.z * M.A[2][1]),
                         (this.x * M.A[0][2] + this.y * M.A[1][2] + this.z * M.A[2][2]));
    } // End of 'vecTransform' function

    mulMatr(M) {
        let w = this.x * M.A[0][3] + this.y * M.A[1][3] + this.z * M.A[2][3] + M.A[3][3];
        return new _vec3((this.x * M.A[0][0] + this.y * M.A[1][0] + this.z * M.A[2][0] + M.A[3][0]) / w,
                         (this.x * M.A[0][1] + this.y * M.A[1][1] + this.z * M.A[2][1] + M.A[3][1]) / w,
                         (this.x * M.A[0][2] + this.y * M.A[1][2] + this.z * M.A[2][2] + M.A[3][2]) / w);
    } // End of 'mulMatr' function
}
export function vec3(...args) {
    return new _vec3(...args);
}