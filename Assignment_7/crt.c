#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int multiplicative_inverse(int n,int m)
{
	int i;
	for(i=1;i<m;i++)
		if((i*n)%m==1) return i;
	return -1;
}

int gcd(int a0, int b0)
{
	
	int q=a0/b0;
	int r=a0-q*b0;
	int temp;

	while(r>0)
	{
		a0=b0;
		b0=r;
		q=a0/b0;
		r=a0-q*b0;
	}
	return b0;
}


int main()
{
	int no_of_eqn;
	printf("Number of equations\t");
	scanf("%d",&no_of_eqn);

	int x,*b,*m,*M,*M_inv;
	long long int N=1;
	b = (int*)malloc(no_of_eqn * sizeof(int));
	m = (int*)malloc(no_of_eqn * sizeof(int));
	M = (int*)malloc(no_of_eqn * sizeof(int));
	M_inv = (int*)malloc(no_of_eqn * sizeof(int));

	printf("\n     x ≡  b (mod m) \n\n     Enter b and m seperated by space\n\n");
 
	for(int i=0;i<no_of_eqn;i++)
	{
		printf("Eqn %d\t",i+1);
		scanf("%d %d",&b[i],&m[i]);
	}
	
	for (int i = 0; i < no_of_eqn - 1; i++)  
        	for (int j = i + 1; j < no_of_eqn; j++) 
            		if (gcd(m[i], m[j])!=1) 
               		{
				printf("%d and %d are not relatively co-prime", m[i],m[j]);
				return 0;
			}

	for(int i=0;i<no_of_eqn;i++)
		N*=m[i];

	for(int i=0;i<no_of_eqn;i++)
		M[i] = N/m[i];
	
	for(int i=0;i<no_of_eqn;i++)
        {		
		M_inv[i] = multiplicative_inverse(M[i],m[i]);
		if(M_inv[i]==-1)
		{
			printf("Multiplicative inverse of %d does not exists",M[i]);
			return 0;
		}
	}
		
	long long int P=0;

	for(int i=0;i<no_of_eqn;i++)
		P = P + (b[i]*M[i]*M_inv[i]);
	
	P = P%N;
	
	printf("\n\n     x = %lld\n\n",P); 

return 0;
}
