class _vec3 {
    constructor(x, y, z) {
        if (x == undefined || y == undefined || z == undefined)
            this.x = 0, this.y = 0, this.z = 0;
        else if (y == undefined && z == undefined)
            this.x = x, this.y = x, this.z = x;
    }
    set(x, y, z) {
        this.x = x;
        this.y = y;
        this.z = z;
        return this;
    }
    add(v) {
        this.x += v.x;
        this.y += v.y;
        this.z += v.z;
        return this;
    }
    sub(v) {
        this.x -= v.x;
        this.y -= v.y;
        this.z -= v.z;
        return this;
    }
    mul(num) {
        this.x *= num;
        this.y *= num;
        this.z *= num;
        return this;
    }
    div(num) {
        if (num != 0) {
            this.x /= num;
            this.y /= num;
            this.z /= num;
        }
        return this;
    }
    neg() {
        this.x = -this.x;
        this.y = -this.y;
        this.z = -this.z;
    }
    dot(v) {
        return this.x * v.x + this.y * v.y + this.z * v.z;
    }
    cross(v) {
        return this.set(this.y * v.z - this.z * v.y,
                        this.z * v.x - this.x * v.z,
                        this.x * v.y - this.y * v.x);
    }
    len2() {
        return this.dot(this);
    }
    len() {
        len = this.len2();
        if (len == 1 || len == 0)
            return len;
        return Math.SQRT2(len);
    }
    normalize() {
        len = this.len2();
        if (len == 1 || len == 0)
            return this;
        return this.div(Math.SQRT2(len));
    }
}