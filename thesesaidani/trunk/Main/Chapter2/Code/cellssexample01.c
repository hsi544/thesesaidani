#pragma css task inout(diag[B][B]) // pragma surrounding a cellss task
void lu0(float *diag){
  int i, j, k;
  for (k=0; k<BS; k++)
  for (i=k+1; i<BS; i++) {
    diag[i][k] = diag[i][k] / diag[k][k];
    for (j=k+1; j<BS; j++)
      diag[i][j] -= diag[i][k] * diag[k][j];
  }
}
#pragma css task input(diag[B][B]) inout(row[B][B])
void bdiv(float *diag, float *row){
  ...
}
#pragma css task input(row[B][B],col[B][B]) inout(inner[B][B])
void bmod(float *row, float *col, float *inner){
  ...
}
#pragma css task input(diag[B][B]) inout(col[B][B])
void fwd(float *diag, float *col){
  ...
}
void write_matrix (FILE * file, float *matrix[NB][NB]){
  int rows, columns;
  int i, j, ii, jj;
  fprintf (file, "%d\n %d\n", NB * B, NB * B);
  for (i = 0; i < NB; i++)
    for (ii = 0; ii < B; ii++){
      for (j = 0; j < NB; j++){
#pragma css wait on(matrix[i][j])
        for (jj = 0; jj < B; jj++)
          fprintf (file, "%f ", matrix[i][j][ii][jj]);
      }
      fprintf (file, "\n");
    }
}

