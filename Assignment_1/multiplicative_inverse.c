#include<stdio.h>
#include<math.h>

int multiplicative_inverse(int n,int m)
{

    int i;

    for(i=1;i<m;i++)
    	if((i*n)%m==1) return i;

    return -1;
}

int main()
{
	int number,mod, inv;
	printf("Modular Multiplicative Inverse of a Number\n");
	printf("input : ");
	scanf("%d",&number);
	printf("mod : ");
	scanf("%d",&mod);

	inv = multiplicative_inverse(number,mod);
	
	if(inv == -1)
		printf("inverse = %s\n","does not exist");
	else
		printf("inverse = %d\n",inv);
    
}
