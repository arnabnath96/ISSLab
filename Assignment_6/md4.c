#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "banner.c" 

static char DEBUG = 0;

static unsigned int *MD4Digest(unsigned int *w, int len);
static void setMD4Registers(unsigned int AA, unsigned int BB, unsigned int CC, unsigned int DD);
static unsigned int changeEndianness(unsigned int x);
static void resetMD4Registers(void);
 
static const char *BASE16 = "0123456789abcdef";
 
#define F(X,Y,Z) (((X)&(Y))|((~(X))&(Z)))
#define G(X,Y,Z) (((X)&(Y))|((X)&(Z))|((Y)&(Z)))
#define H(X,Y,Z) ((X)^(Y)^(Z))
 
#define LEFTROTATE(A,N) ((A)<<(N))|((A)>>(32-(N)))
 
#define MD4ROUND1(a,b,c,d,x,s) a += F(b,c,d) + x; a = LEFTROTATE(a, s);
#define MD4ROUND2(a,b,c,d,x,s) a += G(b,c,d) + x + (unsigned int)0x5A827999; a = LEFTROTATE(a, s);
#define MD4ROUND3(a,b,c,d,x,s) a += H(b,c,d) + x + (unsigned int)0x6ED9EBA1; a = LEFTROTATE(a, s);
 
static unsigned int A = 0x67452301;
static unsigned int B = 0xefcdab89;
static unsigned int C = 0x98badcfe;
static unsigned int D = 0x10325476;
 
void handle_sigint(int sig) 
{ 
	signal(sig, SIG_IGN);
	printf("\n\nOUCH, you just hit Ctrl-C.\n");
	printf("Exiting\n");
	exit(0);
}

void hexEncode(char* in, char *out){
	
	int i,j;
	j=0;
	for(i=0; i<256; i++){
		out[j++]=BASE16[((in[i] & 0xF0)>>4)];
		out[j++]=BASE16[(in[i] & 0x0F)];
	}
	out[j]='\0';
} 
 
void intToString(unsigned int l,char *s)
{
	for(int i=0; i<4; i++)
		s[i] = (l >> (8*(3-i))) & 0xFF;
}
 
unsigned int stringToint(char* s)
{
	unsigned int l=0;
	for(int i=0; i<4; i++)
		l = l|(((unsigned int)((unsigned char)s[i]))<<(8*(3-i)));

	return l;
}

char *str_cat(char* dst, int dlen, char* src, int slen)
{
    for(int i=0;i<slen;i++)
        dst[dlen+i] = src[i];

    dst[dlen+slen] = '\0';    
    return dst;
}

char *zero_pad(char* dst, int dlen, int slen)
{
    unsigned char zero = 0x00;

    for(int i=0;i<slen;i++)
        dst[dlen+i] = zero;

    dst[dlen+slen] = '\0';    
    return dst;
}

char *MD4(char *str, int len){

    long int msglen;
	unsigned int *w;
	unsigned int *hash;
	unsigned long long mlen;
	unsigned char oneBit = 0x80;
	int i, wlen;
	char *digest = calloc(16, sizeof(char));

	mlen = strlen(str);

	if(DEBUG==1)
		printf("\nMessage = '%.*s'\nMessage Length = %llu\n", (int)mlen, str, mlen);

	str = str_cat(str,strlen(str),(char *)&oneBit,1);

	msglen = strlen(str);
    i=(((56-msglen)%64)+64)%64;

    str = zero_pad(str,msglen,i);
    msglen+=i;
    
	if(DEBUG==1)
		printf("Message Length after padding = %ld\n", msglen);
	

    w = malloc(sizeof(unsigned int)*(msglen/4+2));
	
	for(i=0; i<msglen/4; i++)
		w[i]=stringToint(&str[4*i]);
	
    w[i++] = (mlen<<3) & 0xFFFFFFFF;
	w[i++] = (mlen>>29) & 0xFFFFFFFF; 
	wlen=i;

	for(i=0; i<wlen-2; i++){
		w[i]=changeEndianness(w[i]);
	}

    hash = MD4Digest(w,wlen);

	for(i=0; i<4; i++)
		hash[i]=changeEndianness(hash[i]);

    for(i=0; i<4; i++)
        intToString(hash[i],(char*)&digest[i*4]);

	if(DEBUG==1)
    	printf("Digest = %s\n",digest);

    char *out =  calloc(33,sizeof(char));
    hexEncode(digest,out);
	out[32]='\0';

	return out;
}

