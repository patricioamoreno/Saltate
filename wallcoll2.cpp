#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#include "NRaphson.h"
#include "TransMat.h"
#include "trig.h"
#include "limitate.h"
#include "rebound.h"
#include "invertirM.h"

void WallColl2 (float vrp[],int NR,float e, float f){
    float upm=vrp[0];
    float vpm=vrp[1];
    float wpm=vrp[2];
    float rpx=vrp[3];
    float rpy=vrp[5];
    float rpz=vrp[4];

    const double pi = 3.1415926535897;
        //titab
    float maxtitab=pi/6;
    float titac=atan(fabs(wpm/upm));

    int rmin=0;
    float rmaxtita=trig(titac,maxtitab);

    float rand1=rand()%(101-1);
    float r_dptita=rand1/101*rmaxtita;
    if (r_dptita>0.5) r_dptita=0.5;

    if (NR==1) r_dptita=limitate(rmin,rmaxtita,r_dptita);

    float titab=NRaphson(titac,r_dptita,NR);

        //alfab
    float maxalfab=pi/6;
    float alfac=atan(fabs(vpm)/upm);

    rmin=0;
    float rmaxalfa=trig(alfac,maxalfab);
    float rand2=rand()%(101-1);
    float r_dpalfa=rand2/101*rmaxalfa;


    if (NR==1) r_dpalfa=limitate(rmin,rmaxalfa,r_dpalfa);
    if (r_dpalfa>0.5) r_dpalfa=0.5;
    //r_dpalfa=rand()%(101-1)/101*r_dpalfa;

    float alfab=NRaphson(alfac,r_dpalfa,NR);
    if (vpm<0) alfab=-1*alfab;

        //gamab
    float maxgamab=pi/6;
    float gamac=atan(fabs(wpm/vpm));

    rmin=0;
    float rmaxgama=trig(gamac,maxgamab);
    float rand3=rand()%(101-1);
    float r_dpgama=rand3/101*rmaxgama;

    if (NR==1) r_dpgama=limitate(rmin,rmaxgama,r_dpgama);
    //r_dpgama=rand()%(101-1)/101*r_dpgama;
    if (r_dpgama>0.5) r_dpgama=0.5;

    float gamab=NRaphson(gamac,r_dpgama,NR);
    if (vpm<0) gamab=-1*gamab;

        //matriz de transformacion

    float M[3][3];
    float invM[3][3];
    TransMat(M,invM,alfab,titab,gamab);
    float vrp2[6];

    for (int i=0;i<3;i++){
        vrp2[i]=M[i][0]*vrp[0]+M[i][1]*vrp[1]+M[i][2]*vrp[2];
        vrp2[i+3]=M[i][0]*vrp[3]+M[i][1]*vrp[4]+M[i][2]*vrp[5];
    }
    Rebound(vrp2,e,f);
    invertirM(M,invM);
    for (int i=0;i<3;i++){
        vrp[i]=invM[i][0]*vrp2[0]+invM[i][1]*vrp2[1]+invM[i][2]*vrp2[2];
        vrp[i+3]=invM[i][0]*vrp2[3]+invM[i][1]*vrp2[4]+invM[i][2]*vrp2[5];
    }

}
