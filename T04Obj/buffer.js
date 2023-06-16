class buffer {
    constructor(type, size) {
      this.id = window.gl.createBuffer();
      this.type = type;
      window.gl.bindBuffer(type, this.id);
      window.gl.bufferData(type, size, window.gl.STATIC_DRAW);
    }
  
    update(data) {
      window.gl.bindBuffer(this.type, this.id);
      window.gl.bufferSubData(this.type, 0, new Float32Array(data), 0);
    }
  
    apply() {
      window.gl.bindBuffer(this.type, this.id);
    }
  
    free() {
      window.gl.deleteBuffer(this.id);
      this.id = null;
      this.size = 0;
    }
  }
  
  class vertex_buffer extends buffer {
    constructor(vertices) {
      super(window.gl.ARRAY_BUFFER, (3 + 2 + 3 + 3) * 4 * vertices.length);
      this.numOfVertices = vertices.length;
      window.gl.bufferData(
        window.gl.ARRAY_BUFFER,
        new Float32Array(vertices),
        window.gl.STATIC_DRAW
      );
    }
  
    update(data) {
      window.gl.bindBuffer(window.gl.ARRAY_BUFFER, this.id);
      window.gl.bufferSubData(
        window.gl.ARRAY_BUFFER,
        0,
        new Float32Array(data),
        0
      );
    }
  
    free() {
      super.free();
      this.numOfVertices = 0;
    }
  }
  
  class index_buffer extends buffer {
    constructor(indices) {
      super(window.gl.ELEMENT_ARRAY_BUFFER, 2 * indices.length);
      this.numOfIndices = indices.length;
      window.gl.bufferData(
        window.gl.ELEMENT_ARRAY_BUFFER,
        new Uint32Array(indices),
        window.gl.STATIC_DRAW
      );
    }
  
    update(data) {
      window.gl.bindBuffer(window.gl.ELEMENT_ARRAY_BUFFER, this.id);
      window.gl.bufferSubData(
        window.gl.ELEMENT_ARRAY_BUFFER,
        0,
        new Uint32Array(data),
        0
      );
    }
  
    free() {
      super.free();
      this.numOfIndices = 0;
    }
  }
  
  class uniform_buffer extends buffer {
    constructor(name, size, bindingPoint) {
      super(window.gl.UNIFORM_BUFFER, size);
      window.gl.bufferData(window.gl.UNIFORM_BUFFER, size, window.gl.STATIC_DRAW);
      this.name = name;
      this.bind = bindingPoint;
    }
  
    update(data) {
      window.gl.bindBuffer(window.gl.UNIFORM_BUFFER, this.id);
      window.gl.bufferSubData(
        window.gl.UNIFORM_BUFFER,
        0,
        new Float32Array(data),
        0
      );
    }
  
    apply(prg) {
      let blk_loc = window.gl.getUniformBlockIndex(prg, this.name);
      if (blk_loc !== -1) {
        window.gl.uniformBlockBinding(prg, blk_loc, this.bind);
        window.gl.bindBufferBase(window.gl.UNIFORM_BUFFER, this.bind, this.id);
      }
    }
  
    free() {
      super.free();
    }
  }