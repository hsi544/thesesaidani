void f_args_fill( f_args& a, int v0, int v1[5], float& v2 )
{
  a.arg0 = v0;
  a.arg1.first = &v1[0];
  a.arg2.first = &v2;
};
