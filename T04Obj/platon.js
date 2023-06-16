import { gl } from "./render.js";
import { prim } from "./prim.js";
import { vec3, mat4 } from "./mth/mth.js";
  
function makeVecs(v, ind) {
    let res = [];
    for (let i = 0; i < ind.length; i++) {
      res[i] = vec3(v[ind[i]].x, v[ind[i]].y, v[ind[i]].z);
    }
    return res;
}
  
export function createTetrahedron(matr, shader) {
    let sq2 = Math.sqrt(2),
        sq6 = Math.sqrt(6);
    let v = [
        // position                  // color
        0.0, 1.0, 0.0,               1.0, 1.0, 0.5, 1.0,
        -sq2 / 3, -1 / 3, -sq6 / 3,  1.0, 0.0, 1.0, 1.0,
        -sq2 / 3, -1 / 3, sq6 / 3,   0.0, 0.0, 1.0, 1.0,
        (2 * sq2) / 3, -1 / 3, 0,    0.0, 1.0, 1.0, 1.0,
       ];

    let i = [0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 2, 3];

    // let v = makeVecs(vecs, i);

    // countNormals(v, null);

    //let pr = prim(gl.TRIANGLES, v, null);
    let pr = prim(gl.TRIANGLES, v, 4, i, 12, shader);
    pr.trans = matr;
    return pr;
}

export function createHexahedron(matr, shader) {
    let l = 1 / Math.sqrt(3);
  
    let v = [
        -l, l, -l, 1.0, 1.0, 0.5, 1.0,
        l, l, -l, 1.0, 0.0, 1.0, 1.0,
        l, -l, -l, 0.0, 0.0, 1.0, 1.0,
        -l, -l, -l, 0.0, 1.0, 1.0, 1.0,
        -l, l, l, 1.0, 1.0, 0.5, 1.0, 
        l, l, l, 1.0, 0.0, 1.0, 1.0,
        l, -l, l, 0.0, 0.0, 1.0, 1.0,
        -l, -l, l, 0.0, 1.0, 1.0, 1.0,
    ];

    let i = [
      0, 1, 2,
      0, 2, 3,
      2, 1, 5,
      2, 5, 6,
      3, 2, 6,
      3, 6, 7,
      0, 3, 7,
      0, 7, 4,
      1, 0, 4,
      1, 4, 5,
      6, 5, 4,
      6, 4, 7,
    ];
  
    let pr = prim(gl.TRIANGLES, v, 8, i, 36, shader);
    pr.trans = matr;
    return pr;
}

export function createOctahedron(matr, shader) {
    let vecs = [
      vec3(-1, 0, 0),
      vec3(0, 0, 1),
      vec3(1, 0, 0),
      vec3(0, 0, -1),
      vec3(0, 1, 0),
      vec3(0, -1, 0),
    ];
  
    let i = [
      0, 1, 4,
      1, 2, 4,
      2, 3, 4,
      3, 0, 4,
      0, 1, 5, 
      1, 2, 5, 
      2, 3, 5,
      3, 0, 5,
    ];
  
    let v = [
        -1.0, 0.0, 0.0, 1.0, 1.0, 0.5, 1.0,
        0.0, 0.0, 1.0,  1.0, 0.0, 1.0, 1.0,
        1.0, 0.0, 0.0,  0.0, 0.0, 1.0, 1.0,
        0.0, 0.0, -1.0, 0.0, 1.0, 1.0, 1.0,
        0.0, 1.0, 0.0,  1.0, 1.0, 0.5, 1.0,
        0.0, -1.0, 0.0, 1.0, 0.0, 1.0, 1.0,
    ];

    let pr = prim(gl.TRIANGLES, v, 6, i, 24, shader);
    pr.trans = matr;
    return pr;
}

