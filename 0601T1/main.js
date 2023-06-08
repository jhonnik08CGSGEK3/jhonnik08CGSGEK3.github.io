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