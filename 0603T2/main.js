/*
function loadShader(gl, type, source) {
    const shader = gl.createShader(type);

    gl.shaderSource(shader, source);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert("ERROR MAKING COMPILE_STATUS");
    }

    return shader;
}

function initGL() {
    fetch("./main.vertex.glsl").then((response) => {
        response.text().then((text) => console.log(text));
    });
    // debugger;
    const canvas = document.getElementById("glcanvas");
    const gl = canvas.getContext("webgl2");

    gl.clearColor(0.30, 0.47, 0.80, 1);
    gl.clear(gl.COLOR_BUFFER_BIT);

    const vs = `#version 300 es
        in vec4 in_pos;

        out highp vec4 color;

        void main() {
            gl_Position = in_pos;
            color = vec4(1, 0, 0, 1);
        }
    `;
    const fs = `#version 300 es
        out highp vec4 o_color;

        in highp vec4 color;

        void main() {
            o_color = color;
        }
    `;

    const vertexSh = loadShader(gl, gl.VERTEX_SHADER, vs);
    const fragmentSh = loadShader(gl, gl.FRAGMENT_SHADER, fs);
    //const shaderProgram = gl.createProgram();

    const program = gl.createProgram();
    gl.attachShader(program, vertexSh);
    gl.attachShader(program, fragmentSh);
    gl.linkProgram(program);

    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        alert("ERROR MAKING LINK STATUS");
    }
    
    const posLoc = gl.getAttribLocation(program, "in_pos");
    
    const posBuf = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, posBuf);
    const pos = [0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1];
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(pos), gl.STATIC_DRAW);
    gl.vertexAttribPointer(posLoc, 4, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(posLoc);
    gl.useProgram(program);

    gl.drawArrays(gl.TRIANGLES, 0, 3);
}

window.addEventListener("load", () => {
    initGL();
});
*/
'use strict';

const canvas = document.getElementById("glcanvas");
const gl = canvas.getContext("webgl2");

const vsGLSL = `#version 300 es
uniform View {
  mat4 viewInverse;
  mat4 viewProjection;
};

uniform Light {
  highp vec3 lightWorldPos;
  mediump vec4 lightColor;
};

uniform Model {
  mat4 world;
  mat4 worldInverseTranspose;
} model;

in vec4 a_position;
in vec3 a_normal;

out vec4 v_position;
out vec3 v_normal;
out vec3 v_surfaceToLight;
out vec3 v_surfaceToView;

void main() {
  v_position = (viewProjection * model.world * a_position);
  v_normal = (model.worldInverseTranspose * vec4(a_normal, 0)).xyz;
  v_surfaceToLight = lightWorldPos - (model.world * a_position).xyz;
  v_surfaceToView = (viewInverse[3] - (model.world * a_position)).xyz;
  gl_Position = v_position;
}
`;

const fsGLSL = `#version 300 es
precision highp float;

in vec4 v_position;
in vec3 v_normal;
in vec3 v_surfaceToLight;
in vec3 v_surfaceToView;

uniform Light {
  highp vec3 lightWorldPos;
  mediump vec4 lightColor;
};

uniform Material {
  vec4 diffuse;
  vec4 ambient;
  vec4 specular;
  float shininess;
  float specularFactor;
};

out vec4 theColor;

void main() {
  vec3 a_normal = normalize(v_normal);
  vec3 surfaceToView = normalize(v_surfaceToView);
  vec3 surfaceToLight = normalize(v_surfaceToLight);
  vec3 halfVector = normalize(surfaceToLight + surfaceToView);
  float light = dot(a_normal, surfaceToLight) * 0.5 + 0.5;
  float specularLight = pow(max(0.0, dot(a_normal, halfVector)), shininess);
  theColor = vec4((
      diffuse * light +
      diffuse * ambient +
      specular * specularLight * specularFactor).rgb,
      diffuse.a);
}
`;

const createShader = function(gl, type, glsl) {
  const shader = gl.createShader(type)
  gl.shaderSource(shader, glsl)
  gl.compileShader(shader)
  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    throw new Error(gl.getShaderInfoLog(shader))
  }
  return shader
};

