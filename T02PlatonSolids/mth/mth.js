import { mat4 } from "./mth_matr.js"
import { vec3 } from "./mth_vec.js"
import { camCreate } from "./mth_camera.js"

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

export { mat4 };
export { vec3 };
export { camCreate };