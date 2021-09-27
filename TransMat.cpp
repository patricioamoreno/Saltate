#include<math.h>

void TransMat(float M[][3],float IM[][3], float a,float b,float c){
M[0][0]=cos(a)*cos(b);
M[0][1]=-sin(a);
M[0][2]=cos(a)*sin(b);
M[1][0]=sin(a)*cos(b);
M[1][1]=cos(a);
M[1][2]=sin(a)*sin(b);
M[2][0]=-sin(b);
M[2][1]=0;
M[2][2]=cos(b);

IM[0][0]=cos(a)*cos(b);
IM[0][1]=sin(a)*cos(b);
IM[0][2]=-sin(b);
IM[1][0]=-sin(a);
IM[1][1]=cos(a);
IM[1][2]=0;
IM[2][0]=cos(a)*sin(b);
IM[2][1]=sin(a)*sin(b);
IM[2][2]=cos(b);


//M[0][0]=cos(a)*cos(b);
//M[0][1]=sin(a);
//M[0][2]=cos(a)*sin(b);
//M[1][0]=-1*sin(c)*sin(b)-sin(a)*cos(c)*cos(b);
//M[1][1]=cos(a)*cos(c);
//M[1][2]=sin(c)*cos(b)-sin(a)*cos(c)*sin(b);
//M[2][0]=sin(a)*sin(c)*cos(b)-cos(c)*sin(b);
//M[2][1]=-1*cos(a)*sin(c);
//M[2][2]=cos(c)*cos(b)+sin(a)*sin(c)*sin(b);
}