const createProgram = function(gl, vsGLSL, fsGLSL) {
  const vs = createShader(gl, gl.VERTEX_SHADER, vsGLSL)
  const fs = createShader(gl, gl.FRAGMENT_SHADER, fsGLSL)
  const prg = gl.createProgram()
  gl.attachShader(prg, vs)
  gl.attachShader(prg, fs)
  gl.linkProgram(prg)
  if (!gl.getProgramParameter(prg, gl.LINK_STATUS)) {
    throw new Error(gl.getProgramInfoLog(prg))
  }

  // NOTE! These are only here to unclutter the diagram.
  // It is safe to detach and delete shaders once
  // a program is linked though it is arguably not common.
  // and I usually don't do it.
  gl.detachShader(prg, vs)
  gl.deleteShader(vs)
  gl.detachShader(prg, fs)
  gl.deleteShader(fs)

  return prg
};

const prg = createProgram(gl, vsGLSL, fsGLSL);

const positionLoc = gl.getAttribLocation(prg, 'a_position');
const normalLoc = gl.getAttribLocation(prg, 'a_normal');

// Look up the uniform blocks locations on the program and assign
// each one an index into the uniform buffer bind points
const viewUniformBufferIndex = 0;
const materialUniformBufferIndex = 1;
const modelUniformBufferIndex = 2;
const lightUniformBufferIndex = 3;
gl.uniformBlockBinding(
    prg, 
    gl.getUniformBlockIndex(prg, 'View'),
    viewUniformBufferIndex);
gl.uniformBlockBinding(
    prg, 
    gl.getUniformBlockIndex(prg, 'Model'),
    modelUniformBufferIndex);
gl.uniformBlockBinding(
    prg, 
    gl.getUniformBlockIndex(prg, 'Material'),
    materialUniformBufferIndex);
gl.uniformBlockBinding(
    prg, 
    gl.getUniformBlockIndex(prg, 'Light'),
    lightUniformBufferIndex);

// vertex positions for a cube
const cubeVertexPositions = new Float32Array([
    1, 1, -1, 1, 1, 1, 1, -1, 1, 1, -1, -1, -1, 1, 1, -1, 1, -1, -1, -1, -1, -1, -1, 1, -1, 1, 1, 1, 1, 1, 1, 1, -1, -1, 1, -1, -1, -1, -1, 1, -1, -1, 1, -1, 1, -1, -1, 1, 1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, 1, 1, -1, 1, -1, -1, -1, -1, -1,
]);
// vertex normals for a cube
const cubeVertexNormals = new Float32Array([
    1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1,
]);
// vertex indices for the triangles of a cube
// the data above defines 24 vertices. We need to draw 12
// triangles, 2 for each size, each triangle needs
// 3 vertices so 12 * 3 = 36
const cubeVertexIndices = new Uint16Array([
    0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23,
],);

const cubeVertexArray = gl.createVertexArray();
gl.bindVertexArray(cubeVertexArray);

const positionBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
gl.bufferData(gl.ARRAY_BUFFER, cubeVertexPositions, gl.STATIC_DRAW);
gl.enableVertexAttribArray(positionLoc);
gl.vertexAttribPointer(
    positionLoc,  // location
    3,            // size (components per iteration)
    gl.FLOAT,     // type of to get from buffer
    false,        // normalize
    0,            // stride (bytes to advance each iteration)
    0,            // offset (bytes from start of buffer)
);

const normalBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, normalBuffer);
gl.bufferData(gl.ARRAY_BUFFER, cubeVertexNormals, gl.STATIC_DRAW);
gl.enableVertexAttribArray(normalLoc);
gl.vertexAttribPointer(
    normalLoc,  // location
    3,          // size (components per iteration)
    gl.FLOAT,   // type of to get from buffer
    false,      // normalize
    0,          // stride (bytes to advance each iteration)
    0,          // offset (bytes from start of buffer)
);

