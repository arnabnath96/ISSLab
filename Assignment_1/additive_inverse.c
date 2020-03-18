#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int additive_inverse(int n , int m)
{
	if(n>0) return (m-n);
	return abs(m+n);
}

int main()
{
	int number, mod;

	printf("Modular Additive Inverse of a Number\n");

	printf("input : ");
	scanf("%d",&number);
	printf("mod : ");
	scanf("%d",&mod);

	printf("Inverse = %d\n",additive_inverse(number,mod));
	
}
