#include<math.h>
#include<stdlib.h>

float trig(float titc,float titb){
float r_dp;
if (titb>0) r_dp=fabs(0.5*(cos(titb)-tan(titc)*sin(titb)));
else r_dp=0.5*(cos(fabs(titb))-tan(titc)*sin(fabs(titb)));

return r_dp;
}
