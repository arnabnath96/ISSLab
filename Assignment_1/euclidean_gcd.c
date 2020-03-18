#include<stdio.h>

int main()
{
	int r[100005];
	int result[100005];
	int k=0;

	printf("Enter two numbers:\n");
	scanf("%d %d",&r[0],&r[1]);

	int quotient;
	int m=1;

	printf("q\tr1\tr2\tr\n");
	while(r[m]>0)
	{
		result[k++]=r[m-1]/r[m];
		printf("%d\t%d\t%d\t%d\n",result[k-1], r[m-1],r[m],r[m-1]%r[m]);
		r[m+1]=r[m-1]-result[k-1]*r[m];
		m++;		
	}
	m=m-1;

	printf("GCD: %d\n", r[m]);

return 0;
}
