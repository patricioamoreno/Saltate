#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include "vecinos.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
//#include "NRaphson.h"
//#include "TransMat.h"
//#include "trig.h"
//#include "limitate.h"
//#include "rebound.h"
#include "wallcoll2.h"
#include "Drag_Rot.h"
const double pi = 3.1415926535897;
using namespace std;



float x=120;
float y=50;
float z=64;
const int Nceldas=x*y*z;
const int Nparticulas=4; //depende del numero de particulas que se quiera correr
float T=5;
float dt=0.0005;
int pasos=T/dt;
int leer=0;

float LX=6.28*0.028;//0.17584
float LY=1*0.028;
float LZ=1.57*0.028*2;


float a=LX/x;
float b=pow(37.80027,(1/(y-1)));   //37.80027^(1/(y-1));
float c=LZ/z;
float m=1/(1+b*(pow(b,(y-1))-1)/(b-1));

struct PARTICULA{
    int ii;
    float x[3];//posicion
    float xr[3];//posicion total
    float u[3];//velocidad
    float ur[4];//velocidad relativa
    float rp[3];//rotacion
    float rpr[4];
    float ar[3];
    float t; //tiempo
    float ncelda; //numero celda a la que pertenece
    float Fdr[3];//fuerza drag
    float Fsw[3];//fuerza peso sumergido
    float Fvm;
    float Flf;
    float Fmg;
    float Cd;
};//tparticulas[Nparticulas];


PARTICULA* tparticulas = new PARTICULA[Nparticulas];


struct CELDAS{
    int ii;
    float xi[3];
    float Li[3];
    float ui[3];
    float vort[3];
    int nparticulas;
    int vecinos[27];
    int particulas[500];
};//tceldas[384000];

CELDAS* tceldas = new CELDAS[384000];

int DR=1; // Drag
int VM=1; // Virtual Mass
int SW=1; // Submerged Weight
int BS=0; // Basset
int MG=1; // Magnus
int LF=1; // Lift
int lubrication=2; //no se usa por ahora


float dp=0.56/1000; //diametro en metros
float g=9.8;
float nu=1*pow(10,-6); //m2/s

float rhos=2650; //kg/m3
float rho=1000; //kg/m3
float R=(rhos/rho-1);
float Rp=pow(R*g*pow(dp,3),0.5)/nu;
float CM=0.5; //virtual mass coeff, normalmente 0.5
float theta=0.001;
float alpha=1/(1+R+CM+VM);
float Taus=0.067;  //Tau star
float invTaus=1/Taus;
float Tauc=0.22*pow(Rp,-0.6)+0.06*pow(10,-7*pow(Rp,-0.6));
float e=0.65;
float f=0.1;
float NR=0;
float uu=0.024;
float CL=0.2;

void RelativeVel(PARTICULA &p,CELDAS &c){
	for (int i=0;i<3;i++){
	p.ur[i]=p.u[i]-c.ui[i];
	}
	p.ur[3]=sqrt(pow(p.ur[0],2)+pow(p.ur[1],2)+pow(p.ur[2],2));
}


void mover(PARTICULA &p){

    p.x[0]=fmod((p.x[0]+p.u[0]*dt*uu)+LX,LX);
    p.x[1]=p.x[1]+p.u[1]*dt*uu;//fmod((p.x[1]+p.u[1]*dt),LY);
    p.x[2]=fmod((p.x[2]+p.u[2]*dt*uu+LZ/2),LZ)-LZ/2;
    for (int i=0;i<3;i++) p.xr[i]=p.xr[i]+p.u[i]*dt*uu;

}

bool esta_particula_dentro_celda(PARTICULA &p, CELDAS &c){
    bool resultado=true;
    for (int i=0;i<3;i++){
        if (c.xi[i]>p.x[i] || p.x[i]>c.xi[i]+c.Li[i] ) resultado=false;
    }
    return resultado;
}

void FSubmergedWeight(int SW,float alpha,float theta,float invTaus,PARTICULA &p){
p.Fsw[0]=SW*(alpha*sin(theta)*invTaus);
p.Fsw[1]=SW*(alpha*cos(theta)*invTaus);
}