export function createDodecahedron(matr, shader) {
    let v = [
        0, -1, 0,
        0, 1, 0,
        -2 / Math.sqrt(5), -1 / Math.sqrt(5), 0,
        2 / Math.sqrt(5), 1 / Math.sqrt(5), 0,      
        0.5 + 0.5 / Math.sqrt(5), -1 / Math.sqrt(5), -Math.sqrt(0.1 * (5 - Math.sqrt(5))), 1.0, 1.0, 0.5, 1.0,
        0.5 + 0.5 / Math.sqrt(5), -1 / Math.sqrt(5), Math.sqrt(0.1 * (5 - Math.sqrt(5))),  1.0, 0.0, 1.0, 1.0,
        -0.1 * (5 + Math.sqrt(5)), 1 / Math.sqrt(5), -Math.sqrt(0.1 * (5 - Math.sqrt(5))), 0.0, 0.0, 1.0, 1.0,
        -0.1 * (5 + Math.sqrt(5)), 1 / Math.sqrt(5), Math.sqrt(0.1 * (5 - Math.sqrt(5))),  0.0, 1.0, 1.0, 1.0,
        0.1 * Math.sqrt(5) - 0.5, -1 / Math.sqrt(5), -Math.sqrt(0.1 * (5 + Math.sqrt(5))), 1.0, 1.0, 0.5, 1.0,
        0.1 * Math.sqrt(5) - 0.5, -1 / Math.sqrt(5), Math.sqrt(0.1 * (5 + Math.sqrt(5))),  1.0, 0.0, 1.0, 1.0,
        0.5 - 0.1 * Math.sqrt(5), 1 / Math.sqrt(5), -Math.sqrt(0.1 * (5 + Math.sqrt(5))),  0.0, 0.0, 1.0, 1.0,
        0.5 - 0.1 * Math.sqrt(5), 1 / Math.sqrt(5), Math.sqrt(0.1 * (5 + Math.sqrt(5))),   0.0, 1.0, 1.0, 1.0,
    ];
  
    /*let i = [
      0, 8, 2,
      0, 2, 9, 
      0, 9, 5, 
      0, 5, 4, 
      0, 4, 8,
      1, 3, 10, 
      1, 10, 6, 
      1, 6, 7, 
      1, 7, 11, 
      1, 11, 3,  
      3, 4, 5, 
      10, 4, 8, 
      6, 8, 2, 
      7, 2, 9, 
      11, 9, 5,
      4, 10, 3, 
      8, 10, 6, 
      2, 6, 7, 
      9, 7, 11, 
      5, 11, 3,
    ];*/
  
    let i = [
      0, 1, 2,
      0, 2, 3,
      0, 3, 4,
      1, 2, 13, 
      2, 13, 14, 
      13, 14, 18, 
      0, 1, 12, 
      1, 12, 13, 
      12, 13, 17, 
      4, 0, 11, 
      0, 11, 12, 
      11, 12, 16, 
      3, 4, 10, 
      4, 10, 11, 
      10, 11, 15, 
      2, 3, 14, 
      3, 14, 10,
      14, 10, 19, 
      19, 14, 18, 
      18, 19, 9, 
      8, 18, 9,
      18, 13, 17, 
      17, 18, 8, 
      7, 17, 8,
      17, 12, 16, 
      16, 17, 7, 
      6, 16, 7,
      16, 11, 15, 
      15, 16, 6,
      5, 15, 6,
      15, 10, 19, 
      19, 15, 5, 
      9, 19, 5,
      5, 6, 7, 
      5, 7, 8, 
      5, 8, 9,
    ];
  
    let pr = prim(gl.TRIANGLES, v, 12, i, 108, shader);
    pr.trans = matr;
    return pr;
  }
  
/* function createIcosahedron(matr) {
    let vecs = [
      vec3(0, -1, 0),
      vec3(0, 1, 0),
      vec3(-2 / Math.sqrt(5), -1 / Math.sqrt(5), 0),
      vec3(2 / Math.sqrt(5), 1 / Math.sqrt(5), 0),
      vec3(
        0.5 + 0.5 / Math.sqrt(5),
        -1 / Math.sqrt(5),
        -Math.sqrt(0.1 * (5 - Math.sqrt(5)))
      ),
      vec3(
        0.5 + 0.5 / Math.sqrt(5),
        -1 / Math.sqrt(5),
        Math.sqrt(0.1 * (5 - Math.sqrt(5)))
      ),
      vec3(
        -0.1 * (5 + Math.sqrt(5)),
        1 / Math.sqrt(5),
        -Math.sqrt(0.1 * (5 - Math.sqrt(5)))
      ),
      vec3(
        -0.1 * (5 + Math.sqrt(5)),
        1 / Math.sqrt(5),
        Math.sqrt(0.1 * (5 - Math.sqrt(5)))
      ),
      vec3(
        0.1 * Math.sqrt(5) - 0.5,
        -1 / Math.sqrt(5),
        -Math.sqrt(0.1 * (5 + Math.sqrt(5)))
      ),
      vec3(
        0.1 * Math.sqrt(5) - 0.5,
        -1 / Math.sqrt(5),
        Math.sqrt(0.1 * (5 + Math.sqrt(5)))
      ),
      vec3(
        0.5 - 0.1 * Math.sqrt(5),
        1 / Math.sqrt(5),
        -Math.sqrt(0.1 * (5 + Math.sqrt(5)))
      ),
      vec3(
        0.5 - 0.1 * Math.sqrt(5),
        1 / Math.sqrt(5),
        Math.sqrt(0.1 * (5 + Math.sqrt(5)))
      ),
    ];
  
    // 0, 8, 2,
    // 0, 2, 9,
    // 0, 9, 5,
    // 0, 5, 4,
    // 0, 4, 8,
  
    // 1, 3, 10,
    // 1, 10, 6,
    // 1, 6, 7,
    // 1, 7, 11,
    // 1, 11, 3,
  
    // 3, 4, 5,
    // 10, 4, 8,
    // 6, 8, 2,
    // 7, 2, 9,
    // 11, 9, 5,
  
    // 4, 10, 3,
    // 8, 10, 6,
    // 2, 6, 7,
    // 9, 7, 11,
    // 5, 11, 3,
    let i = [
      0, 8, 2, 0, 2, 9, 0, 9, 5, 0, 5, 4, 0, 4, 8,
  
      1, 3, 10, 1, 10, 6, 1, 6, 7, 1, 7, 11, 1, 11, 3,
  
      3, 4, 5, 10, 4, 8, 6, 8, 2, 7, 2, 9, 11, 9, 5,
  
      4, 10, 3, 8, 10, 6, 2, 6, 7, 9, 7, 11, 5, 11, 3,
    ];
  
    let v = makeV(vecs, i);
  
    countNormals(v, null);
  
    let pr = new dsPrim(window.gl.TRIANGLES, v, null);
    pr.trans = matr;
    return pr;
}
*/