const indexBuffer = gl.createBuffer();
gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);
gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, cubeVertexIndices, gl.STATIC_DRAW);


// This is not really needed but if we end up binding anything
// to ELEMENT_ARRAY_BUFFER, say we are generating indexed geometry
// we'll change cubeVertexArray's ELEMENT_ARRAY_BUFFER. By binding
// null here that won't happen.
gl.bindVertexArray(null);

// create a buffer for the view uniform block
// and an ArrayBuffer and 2 views into that buffer
// one for each uniform
const viewUniformBlockBuffer = gl.createBuffer();
const viewArrayBuffer = new ArrayBuffer(16 * 4 * 2);
const viewArrayData = new Float32Array(viewArrayBuffer);
const viewInverse = new Float32Array(viewArrayBuffer, 0, 16);
const viewProjection = new Float32Array(viewArrayBuffer, 16 * 4, 16);

// create a buffer for the model uniform block
// and an ArrayBuffer and 2 views into that buffer
// one for each uniform
const modelUniformBlockBuffer = gl.createBuffer();
const modelArrayBuffer = new ArrayBuffer(16 * 4 * 2);
const modelArrayData = new Float32Array(modelArrayBuffer);
const world = new Float32Array(modelArrayBuffer, 0, 16);
const worldInverseTranspose = new Float32Array(modelArrayBuffer, 16 * 4, 16);

// create a buffer for both the Light uniform block
const lightUniformBlockBuffer = gl.createBuffer();
const lightUniformBlockData = new Float32Array([
  0, 10, 30, 0,    // vec3, lightWorldPos, padded to 16 bytes
  1, 1, 1, 1,     // vec4, lightColor
]);
gl.bindBuffer(gl.UNIFORM_BUFFER, lightUniformBlockBuffer);
gl.bufferData(gl.UNIFORM_BUFFER, lightUniformBlockData, gl.STATIC_DRAW);

// create 3 buffers for 3 different material settings
const blueMatUniformBlockBuffer = gl.createBuffer();
gl.bindBuffer(gl.UNIFORM_BUFFER, blueMatUniformBlockBuffer);
gl.bufferData(gl.UNIFORM_BUFFER, new Float32Array([
    0.5, 0.7, 1, 1,  // vec4, diffuse
    0, 0, 0, 0,      // vec4, ambient
    1, 1, 1, 1,      // vec4, specular
    100,             // float, shininess
    1,               // float, specularFactor
    0, 0,            // padding
]), gl.STATIC_DRAW);

const redMatUniformBlockBuffer = gl.createBuffer();
gl.bindBuffer(gl.UNIFORM_BUFFER, redMatUniformBlockBuffer);
gl.bufferData(gl.UNIFORM_BUFFER, new Float32Array([
    1, 0.3, 0.5, 1,  // vec4, diffuse
    0, 0, 0, 0,      // vec4, ambient
    1, 1, 1, 1,      // vec4, specular
    50,              // float, shininess
    1,               // float, specularFactor
    0, 0,            // padding
]), gl.STATIC_DRAW);

const greenMatUniformBlockBuffer = gl.createBuffer();
gl.bindBuffer(gl.UNIFORM_BUFFER, greenMatUniformBlockBuffer);
gl.bufferData(gl.UNIFORM_BUFFER, new Float32Array([
    0.2, 1, 0.5, 1,  // vec4, diffuse
    0, 0, 0, 0,      // vec4, ambient
    1, 1, 1, 1,      // vec4, specular
    3,               // float, shininess
    1,               // float, specularFactor
    0, 0,            // padding
]), gl.STATIC_DRAW);

// above this line is initialization code
// --------------------------------------
// below is rendering code.

gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);
gl.clear(gl.COLOR_BUFFER_BIT);
gl.enable(gl.DEPTH_TEST);
gl.enable(gl.CULL_FACE);

gl.useProgram(prg);

// compute the projection matrix into the TypedArray we declared above
m4.perspective(
  60 * Math.PI / 180,  // fov
  gl.canvas.clientWidth / gl.canvas.clientHeight,  // aspect
  0.1,  // near
  10,   // far
  viewProjection,
);
// compute the inverse view matrix (lazy so setting identity)
m4.identity(viewInverse);

