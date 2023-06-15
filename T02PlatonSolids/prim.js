import { vec3, mat4, camCreate } from "./mth/mth.js";
import { gl } from "./render.js";

const
    vertex = "xyzrgba",
    sizeInBytes = vertex.length * 4,
    sizeInNumbers = vertex.length;

function countNormals(v, ind) {
  for (let i = 0; i < v.length; i++)
    v[i].n = vec3(0);

  if (ind !== null)
    for (let i = 0; i + 2 < ind.length; i += 3) {
      let n = v[ind[i + 1]].p
        .sub(v[ind[i]].p)
        .cross(v[ind[i + 2]].p.sub(v[ind[i]].p))
        .norm();
      v[ind[i]].n = v[ind[i]].n.add(n).norm();
      v[ind[i + 1]].n = v[ind[i + 1]].n.add(n).norm();
      v[ind[i + 2]].n = v[ind[i + 2]].n.add(n).norm();
    }
  else
    for (let i = 0; i + 2 < v.length; i += 3) {
      let n = v[i + 1].p
        .sub(v[i].p)
        .cross(v[i + 2].p.sub(v[i].p))
        .norm();
      v[i].n = v[i].n.add(n).norm();
      v[i + 1].n = v[i + 1].n.add(n).norm();
      v[i + 2].n = v[i + 2].n.add(n).norm();
    }
}

class _vert {
  constructor(p, t, n, c) {
    if (p === undefined) {
      this.p = vec3(0);
      this.t = vec2(0);
      this.n = vec3(0);
      this.c = vec3(0);
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
      this.vBuf = this.vArray = this.iBuf = 0;  
      this.gl = gl;
      this.type = type;
      this.vData = vData;
      this.iData = iData;
      this.shader = shaderProgram;

      this.vCnt = vCnt;
      this.iCnt = iCnt;

      this.trans = mat4();

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

    draw(world, cam) {
        let w = this.trans.mulMatr(world);
        let rotcube = 15;
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
}

class dsPrim {
  constructor(type, v, ind) {
    this.vBuf = this.vA = this.iBuf = 0;
    if (v !== null) {
      let primVertexArray = window.gl.createVertexArray();
      window.gl.bindVertexArray(primVertexArray);

      const pos = vertArr2floatArr(v);

      this.vBuf = new vertex_buffer(pos);
      this.vA = primVertexArray;
    }
    if (ind !== null) {
      let primIndexBuffer = window.gl.createBuffer();
      window.gl.bindBuffer(window.gl.ELEMENT_ARRAY_BUFFER, primIndexBuffer);
      window.gl.bufferData(
        window.gl.ELEMENT_ARRAY_BUFFER,
        ind.length * 2,
        window.gl.STATIC_DRAW
      );
      window.gl.bufferData(
        window.gl.ELEMENT_ARRAY_BUFFER,
        new Uint32Array(ind),
        window.gl.STATIC_DRAW
      );

      this.iBuf = new index_buffer(ind);
      this.numOfElements = ind.length;
    } else this.numOfElements = v.length;

    this.trans = matrIdentity();
    this.type = type;
    this.mtlNo = 0;
    [this.minBB, this.maxBB] = countBB(v);
  }

  // Primitive free function
  free = () => {
    this.vBuf.free();
    this.iBuf.free();
  };

  // Primitive draw function
  draw = (world) => {
    let w = this.trans.mulMatr(world),
      wInv = w.inverse().transpose(),
      wvp = w.mulMatr(ds_cam.matrVP);

    let prg = dsRnd.mtl.apply(this.mtlNo);
    if (prg === 0) return;

    let arr = [];
    mat2floatArr(arr, w);
    mat2floatArr(arr, wInv);
    mat2floatArr(arr, wvp);

    dsRnd.matrixUBO.update(arr);
    dsRnd.matrixUBO.apply(prg);

    ds_cam.ubo.apply(prg);

    let loc;

    if ((loc = window.gl.getUniformLocation(prg, "Time")) !== -1) {
      window.gl.uniform1f(loc, myTimer.localTime);
    }

    if ((loc = window.gl.getAttribLocation(prg, "InPos")) !== -1) {
      window.gl.vertexAttribPointer(
        loc,
        3,
        window.gl.FLOAT,
        false,
        (3 + 2 + 3 + 3) * 4,
        0
      );
      window.gl.enableVertexAttribArray(loc);
    }

    if ((loc = window.gl.getAttribLocation(prg, "InNormal")) !== -1) {
      window.gl.vertexAttribPointer(
        loc,
        3,
        window.gl.FLOAT,
        false,
        (3 + 2 + 3 + 3) * 4,
        (3 + 2) * 4
      );
      window.gl.enableVertexAttribArray(loc);
    }

    window.gl.bindVertexArray(this.vA);
    this.vBuf.apply();
    if (this.iBuf === 0) {
      window.gl.drawArrays(this.type, 0, this.numOfElements);
    } else {
      this.iBuf.apply();
      window.gl.drawElements(
        this.type,
        this.numOfElements,
        window.gl.UNSIGNED_INT,
        0
      );
    }
  };
}

export function prim(...args) {
  return new _prim(...args);
}