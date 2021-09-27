#include <stdio.h>
#include <iostream>

int i,j, k, s;
float m1[1000][1000], coef, aux[1000], elemento;

void invertirM(float M[][3],float invM[][3])
{
	k=3;
	for (i=0;i<k;i++){
		for (j=0;j<k;j++){
			m1[i][j]=M[i][j];
		}
	}

//	for (i=0;i<k;i++)
//		{
//			for (j=0;j<k;j++)
//			{
//				cout << "Ingrese el valor de [" <<i<<"]["<<j<<"]";
//				cin >> m1 [i][j];
//			}
//		}

	for (i=0;i<k;i++)
		for(j=k;j<2*k;j++)
		{
			if(i==(j-k))
				m1[i][j]=1;
			else
				m1[i][j]=0;
		}
		//Iteraciones
		for (s=0;s<k;s++)
		{
			elemento=m1[s][s];
			for (j=0;j<2*k;j++)
				m1[s][j]=m1[s][j]/elemento;

			for(i=0;i<k;i++)
			{
				if (i==s)
					;
				else
				{
					coef= m1[i][s];
					for (j=0;j<3*k;j++)
						aux[j]=m1[s][j]*(coef*-1);
					for (j=0;j<3*k;j++)
						m1[i][j]=m1[i][j]+aux[j];
				}
			}
		}

        for (int i=0;i<3;i++){
            for (int j=3;j<6;j++){
                invM[i][j-3]=m1[i][j];
            }

        }
}

