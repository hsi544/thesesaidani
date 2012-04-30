float *A[NB][NB];
#pragma css task inout(diag[B][B])
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
void write_matrix (FILE * file, float *matrix[NB][NB])
{
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
int main(int argc, char **argv) {
  int ii, jj, kk;
  FILE *fileC;
  …
  initialize (A);
#pragma css start
  for (kk=0; kk<NB; kk++) {
    lu0(A[kk][kk]);
    for (jj=kk+1; jj<NB; jj++)
      if (A[kk][jj] != NULL)
        fwd(A[kk][kk], A[kk][jj]);
    for (ii=kk+1; ii<NB; ii++)
      if (A[ii][kk] != NULL) {
        bdiv (A[kk][kk], A[ii][kk]);
        for (jj=kk+1; jj<NB; jj++)
          if (A[kk][jj] != NULL) {
            if (A[ii][jj]==NULL)
              A[ii][jj]=allocate_clean_block();
              bmod(A[ii][kk], A[kk][jj], A[ii][jj]);
          }
      }
  }
  fileC = fopen (argv[3], "w");
  write_matrix (fileC, A);
  fclose (fileC);
  #pragma css finish
}