// upload the view uniform data to the buffer.
gl.bindBuffer(gl.UNIFORM_BUFFER, viewUniformBlockBuffer);
gl.bufferData(gl.UNIFORM_BUFFER, viewArrayData, gl.DYNAMIC_DRAW);

// bind the view buffer to the uniform buffer bind point
// the program will look at
gl.bindBufferBase(
    gl.UNIFORM_BUFFER,
    viewUniformBufferIndex,
    viewUniformBlockBuffer);

// bind the model buffer to the uniform buffer bind point
// the program will look at
gl.bindBufferBase(
    gl.UNIFORM_BUFFER,
    modelUniformBufferIndex,
    modelUniformBlockBuffer);

// bind the light buffer to the uniform buffer bind point
// the program will look at
gl.bindBufferBase(
    gl.UNIFORM_BUFFER,
    lightUniformBufferIndex,
    lightUniformBlockBuffer);

gl.bindVertexArray(cubeVertexArray);

// compute the world matrices
m4.identity(world);
m4.translate(world, -2, 0, -4, world);
m4.xRotate(world, 0.5, world);
m4.yRotate(world, 0.5, world);
m4.inverse(world, worldInverseTranspose);
m4.transpose(worldInverseTranspose, worldInverseTranspose);

// upload the model uniform data to the buffer.
gl.bindBuffer(gl.UNIFORM_BUFFER, modelUniformBlockBuffer);
gl.bufferData(gl.UNIFORM_BUFFER, modelArrayData, gl.DYNAMIC_DRAW);

// bind the blue material buffer to the uniform buffer bind point
// the program will look at
gl.bindBufferBase(
    gl.UNIFORM_BUFFER,
    materialUniformBufferIndex,
    blueMatUniformBlockBuffer);

gl.drawElements(
    gl.TRIANGLES,
    36,                // num vertices to process
    gl.UNSIGNED_SHORT, // type of indices
    0,                 // offset on bytes to indices
);

// compute the world matrices
m4.identity(world);
m4.translate(world, 0, 0, -4, world);
m4.xRotate(world, 0.9, world);
m4.yRotate(world, 0.9, world);
m4.inverse(world, worldInverseTranspose);
m4.transpose(worldInverseTranspose, worldInverseTranspose);

// upload the model uniform data to the buffer.
gl.bindBuffer(gl.UNIFORM_BUFFER, modelUniformBlockBuffer);
gl.bufferData(gl.UNIFORM_BUFFER, modelArrayData, gl.DYNAMIC_DRAW);

// bind the red material buffer to the uniform buffer bind point
// the program will look at
gl.bindBufferBase(
    gl.UNIFORM_BUFFER,
    materialUniformBufferIndex,
    redMatUniformBlockBuffer);

gl.drawElements(
    gl.TRIANGLES,
    36,                // num vertices to process
    gl.UNSIGNED_SHORT, // type of indices
    0,                 // offset on bytes to indices
);

// compute the world matrices
m4.identity(world);
m4.translate(world, 3, 0, -4, world);
m4.xRotate(world, 0.7, world);
m4.yRotate(world, -0.6, world);
m4.inverse(world, worldInverseTranspose);
m4.transpose(worldInverseTranspose, worldInverseTranspose);

// upload the model uniform data to the buffer.
gl.bindBuffer(gl.UNIFORM_BUFFER, modelUniformBlockBuffer);
gl.bufferData(gl.UNIFORM_BUFFER, modelArrayData, gl.DYNAMIC_DRAW);

// bind the green material buffer to the uniform buffer bind point
// the program will look at
gl.bindBufferBase(
    gl.UNIFORM_BUFFER,
    materialUniformBufferIndex,
    greenMatUniformBlockBuffer);

gl.drawElements(
    gl.TRIANGLES,
    36,                // num vertices to process
    gl.UNSIGNED_SHORT, // type of indices
    0,                 // offset on bytes to indices
);