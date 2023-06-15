import { vec3 } from "./mth_vec.js"

function D2R(A) {
    return A * (PI / 180.0);
}

function R2D(A) {
    return A * (180.0 / PI);
}

const PI = 3.14159265358979323846;

class _mat4 {
    constructor(A00, A01, A02, A03,
                A10, A11, A12, A13,
                A20, A21, A22, A23,
                A30, A31, A32, A33) {
        if (A01 != undefined) {
            this.A = [
                [A00, A01, A02, A03],
                [A10, A11, A12, A13],
                [A20, A21, A22, A23],
                [A30, A31, A32, A33],
            ];
        }
        else if (A00 != undefined && A01 == undefined){
            this.A = [
                [A00, A00, A00, A00],
                [A00, A00, A00, A00],
                [A00, A00, A00, A00],
                [A00, A00, A00, A00],
            ];
        }
        else {
            this.A = [
                [1, 0, 0, 0],
                [0, 1, 0, 0],
                [0, 0, 1, 0],
                [0, 0, 0, 1],
            ];
        }
    } // End of 'constructor' function

    identity() {
        return new _mat4(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
    } // End of 'identity' function

    translate(T) {
        return new _mat4(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         T.x, T.y, T.z, 1);
    } // End of 'translate' function

    scale(S) {
        return new _mat4(S.x, 0, 0, 0,
                         0, S.y, 0, 0,
                         0, 0, S.z, 0,
                         0, 0, 0, 1);
    } // End of 'scale' function

    rotateX(angleInDegree) {
        let a = D2R(angleInDegree), co = Math.cos(a), si = Math.sin(a);

        return new _mat4(1, 0, 0, 0,
                         0, co, si, 0,
                         0, -si, co, 0,
                         0,  0, 0, 1);
    } // End of 'rotateX' function

    rotateY(angleInDegree) {
        let a = D2R(angleInDegree), co = Math.cos(a), si = Math.sin(a);

        return new _mat4(co, 0, -si, 0,
                         0, 1, 0, 0,
                         si, 0, co, 0,
                         0,  0, 0, 1);
    } // End of 'rotateY' function

    rotateZ(angleInDegree) {
        let a = D2R(angleInDegree), co = Math.cos(a), si = Math.sin(a);

        return new _mat4(co, si, 0, 0,
                         -si, co, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
    } // End of 'rotateZ' function

    rotateVec(angleInDegree, v1) {
        a = D2R(angleInDegree),
        si = Math.sin(a), co = Math.cos(a);
        let v = v1.normalize();
        return new _mat4(co + v.x * v.x * (1 - co), v.x * v.y * (1 - co) + v.z * si, v.x * v.z * (1 - co) - v.y * si, 0,
                         v.y * v.x * (1 - co) - v.z * si, co + v.y * v.y * (1 - co), v.y * v.z * (1 - co) + v.x * si, 0,
                         v.z * v.x * (1 - co) + v.y * si, v.z * v.y * (1 - co) - v.x * si, co + v.z * v.z * (1 - co), 0,
                         0, 0, 0, 1);
    } // End of 'rotateVec' function

    mulMatr(M) {
        let i, j, k;
        let r = new _mat4(0);
        
        for (i = 0; i < 4; i++)
            for (j = 0; j < 4; j++)
                for (k = 0; k < 4; k++)
                    r.A[i][j] += this.A[i][k] * M.A[k][j];
        
        return r;
    }

    transpose() {
        let tmp;
        for (let i = 0; i < 4; i++)
            for (let j = 0; j < 4; j++)
                if (i != j) {
                    tmp = this.A[i][j];
                    this.A[i][j] = this.A[j][i];
                    this.A[j][i] = tmp;
                }
        return this;
    } // End of 'transpose' function

    determ3x3(A11, A12, A13,
              A21, A22, A23,
              A31, A32, A33) {
        return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
        A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
    } // End of 'determ3x3' function

    determ4x4() {
        return  +this.A[0][0] * this.determ3x3(this.A[1][1], this.A[1][2], this.A[1][3],
                                               this.A[2][1], this.A[2][2], this.A[2][3],
                                               this.A[3][1], this.A[3][2], this.A[3][3]) +
                -this.A[0][1] * this.determ3x3(this.A[1][0], this.A[1][2], this.A[1][3],
                                               this.A[2][0], this.A[2][2], this.A[2][3],
                                               this.A[3][0], this.A[3][2], this.A[3][3]) +
                +this.A[0][2] * this.determ3x3(this.A[1][0], this.A[1][1], this.A[1][3],
                                               this.A[2][0], this.A[2][1], this.A[2][3],
                                               this.A[3][0], this.A[3][1], this.A[3][3]) +
                -this.A[0][3] * this.determ3x3(this.A[1][0], this.A[1][1], this.A[1][2],
                                               this.A[2][0], this.A[2][1], this.A[2][2],
                                               this.A[3][0], this.A[3][1], this.A[3][2]);
    } // End of 'determ4x4' function

    inverse() {
        let r = new _mat4(0);
        let det = this.determ4x4();
        let s = [1, -1], P = [[1, 2, 3], [0, 2, 3], [0, 1, 3], [0, 1, 2]];

        if (det == 0)
            return this.identity();

        for (let i = 0; i < 4; i++)
            for (let j = 0; j < 4; j++)
                r.A[j][i] = s[(i + j) % 2] * this.determ3x3(this.A[P[i][0]][P[j][0]], this.A[P[i][0]][P[j][1]], this.A[P[i][0]][P[j][2]],
                                                            this.A[P[i][1]][P[j][0]], this.A[P[i][1]][P[j][1]], this.A[P[i][1]][P[j][2]],
                                                            this.A[P[i][2]][P[j][0]], this.A[P[i][2]][P[j][1]], this.A[P[i][2]][P[j][2]]) / det;
        return r;
    }

    view(loc, at, up1) {
        let dir = at.sub(loc).normalize(),  // VecNormalize(VecSubVec(At, Loc)),
            r = dir.cross(up1).normalize(), // VecNormalize(VecCrossVec(Dir, Up1)),
            up = r.cross(dir);              // VecCrossVec(r, Dir);
        let View = new _mat4(r.x, up.x, -dir.x, 0,
                             r.y, up.y, -dir.y, 0,
                             r.z, up.z, -dir.z, 0,
                             -loc.dot(r), -loc.dot(up), loc.dot(dir), 1);
        return View;
    }

    ortho(l, r, b, t, n, f) {
        return new _mat4(2 / (r - l), 0, 0, 0,
                         0, 2 / (t - b), 0, 0,
                         0, 0, -2 / (f - n), 0,
                         -(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1);
    }

    frustrum(l, r, b, t, n, f) {
        return new _mat4(2 * n / (r - l), 0, 0, 0,
                         0, 2 * n / (t - b), 0, 0,
                         (r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1,
                         0, 0, -2 * n * f / (f - n), 0);
    }

    toArray() {
        let r = [];

        for (let i = 0; i < 4; i++) {
            for (let j = 0; j < 4; j++) {
                r.push(this.A[i][j]);
            }
        }
        return r;
    }
}

export function mat4(...args) {
    return new _mat4(...args);
}