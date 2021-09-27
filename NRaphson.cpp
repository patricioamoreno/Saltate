#include<math.h>
#include<stdlib.h>


float NRaphson(float titac,float r_dp,int NR){
const double pi = 3.1415926535897;
int sw;
if(r_dp>=0) sw=1;
else if(r_dp<0) sw=-1;
r_dp=abs(r_dp);
float titab=pi/12;
double diff=1000;
float angle=pi/5;
double titabnew;

while (diff>0.00000001){

if (NR==0){
titabnew=titab-(r_dp-0.5*(cos(titab)-tan(titac)*sin(titab)))/(0.5*(sin(titab)+tan(titac)*cos(titab)));
}
else if (NR==1){
titabnew=titab-(r_dp-0.5*(sin(titab)-tan(titac)*cos(titab)))/(-0.5*(cos(titab)+tan(titac)*sin(titab)));
}
diff=abs(titabnew-titab);
titab=titabnew;
}

if (NR==1) titab=pi/2-titab;

if (titab>angle) titab=angle;
else if (titab<-1*angle) titab=-1*angle;

titab=titab*sw;
return titab;
}

