

void task matmul :: inner (in float A[M][P],
                           in float B[P][N],
                           inout float C[M][N]) 
{ 
  // Tunable parameters specify the size 
  // of subblocks of A, B, and C. 
  tunable int U; 
  tunable int X; 
  tunable int V; 
  // Partition matrices into sets of blocks 
  // using regular 2D chopping . 
  blkset Ablks = rchop(A, U, X);
  blkset Bblks = rchop(B, X, V); 
  blkset Cblks = rchop(C, U, V);
    // Compute all blocks of C in parallel.
    mappar(int i=0 to M/U, int j=0 to N/V){ 
      mapreduce (int k=0 to P/X){
        // Invoke the matmul task recursively
        // on the subblocks of A, B, and C.
        matmul(Ablks[i][k], Bblks[k][j], Cblks[i][j]);
      } 
    } 
} 
void task matmul :: leaf(in float A[M][P],
                         in float B[P][N],
                         inout float C[M][N])
{ 
  // Compute matrix product directly
  for (int i=0;i<M;i++)
    for (int j=0;j<N;j++)
      for (int k=0;k<P;k++)
        C[i][j] += A[i][k]*B[k][j];
}