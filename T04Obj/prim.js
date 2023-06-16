import { vec2, vec3, vec4, mat4, camCreate } from "./mth/mth.js";
import { gl } from "./render.js";

const
    vertex = "xyzrgba",
    sizeInBytes = vertex.length * 4,
    sizeInNumbers = vertex.length;

async function fetchOBJ(objectURL) {
  try {
    const response = await fetch(objectURL);
    const text = await response.text();

    return text;
  } catch(err) {
    console.error(err);
  }
}

function countNormals(v, ind) {
  for (let i = 0; i < v.length; i++)
    v[i].n = vec3(0);

  if (ind !== null)
    for (let i = 0; i + 2 < ind.length; i += 3) {
      let n = v[ind[i + 1]].p
        .sub(v[ind[i]].p)
        .cross(v[ind[i + 2]].p.sub(v[ind[i]].p))
        .normalize();
      v[ind[i]].n = v[ind[i]].n.add(n).normalize();
      v[ind[i + 1]].n = v[ind[i + 1]].n.add(n).normalize();
      v[ind[i + 2]].n = v[ind[i + 2]].n.add(n).normalize();
    }
  else
    for (let i = 0; i + 2 < v.length; i += 3) {
      let n = v[i + 1].p
        .sub(v[i].p)
        .cross(v[i + 2].p.sub(v[i].p))
        .normalize();
      v[i].n = v[i].n.add(n).normalize();
      v[i + 1].n = v[i + 1].n.add(n).normalize();
      v[i + 2].n = v[i + 2].n.add(n).normalize();
    }
}

class _vert {
  constructor(p, t, n, c) {
    if (p === undefined) {
      this.p = vec3(0);
      this.t = vec2(0);
      this.n = vec3(0);
      this.c = vec4(0);
    } else if (typeof p === "object" && t === undefined) {
      this.p = p.p;
      this.t = p.t;
      this.n = p.n;
      this.c = p.c;
    } else {
      this.p = p;
      this.t = t;
      this.n = n;
      this.c = c;
    }
  }
}

function vert(...args) {
  return new _vert(...args);
}

class _prim {
    constructor(type, vData, vCnt, iData, iCnt, shaderProgram) {
      this.gl = gl;
      this.vBuf = this.vArray = this.iBuf = 0;
      this.vData = this.iData = this.shader = 0;
      this.vCnt = this.iCnt = 0;
      this.trans = mat4();
      if (type != undefined && vData != undefined) {
        this.type = type;
        this.vData = vData;
        this.iData = iData;
        this.shader = shaderProgram;

        this.vCnt = vCnt;
        this.iCnt = iCnt;

        if (vData.length != 0 && vCnt != 0) {
            // Create vertex buffer object
            this.vBuf = gl.createBuffer();
            this.vArray = gl.createVertexArray();
            gl.bindVertexArray(this.vArray);
        
            gl.bindBuffer(gl.ARRAY_BUFFER, this.vBuf);
            gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(this.vData), gl.STATIC_DRAW);

            gl.vertexAttribPointer(0, 3, gl.FLOAT, false, sizeInBytes, 0);  // position
            gl.vertexAttribPointer(1, 4, gl.FLOAT, false, sizeInBytes, 12); // color

            gl.enableVertexAttribArray(0);
            gl.enableVertexAttribArray(1);

            gl.bindVertexArray(null);
        }

        if (iData.length != 0 && iCnt != 0) {
            // Create index buffer object
            this.iBuf = gl.createBuffer();
            gl.bindBuffer(this.gl.ELEMENT_ARRAY_BUFFER, this.iBuf);
            gl.bufferData(this.gl.ELEMENT_ARRAY_BUFFER, new Uint32Array(this.iData), this.gl.STATIC_DRAW);
            gl.bindBuffer(this.gl.ELEMENT_ARRAY_BUFFER, null);
        }
      }  
    }

    draw(world, cam) {
        let w = this.trans.mulMatr(world);
        let winv = (w.inverse()).transpose();
        let wvp = w.mulMatr(cam.matrVP);
    
        //let wvp = w.mul(TE1_RndMatrVP);
        const locWVP = gl.getUniformLocation(this.shader, "matrWVP");
        if (locWVP != null)
            gl.uniformMatrix4fv(locWVP, false, new Float32Array(wvp.toArray()));
        gl.bindVertexArray(this.vArray);
        gl.useProgram(this.shader);

        if (this.iCnt == 0) {
            gl.drawArrays(this.type, 0, this.vData.length / sizeInNumbers);
        }
        else {
            gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.iBuf);
            gl.drawElements(this.type, this.iCnt, gl.UNSIGNED_INT, 0);
            gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, null);
        }
        gl.bindVertexArray(null);
    }

    load(textObj, shader) {
      let nv = 0, nf = 0, i = 0;
      let Ind = [];
      let V = [];
      let Buf;
      let obj = textObj.split("\n");

      while((Buf = obj[i]) != null) {
        if (Buf[0] == 'v' && Buf[1] == ' ')
          nv++;
        else if (Buf[0] == 'f' && Buf[1] == ' ')
          nf++;
        i++;
      }

      i = 0;
      while((Buf = obj[i]) != null) {
        let xyz = Buf.split(' ');
        if (Buf[0] == 'v' && Buf[1] == ' ') {
          V.push(+xyz[1]);
          V.push(+xyz[2]);
          V.push(+xyz[3]);
          V.push(+1.0);
          V.push(+1.0);
          V.push(+1.0);
          V.push(+1.0);
        }
        else if (Buf[0] == 'f' && Buf[1] == ' ') {
          let c0 = 0, c1 = 0, c;

          for (let cnt = 1; cnt <= 3; cnt++) {
            c = parseInt(xyz[cnt]) - 1;
            if (cnt == 1)
              c0 = c;
            else if (cnt == 2)
              c1 = c;
            else {
              Ind.push(c0);
              Ind.push(c1);
              Ind.push(c);
              c1 = c;
            }
          }
        }
        i++;
      }
      return new _prim(gl.TRIANGLES, new Float32Array(V), nv, new Uint16Array(Ind), nf, shader);
    }
}

export function prim(...args) {
  return new _prim(...args);
}