#include<stdio.h>
#include<math.h>

void mover(PARTICULA &p){

    p.x[0]=fmod((p.x[0]+p.u[0]*dt),LX);
    p.x[1]=fmod((p.x[1]+p.u[1]*dt),LY);
    p.x[2]=fmod((p.x[2]+p.u[2]*dt+LZ/2),LZ)-LZ/2;
}
