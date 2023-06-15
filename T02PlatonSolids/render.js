import { vec3, mat4, camCreate } from "./mth/mth.js";
import { prim } from "./prim.js";
import { createTetrahedron, createHexahedron, createOctahedron, createDodecahedron } from "./platon.js";

// Global context data
let gl;
export { gl };

// Timer data
/*
let startTime,
  oldTime,
  oldTimeFPS,
  pauseTime,
  timePerSec,
  frameCounter,
  FPS,
  globalTime,
  globalDeltaTime,
  localTime,
  localDeltaTime;
*/
// import { mat4, vec3 } from "./mth/mth.js"

/*** LOAD SHADER (FETCH) ***/
function loadShader(gl, type, source) {
    const shader = gl.createShader(type);

    gl.shaderSource(shader, source);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert("ERROR LOADING SHADER")
    }

    return shader;
}

export function initGL() {
    const canvas = document.getElementById("glCanvas");
    gl = canvas.getContext("webgl2");
    let vs, fs;
    
    gl.clearColor(0.30, 0.47, 0.80, 1);
    gl.clear(gl.COLOR_BUFFER_BIT);

    const ft1 = fetch("./shaders/default/vert.glsl")
        .then((res) => res.text())
        .then((data) => {
        vs = data;
        });
    const ft2 = fetch("./shaders/default/frag.glsl")
        .then((res) => res.text())
        .then((data) => {
        fs = data;
        });
    const allData = Promise.all([ft1, ft2]);
    allData.then((res) => {
        const vertexSh = loadShader(gl, gl.VERTEX_SHADER, vs);
        const fragmentSh = loadShader(gl, gl.FRAGMENT_SHADER, fs);

        const shaderProgram = gl.createProgram();
        gl.attachShader(shaderProgram, vertexSh);
        gl.attachShader(shaderProgram, fragmentSh);
        gl.linkProgram(shaderProgram);

        const vBuf = gl.createBuffer();

        let cam = camCreate();
        let m = mat4();
        cam.resize(500, 500);
        cam.camSet(vec3(20, 20, 30), vec3(0, 0, 0), vec3(0, 1, 0));
        
        let prTetra = createTetrahedron(mat4(), shaderProgram);
        let prHex = createHexahedron(mat4(), shaderProgram);
        let prOcta = createOctahedron(mat4(), shaderProgram);
        //let prDodeca = createDodecahedron(mat4(), shaderProgram);
        let rot = 0;

        const render = () => {
            gl.clear(gl.COLOR_BUFFER_BIT);
            gl.enable(gl.DEPTH_TEST);
            
            prTetra.draw((m.scale(vec3(7, 7, 7)).mulMatr(m.rotateY(rot)).mulMatr(m.translate(vec3(0, 8, 0)))), cam);
            prHex.draw((m.scale(vec3(5)).mulMatr(m.translate(vec3(10, -3, 0))).mulMatr(m.rotateY(rot))), cam);
            prOcta.draw((m.scale(vec3(7, 7, 7)).mulMatr(m.rotateX(rot)).mulMatr(m.translate(vec3(-3, -3, 0)))), cam);
            //prDodeca.draw((m.scale(vec3(7, 7, 7)).mulMatr(m.rotateY(rot))), cam);
            rot++;
            //gl.bindBuffer(gl.ARRAY_BUFFER, vBuf);

            //gl.vertexAttribPointer(0, 4, gl.FLOAT, false, 7 * 4, 0);     // position
            //gl.vertexAttribPointer(1, 4, gl.FLOAT, false, 7 * 4, 4 * 3); // color

            //gl.enableVertexAttribArray(0);
            //gl.enableVertexAttribArray(1);

            

            // gl.drawArrays(gl.TRIANGLES, 0, dataBuf.length / 8);
            
            window.requestAnimationFrame(render);
        }

        render();
    });
}

/*** VG4 SAMPLE ***/
/*
function loadShader(type, source) {
    const shader = gl.createShader(type);

    gl.shaderSource(shader, source);
    gl.compileShader(shader);

    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        alert("ERROR LOADING SHADER");
    }

    return shader;
}

function initGL() {
    canvas = document.getElementById("glcanvas");
    gl = canvas.getContext("webgl2");
    gl.clearColor(0.3, 0.47, 0.8, 1);
    //gl.clear(gl.COLOR_BUFFER_BIT);

    // Shader initialization
    const ft1 = fetch("shaders/default/vert.glsl")
        .then((res) => res.text())
        .then((data) => {
        vs = data;
        });
    const ft2 = fetch("shaders/default/frag.glsl")
        .then((res) => res.text())
        .then((data) => {
        fs = data;
        });
    const allData = Promise.all([ft1, ft2]);
    allData.then((res) => {
        // Shaders
        const vertexShader = loadShader(gl.VERTEX_SHADER, vs);
        const fragmentShader = loadShader(gl.FRAGMENT_SHADER, fs);
        shaderProgram = gl.createProgram();
        gl.attachShader(shaderProgram, vertexShader);
        gl.attachShader(shaderProgram, fragmentShader);
        gl.linkProgram(shaderProgram);
        if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
        const Buf = gl.getProgramInfoLog(shaderProgram);
        console.log(Buf);
        alert("ERROR LINK_STATUS");
        }

        // Vertex array/buffer
        const posLoc = gl.getAttribLocation(shaderProgram, "in_pos");

        frameVertexArray = gl.createVertexArray();
        gl.bindVertexArray(frameVertexArray);

        posBuffer = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, posBuffer);
        const x = 1;
        const pos = [-x, x, 0, -x, -x, 0, x, x, 0, x, -x, 0];
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(pos), gl.STATIC_DRAW);
        gl.vertexAttribPointer(posLoc, 3, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(0);
        gl.useProgram(shaderProgram);
        time_loc = gl.getUniformLocation(shaderProgram, "Time");

        frameBuffer = gl.createBuffer();
        gl.bindBuffer(gl.UNIFORM_BUFFER, frameBuffer);
        gl.bufferData(gl.UNIFORM_BUFFER, 12 * 4, gl.STATIC_DRAW);

        /*
        gl.uniformBlockBinding(
        shaderProgram,
        gl.getUniformBlockIndex(shaderProgram, "frameBuffer"),
        frameUniformBufferIndex,
        );
        

        //myTimerInit();
    });
}
*/