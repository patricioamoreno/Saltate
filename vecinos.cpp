#include<stdio.h>
#include<math.h>
//dato de entrada: N° real de la celda



int vecinos(int *v, int ii){

float x=120;
float y=50;
float z=64;
float i=ii+1;

float a=fmod(i-1,x);
float b=fmod(i-1,x*y);
float c=floor((i-1)/(x*y));


    if (b>(x-1) && b<x*(y-1)){

        if (c<(z-1) && c>0){
            if (a>0 && a<(x-1)){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                    v[j+6]=v[j]+x;
                }
                for(int j=0;j<9;j++){
                    v[j+9]=v[j]+x*y;
                    v[j+18]=v[j]-x*y;
                }
            }
            else if (a==0){
                v[0]= i-1;
                v[1]= i+x-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                    v[j+6]=v[j]+x;
                }
                for(int j=0;j<9;j++){
                    v[j+9]=v[j]+x*y;
                    v[j+18]=v[j]-x*y;
                }
            }
            else if (a==x-1){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i-x;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                    v[j+6]=v[j]+x;
                }
                for(int j=0;j<9;j++){
                    v[j+9]=v[j]+x*y;
                    v[j+18]=v[j]-x*y;
                }
            }
        }
        else if (c==0){
            if (a>0 && a<(x-1)){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                    v[j+6]=v[j]+x;
                }
                for(int j=0;j<9;j++){
                    v[j+9]=v[j]+x*y;
                    v[j+18]=v[j]+x*(z-1)*y;
                }
            }
            else if (a==0){
                v[0]= i-1;
                v[1]= i+x-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                    v[j+6]=v[j]+x;
                }
                for(int j=0;j<9;j++){
                    v[j+9]=v[j]+x*y;
                    v[j+18]=v[j]+x*(z-1)*y;
                }
            }
            else if (a==x-1){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i-x;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                    v[j+6]=v[j]+x;
                }
                for(int j=0;j<9;j++){
                    v[j+9]=v[j]+x*y;
                    v[j+18]=v[j]+x*(z-1)*y;
                }
            }
        }
        else if (c==(z-1)){
            if (a>0 && a<(x-1)){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                    v[j+6]=v[j]+x;
                }
                for(int j=0;j<9;j++){
                    v[j+9]=v[j]-x*(z-1)*y;
                    v[j+18]=v[j]-x*y;
                }
            }
            else if (a==0){
                v[0]= i-1;
                v[1]= i+x-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                    v[j+6]=v[j]+x;
                }
                for(int j=0;j<9;j++){
                    v[j+9]=v[j]-x*(z-1)*y;
                    v[j+18]=v[j]-x*y;
                }
            }
            else if (a==x-1){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i-x;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                    v[j+6]=v[j]+x;
                }
                for(int j=0;j<9;j++){
                    v[j+9]=v[j]-x*(z-1)*y;
                    v[j+18]=v[j]-x*y;
                }
            }
        }
    }
    else if (b<x){
        if (c<(z-1) && c>0){
            if (a>0 && a<(x-1)){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]+x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]-x*y;
                }
            }
            else if (a==0){
                v[0]= i-1;
                v[1]= i+x-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]+x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]-x*y;
                }
            }
            else if (a==x-1){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i-x;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]+x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]-x*y;
                }
            }
        }
        else if (c==0){
            if (a>0 && a<(x-1)){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]+x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]+x*(z-1)*y;
                }
            }
            else if (a==0){
                v[0]= i-1;
                v[1]= i+x-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]+x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]+x*(z-1)*y;
                }
            }
            else if (a==x-1){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i-x;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]+x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]+x*(z-1)*y;
                }
            }
        }
        else if (c==(z-1)){
            if (a>0 && a<(x-1)){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]+x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]-x*(z-1)*y;
                    v[j+12]=v[j]-x*y;
                }
            }
            else if (a==0){
                v[0]= i-1;
                v[1]= i+x-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]+x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]-x*(z-1)*y;
                    v[j+12]=v[j]-x*y;
                }
            }
            else if (a==x-1){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i-x;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]+x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]-x*(z-1)*y;
                    v[j+12]=v[j]-x*y;
                }
            }
        }
    }
    else if (b>x*(y-1)-1){
        if (c<(z-1) && c>0){
            if (a>0 && a<(x-1)){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]-x*y;
                }
            }
            else if (a==0){
                v[0]= i-1;
                v[1]= i+x-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]-x*y;
                }
            }
            else if (a==x-1){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i-x;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]-x*y;
                }
            }
        }
        else if (c==0){
            if (a>0 && a<(x-1)){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]-x*(z-1)*y;
                }
            }
            else if (a==0){
                v[0]= i-1;
                v[1]= i+x-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]-x*(z-1)*y;
                }
            }
            else if (a==x-1){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i-x;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]+x*y;
                    v[j+12]=v[j]-x*(z-1)*y;
                }

            }
        }
        else if (c==(z-1)){
            if (a>0 && a<(x-1)){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]-x*(z-1)*y;
                    v[j+12]=v[j]-x*y;
                }
            }
            else if (a==0){
                v[0]= i-1;
                v[1]= i+x-2;
                v[2]= i;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]-x*(z-1)*y;
                    v[j+12]=v[j]-x*y;
                }
            }
            else if (a==x-1){
                v[0]= i-1;
                v[1]= i-2;
                v[2]= i-x;
                for(int j=0;j<3;j++){
                    v[j+3]=v[j]-x;
                }
                for(int j=0;j<6;j++){
                    v[j+6]=v[j]-x*(z-1)*y;
                    v[j+12]=v[j]-x*y;
                }
            }
        }
    }
//if k>0
//    vecinos=vv;
//elseif j>0
//    vecinos=v;
//for (int j=0;j<26;j++){
//printf("%f \n",v[j]);
//}

return 0;
}