void FDrag(PARTICULA &p,float Taus,float Rp,int DR){
float Rep=p.ur[3]*sqrt(Taus)*Rp;
p.Cd=24/Rep*(1+0.15*sqrt(Rep)+0.017*Rep)-0.208/(1+10000*pow(Rep,-0.5));
p.Fdr[0]=DR*(0.75*alpha*p.Cd*p.ur[0]*p.ur[3]);
p.Fdr[1]=DR*(0.75*alpha*p.Cd*p.ur[1]*p.ur[3]);
p.Fdr[2]=DR*(0.75*alpha*p.Cd*p.ur[2]*p.ur[3]);
}

void FVirtualMass(PARTICULA &p, int VM,float alpha, float CM){
    float dufdz;
    if (p.x[1]==0) dufdz=0;
    else dufdz=2.5/(p.x[1]/dp);
    p.Fvm=VM*(alpha*CM*p.ur[2]*dufdz);
}

//void FLift(PARTICULA &p, float Taus,float dp,float alpha, float CL, int LF){
void FLift(PARTICULA &p, float Taus,float dp,float alpha, float CL, int LF,CELDAS &c, CELDAS &cbot1,CELDAS &cbot2,CELDAS &ctop1,CELDAS &ctop2){
float ufbot;
float uftop;
//float j=p.ncelda,k=p.ncelda;
float zbot,ztop;
//int n=0;
float z=p.x[1];
zbot=z-dp/2;
ztop=z+dp/2;
if (z<dp/2) ufbot=0;
else if(zbot >cbot1.xi[1]+cbot1.Li[1]/2 && zbot<=c.xi[1]+c.Li[1]/2){
ufbot=(zbot-(cbot1.xi[1]+cbot1.Li[1]/2))/(cbot1.Li[1]/2+c.Li[1]/2)*(cbot1.ui[1]-c.ui[1])+c.ui[1];
}
else if(zbot >cbot2.xi[1]+cbot2.Li[1]/2 && zbot<=cbot1.xi[1]+cbot1.Li[1]/2){
ufbot=(zbot-(cbot2.xi[1]+cbot2.Li[1]/2))/(cbot2.Li[1]/2+cbot1.Li[1]/2)*(cbot2.ui[1]-cbot1.ui[1])+cbot1.ui[1];
}
else if(zbot <cbot2.xi[1]+cbot2.Li[1]/2){
ufbot=cbot2.ui[1];
}

if(ztop <ctop1.xi[1]+ctop1.Li[1]/2 && ztop>=c.xi[1]+c.Li[1]/2){
uftop=(ztop-(ctop1.xi[1]+ctop1.Li[1]/2))/(ctop1.Li[1]/2+c.Li[1]/2)*(ctop1.ui[1]-c.ui[1])+c.ui[1];
}
else if(ztop <ctop2.xi[1]+ctop2.Li[1]/2 && ztop>=ctop1.xi[1]+ctop1.Li[1]/2){
uftop=(ztop-(ctop1.xi[1]+ctop1.Li[1]/2))/(ctop2.Li[1]/2+ctop1.Li[1]/2)*(ctop2.ui[1]-ctop1.ui[1])+ctop1.ui[1];
}
else if(ztop >ctop2.xi[1]+ctop2.Li[1]/2){
uftop=ctop2.ui[1];
}
//if (z<dp/2) ufbot=0;
//
//else{
//	zbot=p.x[1]-dp/2;
//	do{
//		j=j-120*n;
//		n++;
//	}while (zbot >tceldas[j].xi[1]+tceldas[j].Li[1] ||zbot <tceldas[j].xi[1]);
//}
//ztop=p.x[1]+dp/2;
//n=0;
//do{
//	k=k+120*n;
//	n++;
//}while (ztop >tceldas[k].xi[1]+tceldas[k].Li[1] ||ztop <tceldas[j].xi[1]);
//
//ufbot=tceldas[j].ui[0];
//uftop=tceldas[k].ui[0];



float ur2t=pow(p.u[0]-uftop,2)+pow(p.ur[1],2)+pow(p.ur[2],2);
float ur2b=pow(p.u[0]-ufbot,2)+pow(p.ur[1],2)+pow(p.ur[2],2);
p.Flf=LF*(0.75*alpha*CL*(ur2t-ur2b));



//    float RP=0.5;
//    float Re=RP*sqrt(Taus);
//	float uftop=2.5*log10(Re*(p.x[1]/dp+0.5))+5.5;                //Ufluid(dp,Taus,p.x[1]+dp/2,Rp)
//	float ufbot=2.5*log10(Re*(p.x[1]/dp-0.5))+5.5;
//	if (uftop<0) uftop=0;
//	if (ufbot<0) ufbot=0;
//	                //Ufluid(dp,Taus,p.x[1]-dp/2,Rp)
//	float ur2t=pow(p.u[0]-uftop,2)+pow(p.ur[1],2)+pow(p.ur[2],2);
//	float ur2b=pow(p.u[0]-ufbot,2)+pow(p.ur[1],2)+pow(p.ur[2],2);
//	p.Flf=LF*(0.75*alpha*CL*(ur2t-ur2b));
}

