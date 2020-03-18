#include<stdio.h>
int main()
{
	int a0, b0, t0=0, t=1, s0=1, s=0;

	
	printf("Enter two numbers :");
	scanf("%d %d",&a0,&b0);

    
	int q=a0/b0;
	int r=a0-q*b0;

	int temp;

	while(r>0)
	{
		temp=t0-q*t;
		t0=t;
		t=temp;

		temp=s0-q*s;
		s0=s;
		s=temp;
		a0=b0;
		b0=r;
		q=a0/b0;
		r=a0-q*b0;
	}
	r=b0;

	printf("GCD=%d\n",r);
	printf("s=%d\n",s);
	printf("t=%d\n",t);

return 0;	
}
