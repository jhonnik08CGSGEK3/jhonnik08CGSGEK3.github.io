import { mat4 } from "./mth_matr.js";
import { vec3 } from "./mth_vec.js"

class _camera {
    constructor() {
        // Projection properties
        this.projSize = 0.1; // Project plane fit square
        this.projDist = 0.1; // Distance to project plane from viewer (near)
        this.projFarClip = 1800; // Distance to project far clip plane (far)
    
        // Local size data
        this.frameW = 1000; // Frame width
        this.frameH = 1000; // Frame height
    
        // Matrices
        this.matrView = mat4(); // View coordinate system matrix
        this.matrProj = mat4(); // Projection coordinate system matrix
        this.matrVP = mat4(); // View and projection matrix precalculate value
    
        // Set camera default settings
        this.loc = vec3(); // Camera location
        this.at = vec3(); // Camera destination
        this.dir = vec3(); // Camera Direction
        this.up = vec3(); // Camera UP direction
        this.right = vec3(); // Camera RIGHT direction
      }   
      
      projSet() {
            let rx, ry;

            rx = ry = this.projSize;
        
            if (this.frameW > this.frameH)
                rx *= this.frameW / this.frameH;
            else
                ry *= this.frameH / this.frameW;
        
            this.matrProj =
                this.matrProj.frustrum(-rx / 2, rx / 2, -ry / 2, ry / 2,
                    this.projDist, this.projFarClip);
            this.matrVP = this.matrView.mulMatr(this.matrProj);
      }

      camSet(Loc, At, Up) {
            this.matrView = this.matrView.view(Loc, At, Up);
            this.matrVP = this.matrView.mulMatr(this.matrProj);
            this.at = At;
            this.loc = Loc;
        
            this.dir = vec3(-this.matrView.A[0][2],
                                -this.matrView.A[1][2],
                                -this.matrView.A[2][2]);
            this.up = vec3(this.matrView.A[0][1],
                               this.matrView.A[1][1],
                               this.matrView.A[2][1]);
            this.right = vec3(this.matrView.A[0][0],
                                  this.matrView.A[1][0],
                                  this.matrView.A[2][0]);
      }

      resize(w, h) {
        /* Setup projection */
        this.frameW = w;
        this.frameH = h;
        this.projSet();     
      }
}

export function camCreate() {
    return new _camera();
}