void FMagnus(PARTICULA &p, float dp, float alpha, int MG){
	float dufdz;
	if (p.x[1]==0) dufdz=0;
	else dufdz=2.5/(p.x[1]/dp);
	float omegaY=5.11-1.13*Taus/Tauc;
	p.Fmg=(0.75*alpha*p.ur[3])*(omegaY-0.5*dufdz*MG)*MG;
}

void NewVelocity(PARTICULA &p,float uu){
	p.u[0]=p.u[0]+dt*(-p.Fdr[0]+p.Fsw[0]+p.Fvm);
	p.u[1]=p.u[1]+dt*(-p.Fdr[1]-p.Fsw[1]+p.Fmg);//+p.Flf+p.Fmg
	p.u[2]=p.u[2]+dt*(-p.Fdr[2]);
}

void wallcoll(PARTICULA &p, int NR, float e, float f){
    float vrp[6];
        vrp[0]=p.u[0];
        vrp[1]=p.u[2];
        vrp[2]=p.u[1];
        vrp[3]=p.rp[0];
        vrp[4]=p.rp[2];
        vrp[5]=p.rp[1];

    WallColl2(vrp,NR,e,f);
        p.u[0]=vrp[0];
        p.u[1]=vrp[2];
        p.u[2]=vrp[1];
        p.rp[0]=vrp[3];
        p.rp[1]=vrp[5];
        p.rp[2]=vrp[4];
       // if (p.u[1]<0.015)p.u[1]=0.015;

}

void Ufluid(float LL, float Taus, float zz, CELDAS &c){

    float RP=0.5;
    //float UU=0.024;
    float z=zz/LL;
    float Re=RP*sqrt(Taus);
    if (z>0){
        if (Re<5)  c.ui[0]=2.5*log10(Re*z)+5.5;//smooth wall
        else if (Re>=5 && Re<70)  c.ui[0]=2.5*log10(Re*z)+5.5-2.5*log10(1+0.3*Re);//transition wall
        else   c.ui[0]=2.5*log10(30*z); //rough wall
    }
    else c.ui[0]=0;
    //c.ui[0]=c.ui[0];
    c.ui[1]=0.5;

}

void RelativeRot(PARTICULA &p,CELDAS &c){
	for (int i=0;i<3;i++){
	p.rpr[i]=p.rp[i]-c.vort[i];
	}
	p.rpr[3]=sqrt(pow(p.rpr[0],2)+pow(p.rpr[1],2)+pow(p.rpr[2],2));
}


void AcelAng(PARTICULA &p,CELDAS &c,float R, float Taus, float Rp){
	float Ct=Drag_Rot(p.rpr[3],Taus,Rp);
	float eta=-15*Ct/(16*pi*(R+1));
	for (int i=0;i<3;i++){
		p.ar[i]=eta*p.rpr[3]*(p.rp[i]-c.vort[i]);
		if (p.ar[i]<-10) p.ar[i]=-10;
		if (p.ar[i]>10) p.ar[i]=10;
	}
}

void NewRotp(PARTICULA &p){
	for (int i=0;i<3;i++){
		p.rp[i]=p.rp[i]+p.ar[i]*dt;
	}

}


