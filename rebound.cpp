#include<stdio.h>
#include<math.h>

void Rebound(float vw[],float e,float f){

float rp=0.5;//0.58/1000;
double vmod=sqrt(pow(vw[0]+rp*vw[4],2)+pow(vw[1]-rp*vw[3],2));
float value=-2/(7*f*(e+1));
float ex=(vw[0]+rp*vw[4])/vmod;
float ey=(vw[1]-rp*vw[3] )/vmod;
float vw2[6];

if (vw[2]/vmod<value){
    //swtch=1;
    vw2[0]=(5/7)*(vw[0]-2*rp/5*vw[4]);
    vw2[1]=-e*vw[1];
    vw2[2]=(5/7)*(vw[1]+2*rp/5*vw[3]);

    vw2[3]=vw2[1]/rp;
    vw2[4]=-vw2[0]/rp;
    vw2[5]=vw[5];
}
else{
    //swtch=2;
    vw2[0]=vw[0]+ex*f*(e+1)*vw[2];
    vw2[1]=vw[1]+ey*f*(e+1)*vw[2];
    vw2[2]=-e*vw[1];

    vw2[3]=vw[3]-5/(2*rp)*ey*f*(e+1)*vw[2];
    vw2[4]=vw[4]-5/(2*rp)*ex*f*(e+1)*vw[2];
    vw2[5]=vw[5];
    }


for (int i=0;i<6;i++){
vw[i]=vw2[i];
}
}
