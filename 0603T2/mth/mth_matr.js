class _mat4 {
    constructor(A00, A01, A02, A03,
                A10, A11, A12, A13,
                A20, A21, A22, A23,
                A30, A31, A32, A33) {
        this.A[0][0] = A00, this.A[0][1] = A01, this.A[0][2] = A02, this.A[0][3] = A03;
        this.A[1][0] = A10, this.A[1][1] = A11, this.A[1][2] = A12, this.A[1][3] = A13;
        this.A[2][0] = A20, this.A[2][1] = A21, this.A[2][2] = A22, this.A[2][3] = A23;
        this.A[3][0] = A30, this.A[3][1] = A31, this.A[3][2] = A32, this.A[3][3] = A33;
    }
}