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