unsigned int *MD4Digest(unsigned int *w, int len){

	int i,j;
	unsigned int X[16];
	unsigned int *digest = malloc(sizeof(unsigned int)*4);
	unsigned int AA, BB, CC, DD;
 
	for(i=0; i<len/16; i++)
	{
		for(j=0; j<16; j++)
			X[j]=w[i*16+j];
 
		AA=A;
		BB=B;
		CC=C;
		DD=D;
 
		MD4ROUND1(A,B,C,D,X[0],3);
		MD4ROUND1(D,A,B,C,X[1],7);
		MD4ROUND1(C,D,A,B,X[2],11);
		MD4ROUND1(B,C,D,A,X[3],19);
		MD4ROUND1(A,B,C,D,X[4],3);
		MD4ROUND1(D,A,B,C,X[5],7);
		MD4ROUND1(C,D,A,B,X[6],11);
		MD4ROUND1(B,C,D,A,X[7],19);
		MD4ROUND1(A,B,C,D,X[8],3);
		MD4ROUND1(D,A,B,C,X[9],7);
		MD4ROUND1(C,D,A,B,X[10],11);
		MD4ROUND1(B,C,D,A,X[11],19);
		MD4ROUND1(A,B,C,D,X[12],3);
		MD4ROUND1(D,A,B,C,X[13],7);
		MD4ROUND1(C,D,A,B,X[14],11);
		MD4ROUND1(B,C,D,A,X[15],19);
 
		MD4ROUND2(A,B,C,D,X[0],3);
		MD4ROUND2(D,A,B,C,X[4],5);
		MD4ROUND2(C,D,A,B,X[8],9);
		MD4ROUND2(B,C,D,A,X[12],13);
		MD4ROUND2(A,B,C,D,X[1],3);
		MD4ROUND2(D,A,B,C,X[5],5);
		MD4ROUND2(C,D,A,B,X[9],9);
		MD4ROUND2(B,C,D,A,X[13],13);
		MD4ROUND2(A,B,C,D,X[2],3);
		MD4ROUND2(D,A,B,C,X[6],5);
		MD4ROUND2(C,D,A,B,X[10],9);
		MD4ROUND2(B,C,D,A,X[14],13);
		MD4ROUND2(A,B,C,D,X[3],3);
		MD4ROUND2(D,A,B,C,X[7],5);
		MD4ROUND2(C,D,A,B,X[11],9);
		MD4ROUND2(B,C,D,A,X[15],13);
 
		MD4ROUND3(A,B,C,D,X[0],3);
		MD4ROUND3(D,A,B,C,X[8],9);
		MD4ROUND3(C,D,A,B,X[4],11);
		MD4ROUND3(B,C,D,A,X[12],15);
		MD4ROUND3(A,B,C,D,X[2],3);
		MD4ROUND3(D,A,B,C,X[10],9);
		MD4ROUND3(C,D,A,B,X[6],11);
		MD4ROUND3(B,C,D,A,X[14],15);
		MD4ROUND3(A,B,C,D,X[1],3);
		MD4ROUND3(D,A,B,C,X[9],9);
		MD4ROUND3(C,D,A,B,X[5],11);
		MD4ROUND3(B,C,D,A,X[13],15);
		MD4ROUND3(A,B,C,D,X[3],3);
		MD4ROUND3(D,A,B,C,X[11],9);
		MD4ROUND3(C,D,A,B,X[7],11);
		MD4ROUND3(B,C,D,A,X[15],15);
 
		A+=AA;
		B+=BB;
		C+=CC;
		D+=DD;
	}
 
	digest[0]=A;
	digest[1]=B;
	digest[2]=C;
	digest[3]=D;
	resetMD4Registers();
	return digest;
}
 
unsigned int changeEndianness(unsigned int x){
	return ((x & 0xFF) << 24) | ((x & 0xFF00) << 8) | ((x & 0xFF0000) >> 8) | ((x & 0xFF000000) >> 24);
}
 
void setMD4Registers(unsigned int AA, unsigned int BB, unsigned int CC, unsigned int DD){
	A=AA;
	B=BB;
	C=CC;
	D=DD;
}
 
void resetMD4Registers(void){
	setMD4Registers(0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476);
}

int main(int argc, char **argv)
{
	banner();
	char str[4096];
	
	if(argc>1 && !strcmp("--debug",argv[1]))
	{	
		DEBUG=1;
		printf("Debugging Enabled\n\n");
	}
	printf("Enter a string to encrypt.\nCtrl-C to exit \nDebugging : %s --debug\n",argv[0]);
	signal(SIGINT, handle_sigint); 
	while(1)
	{
		printf(">>\t");
    	scanf("%[^\n]s",str);
		printf("\n%s\n", MD4(str, strlen(str)));
		getchar();
	}
}