int main(){
    for(int i=0;i<Nceldas;i++){
        float aux=floor(fmod(i,x*y)/x);
        tceldas[i].ii=i;
        tceldas[i].xi[0]=fmod(i,x)*a;
        tceldas[i].xi[1]=m*(1+b*(pow(b,aux-1)-1)/(b-1))*0.028;
        tceldas[i].xi[2]=floor((i+1)/(x*y))*c-LZ/2;
        tceldas[i].Li[0]=a;
        tceldas[i].Li[1]=m*pow(b,(aux))*0.028;//m*pow(b,(y-aux-1))*0.028;
        tceldas[i].Li[2]=c;
        tceldas[i].nparticulas=0;
        vecinos(tceldas[i].vecinos, i);
        Ufluid(dp,Taus,tceldas[i].xi[1]+tceldas[i].Li[1]/2,tceldas[i]);
        tceldas[i].vort[1]=2.5/(tceldas[i].xi[1]+tceldas[i].Li[1]/2);
    }

    ifstream lectura;
    lectura.open("U", ios::out | ios::in | ios::binary);
    float vx;
    float vy;
    float vz;
    char a1[1];
    char a2[1];

    if (leer==1){
    //float v[384000][3];
    lectura.seekg(859, ios::beg);
    for (int i=0;i<Nceldas;i++){
        lectura.read(a1,1);
        lectura>>vx;
        lectura>>vy;
        lectura>>vz;
        lectura>>a2;
        lectura.read(a1,1);
        tceldas[i].ui[0]=vx/uu/3;
        tceldas[i].ui[1]=vy/uu/3;
        tceldas[i].ui[2]=vz/uu/3;
        //printf("%f  %f  %f  %s \n",vx,vy,vz,a2);
    }
    lectura.close();
    }

    for(int i=0;i<Nparticulas;i++){
        int aux3=(rand() % 110+1)+360;
        int aux1=fmod(i*x*y,x*y*z)+aux3;
//        while(floor(fmod(aux1,x*y)/x)>10){
//        aux1=rand() % 384000;
//        }
        int aux2=rand() % 6+1;
        tparticulas[i].ii=i;
        for (int j=0;j<3;j++){
        tparticulas[i].x[j]=tceldas[aux1].xi[j]+tceldas[aux1].Li[j]*aux2/10;
        tparticulas[i].xr[j]=tparticulas[i].x[j];
        }

//      tparticulas[i].x[0]=tceldas[aux1].xi[0]+tceldas[aux1].Li[0]*aux2/10;
//      tparticulas[i].x[1]=tceldas[aux1].xi[1]+tceldas[aux1].Li[1]*aux2/10;
//      tparticulas[i].x[2]=tceldas[aux1].xi[2]+tceldas[aux1].Li[2]*aux2/10;
        tparticulas[i].u[0]=4.5;//(aux2+0.1)/9*0.3;
        tparticulas[i].u[1]=1.5;//(aux2+0.1)/9*0.2;
        tparticulas[i].u[2]=0;//(aux2+0.1)/9*0.3;
        tparticulas[i].ncelda=aux1;
        tparticulas[i].t=0;
        tparticulas[i].rp[0]=0;//0.8;
        tparticulas[i].rp[1]=0;//0.8;
        tparticulas[i].rp[2]=0;//0.8;
        //tparticulas[i].t=0;
        tceldas[aux1].nparticulas=tceldas[aux1].nparticulas+1;
//        tceldas[aux1].particulas=&tparticulas[i];
        //revisar punteros entre la celda y las particulas que estan dentro
        //struct PARTICULA *tceldas[aux1].particulas[tceldas[aux1].nparticulas-1];

        tceldas[aux1].particulas[tceldas[aux1].nparticulas-1] = i;
    }

//    printf("%f  %f \n",tceldas[0].xi[1],tceldas[0].Li[1]);
//    printf("%f  %f \n",tceldas[120].xi[1],tceldas[120].Li[1]);
//    printf("%f  %f \n",tceldas[240].xi[1],tceldas[240].Li[1]);
//    for (int i=0;i<27;i++){
//    printf("%d \n",tceldas[275386].vecinos[i]);
//
//    }
    //float *salida1 = new float[(pasos+1)*(3*Nparticulas+1)];//[pasos+1][3*Nparticulas+1];
    //float **salida = &salida1;

    float **salida = new float*[pasos+1];
    for(int i = 0; i < 1; ++i) {
        salida[i] = new float[3*Nparticulas+1];
    }

    int ti=0;

    printf("Hola.%d\n",100);

    FILE *doc;
    doc=fopen("salida.txt","w+");

    for(float t=dt;t<T;t=t+dt){

        for(int i=0;i<Nceldas;i++){
            int nn=0;
            if (tceldas[i].nparticulas>0){
                for(int k=0;k<tceldas[i].nparticulas;k++){
                    int ii=tceldas[i].particulas[k];
                    //printf(".");
                    if(tparticulas[ii].t<t){
                        //std::cout << "." ;
                        //printf(".");
                        PARTICULA &q = tparticulas[ii];
                        //std::cout << "&q = " << &q << std::endl;
                        mover(q);
                        tparticulas[ii].t=t;
                        RelativeVel(tparticulas[ii],tceldas[i]);
                        RelativeRot(tparticulas[ii],tceldas[i]);
                        //printf("%f \n",tparticulas[ii].u[1]);

                        FDrag(tparticulas[ii],Taus,Rp,DR);
                        FSubmergedWeight(SW,alpha,theta,invTaus,tparticulas[ii]);
                        FVirtualMass(tparticulas[ii], VM,alpha, CM);
                        FLift(tparticulas[ii], Taus, dp, alpha, CL, LF,tceldas[i],tceldas[i-120],tceldas[i-240],tceldas[i+240],tceldas[i+240]);
                        FMagnus(tparticulas[ii], dp, alpha, MG);
                        NewVelocity(tparticulas[ii],uu);
                        if (tparticulas[ii].x[1]<dp/2){
                            tparticulas[ii].x[1]=dp/2;
                            tparticulas[ii].xr[1]=dp/2;
                            wallcoll(tparticulas[ii],e,f,NR);
                        }
                        AcelAng(tparticulas[ii],tceldas[i],R,Taus,Rp);
                        NewRotp(tparticulas[ii]);
                        for (int z=0;z<3;z++){
                            salida[0][ii*3+z]=tparticulas[ii].xr[z];
                        }
                        //printf("%f     %f     %f     %d \n",salida[ti][0],salida[ti][1],salida[ti][2],ti);
                        //printf("%f     %f     %f     \n",tparticulas[ii].Fmg,tparticulas[ii].Flf,tparticulas[ii].Fvm);
                        //printf("%f     %f     %d     %f\n",tparticulas[ii].u[1],tparticulas[ii].x[1],i,t);
                        //printf("%f \n",t);
                    }
                    int r=esta_particula_dentro_celda(tparticulas[ii],tceldas[i]);
                    if(r==1){
                        tceldas[i].particulas[nn]=ii;
                        nn=nn+1;
                    }
                    else if (r==0){
                        int j=1;
                        int nc;
                        while(r==0 && j<27){
                            nc=tceldas[i].vecinos[j];
                            r=esta_particula_dentro_celda(tparticulas[ii],tceldas[nc]);
                            j++;
                        }
                        if (r==1){
                            tceldas[nc].particulas[tceldas[nc].nparticulas]=ii;
                            tceldas[nc].nparticulas=tceldas[nc].nparticulas+1;
                        }
                        }
                    }
                }
                tceldas[i].nparticulas=nn;
            }
            ti++;
            for (int j=0;j<Nparticulas*3;j++){
                if (j==0) {
                    fprintf(doc,"\n%f\t",salida[0][j]);
                }
                else{
                    fprintf(doc,"%f\t",salida[0][j]);
                }
            }
        }

    fclose(doc);
    int tt=0;
    printf("%d \n",tt);
    for (int k=0;k<Nceldas;k++){
    tt=tceldas[k].nparticulas+tt;
    }
    printf("%d \n",tt);

//    FILE *doc;
//
//    doc=fopen("salida.txt","w+");
//    for (int i=0;i<pasos-1;i++){
//        for (int j=0;j<Nparticulas*3;j++){
//            if (j==0) {
//                fprintf(doc,"\n%f\t",salida[i][j]);//fprintf(doc,"\n%f\t%f\t%f\t",salida[i][p],salida[i][p+1],salida[i][p+2]);
//                //printf("\n");
//                //printf("%f\t",salida[i][j]);
//            }
//            else{
//                //printf("%f\t",salida[i][j]);
//                fprintf(doc,"%f\t",salida[i][j]);//fprintf(doc,"%f\t%f\t%f\t",salida[i][p],salida[i][p+1],salida[i][p+2]);
//            }
//        }
//    }
//    fclose(doc);
//    return 0;
}


//    for(float t=dt;t<T;t=t+dt){
//        for(int i=0;i<Nparticulas;i++){
//            mover(tparticulas[i]);
//        }
//        printf("x= %f y= %f z= %f \n",tparticulas[2].x[0],tparticulas[1].x[1],tparticulas[1].x[2]);
//    }


//for (int k=0;k<27;k++){
//printf("%d \n",tceldas[120].vecinos[k]);
//}



//printf("%f \n",tparticulas[25].ncelda);
//printf("%d \n",tparticulas[25].ii);
//printf("%f \n",tparticulas[26].u[0]);
//printf("%f \n",tparticulas[25].u[1]);
//printf("%f \n",tparticulas[25].u[2]);




