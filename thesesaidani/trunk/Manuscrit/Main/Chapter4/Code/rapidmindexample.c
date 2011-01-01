
// Reference Code
float f;
float a[512][512][3];
float b[512][512][3];
float func (float r, float s)
{
  return (r + s) * f;
}
void func_arrays( )
{
  for (int x = 0; x<512; x++)
    for (int y = 0; y<512; y++) {
      for (int k = 0; k<3; k++) {
        a[y][x][k] = func(a[y][x][k],b[y][x][k]);
      }
    }
  }
}

// RapidMind Code
#include <rapidmind/platform.hpp>
Value1f f;
Array<2,Value3f> a(512,512);
Array<2,Value3f> b(512,512);
void func_arrays( )
{
  Program func_prog = BEGIN { // define program
    In<Value3f> r, s;
    Out<Value3f> q;
    q = (r + s) * f;
  } END;
  a = func_prog(a,b); // execute program
}
