#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define INITIAL_PERMUTATION_TABLE 	0
#define FINAL_PERMUTATION_TABLE 	1
#define STRAIGHT_PERMUTATION_TABLE 	2
#define EXPANSION_PERMUTATION_TABLE 3

void permute(int inBlockLen, int outBlockLen, char *inBlock, char *outBlock, int permutationType)
{
	static int permutation_table[4][64] =   {
												{	58, 50, 42, 34, 26, 18, 10, 2, 
													60, 52, 44, 36, 28, 20, 12, 4, 
													62, 54, 46, 38, 30, 22, 14, 6, 
													64, 56, 48, 40, 32, 24, 16, 8, 
													57, 49, 41, 33, 25, 17,  9, 1, 
													59, 51, 43, 35, 27, 19, 11, 3, 
													61, 53, 45, 37, 29, 21, 13, 5, 
													63, 55, 47, 39, 31, 23, 15, 7 	},

												{ 	40, 8, 48, 16, 56, 24, 64, 32, 
													39, 7, 47, 15, 55, 23, 63, 31, 
													38, 6, 46, 14, 54, 22, 62, 30, 
													37, 5, 45, 13, 53, 21, 61, 29, 
													36, 4, 44, 12, 52, 20, 60, 28, 
													35, 3, 43, 11, 51, 19, 59, 27, 
													34, 2, 42, 10, 50, 18, 58, 26, 
													33, 1, 41,  9, 49, 17, 57, 25 	},
												
												{	16,  7, 20, 21, 29, 12, 28, 17, 
					                     			 1, 15, 23, 26,  5, 18, 31, 10, 
					                    			 2,  8, 24, 14, 32, 27,  3,  9, 
					                     			19, 13, 30,  6, 22, 11,  4, 25,
					                     			 0,  0,  0,  0,  0,  0,  0,  0,
					                     			 0,  0,  0,  0,  0,  0,  0,  0,
					                     			 0,  0,  0,  0,  0,  0,  0,  0,
					                     			 0,  0,  0,  0,  0,  0,  0,  0	},

					                     		{ 	32, 1,   2,  3,  4,  5,  4,  5, 
													6,  7,   8,  9,  8,  9, 10, 11, 
													12, 13, 12, 13, 14, 15, 16, 17, 
													16, 17, 18, 19, 20, 21, 20, 21, 
													22, 23, 24, 25, 24, 25, 26, 27, 
													28, 29, 28, 29, 30, 31, 32,  1,
													 0,  0,  0,  0,  0,  0,  0,  0,
					                     			 0,  0,  0,  0,  0,  0,  0,  0	}
											}; 

	for (int i=0;i<outBlockLen;++i)
		outBlock[i] = inBlock[permutation_table[permutationType][i]-1];		
}

void exOR(int len, char *inBlock, char *key, char *outBlock)
{
	for(int i=0;i<len;++i)
		outBlock[i] = ( ((inBlock[i]-'0')&1) ^ ((key[i]-'0')&1) )+'0';
}

void substitute(char inBlock[48], char outBlock[32])
{
	static int substitution_table[8][4][16] = { 
												{ 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 
						                          0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 
						                          4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 
						                          15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },

						                        { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 
						                          3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 
						                          0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 
						                          13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 }, 
						  
						                        { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 
						                          13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 
						                          13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 
						                          1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },

						                        { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 
						                          13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 
						                          10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 
						                          3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }, 

						                        { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 
						                          14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 
						                          4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 
						                          11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }, 

						                        { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 
						                          10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 
						                          9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 
						                          4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }, 

						                        { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 
						                          13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 
						                          1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 
						                          6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }, 

						                        { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 
						                          1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 
						                          7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 
						                          2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
		                      				}; 
    
    int row,col,value;

	for(int i=0;i<8;i++)
	{
		row = 2*(inBlock[i*6]-'0') + (inBlock[i*6+5]-'0');
		col = 8*(inBlock[i*6+1]-'0')+ 4*(inBlock[i*6+2]-'0') + 2*(inBlock[i*6+3]-'0') + (inBlock[i*6+4]-'0');
		
		value = substitution_table[i][row][col];

		outBlock[i*4] = value/8 + '0';
		value = value%8;
		outBlock[i*4+1] = value/4 + '0';
		value = value%4;
		outBlock[i*4+2] = value/2 + '0';
		value = value%2;
		outBlock[i*4+3] = value + '0';
	}	                      				

}

void copy(int len, char *src, char *dest)
{
	for(int i=0; i<len; ++i)
		dest[i]=src[i];
}

void DESFunction(char inBlock[32], char roundKey[48], char outBlock[32])
{
	char T1[48], T2[48], T3[32];

	permute(32,48,inBlock,T1,EXPANSION_PERMUTATION_TABLE);
	exOR(48,T1,roundKey,T2);
	substitute(T2,T3);
	permute(32,32,T3,outBlock,STRAIGHT_PERMUTATION_TABLE);
}

void mixer(char leftBlock[32], char rightBlock[32], char roundKey[48])
{

	char T1[32], T2[32], T3[32];

	copy(32,rightBlock,T1);
	DESFunction(T1,roundKey,T2);
	exOR(32,leftBlock,T2,T3);
	copy(32,T3,leftBlock);
}

void swapper(char leftBlock[32], char rightBlock[32])
{
	char T[32]={'\0'};

	copy(32,leftBlock,T);
	copy(32,rightBlock,leftBlock);
	copy(32,T,rightBlock);
}
void combine(int len1 ,int len2, char *leftBlock, char *rightBlock, char *outBlock)
{
	for(int i=0;i<len1;++i)
	{
		outBlock[i] = leftBlock[i];
		outBlock[len1+i] = rightBlock[i];
	}
}

void split(int len1 ,int len2, char *inBlock, char *leftBlock, char *rightBlock)
{
	for(int i=0;i<len2;++i)
	{
		leftBlock[i] = inBlock[i];
		rightBlock[i] = inBlock[len2 + i];
	}
}

void cipher(char plainBlock[64], char roundKeys[16][48], char cipherBlock[64])
{
	char inBlock[64]	= {'\0'};
	char outBlock[64]	= {'\0'};
	char leftBlock[32]	= {'\0'};
	char rightBlock[32] = {'\0'};

	permute(64,64,plainBlock, inBlock, INITIAL_PERMUTATION_TABLE);
	split(64,32,inBlock,leftBlock,rightBlock);
	
	for(int round = 1;round<=16; ++round)
	{
		mixer(leftBlock,rightBlock,roundKeys[round-1]);
		if(round!=16)
			swapper(leftBlock,rightBlock);
	}
	
	combine(32,64,leftBlock,rightBlock,outBlock);
	permute(64,64,outBlock,cipherBlock,FINAL_PERMUTATION_TABLE);
}

void decipher(char cipherBlock[64], char roundKeys[16][48], char plainBlock[64])
{
	char inBlock[64]	= {'\0'};
	char outBlock[64]	= {'\0'};
	char leftBlock[32]	= {'\0'};
	char rightBlock[32] = {'\0'};

	permute(64,64,cipherBlock, inBlock, INITIAL_PERMUTATION_TABLE);
	split(64,32,inBlock,leftBlock,rightBlock);
	for(int round = 1;round<=16; ++round)
	{
		mixer(leftBlock,rightBlock,roundKeys[16-round]);
		if(round!=16)
			swapper(leftBlock,rightBlock);
	}

	combine(32,64,leftBlock,rightBlock,outBlock);
	permute(64,64,outBlock,plainBlock,FINAL_PERMUTATION_TABLE);
}

int GCD(int a0, int b0)
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

void shift_left(char* key, int size, int shift)
{
    int gcd = GCD(shift, size); 
    int j,i,k;
    char temp;
    for (i=0; i<gcd; i++)
    { 
        temp = key[i]; 
        j = i; 
  
        while (1) 
        { 
            k = j + shift; 
            if (k >= size) 
                k = k - size; 
  
            if (k == i) 
                break; 
  
            key[j] = key[k]; 
            j = k; 
        } 
        key[j] = temp; 
    } 
}

void round_key_gen(char *key, char roundkey[16][48])
{
  static int parity_drop_table[56] = { 57, 49, 41, 33, 25, 17, 9, 
                     1, 58, 50, 42, 34, 26, 18, 
                     10, 2, 59, 51, 43, 35, 27, 
                     19, 11, 3, 60, 52, 44, 36, 
                     63, 55, 47, 39, 31, 23, 15, 
                     7, 62, 54, 46, 38, 30, 22, 
                     14, 6, 61, 53, 45, 37, 29, 
                     21, 13, 5, 28, 20, 12, 4 };

  static int shift_table[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };   

  static int compression_pbox[48] = { 14, 17, 11, 24, 1, 5, 
                         3, 28, 15, 6, 21, 10, 
                         23, 19, 12, 4, 26, 8, 
                         16, 7, 27, 20, 13, 2, 
                         41, 52, 31, 37, 47, 55, 
                         30, 40, 51, 45, 33, 48, 
                         44, 49, 39, 56, 34, 53, 
                         46, 42, 50, 36, 29, 32 };                 

  char cipher_key[56];
  int i,j;
  
  // permutation
  for(i=0;i<56;i++)
    cipher_key[i]=key[parity_drop_table[i]-1];

  for(i=0;i<16;i++)
  {
      shift_left(cipher_key,28,shift_table[i]);
      shift_left(cipher_key+28,28,shift_table[i]);

      for(j=0;j<48;j++)
        roundkey[i][j]=cipher_key[compression_pbox[j]-1];

  }  
}

void random_keygen(char *key, int len)
{
    srand(time(NULL));
    for(int i=0;i<len;i++)
        key[i]=rand()%2+'0';
}

void hex2bin(char *hex,char *bin)
{
	for(int i=0;i<16;++i)
		switch(hex[i])
		{
			case '0' :  copy(4,"0000",bin+i*4); break;
			case '1' :  copy(4,"0001",bin+i*4); break;
			case '2' :  copy(4,"0010",bin+i*4); break;
			case '3' :  copy(4,"0011",bin+i*4); break;			
			case '4' :  copy(4,"0100",bin+i*4); break;
			case '5' :  copy(4,"0101",bin+i*4); break;
			case '6' :  copy(4,"0110",bin+i*4); break;
			case '7' :  copy(4,"0111",bin+i*4); break;
			case '8' :  copy(4,"1000",bin+i*4); break;
			case '9' :  copy(4,"1001",bin+i*4); break;
			case 'A' :  copy(4,"1010",bin+i*4); break;
			case 'B' :  copy(4,"1011",bin+i*4); break;
			case 'C' :  copy(4,"1100",bin+i*4); break;
			case 'D' :  copy(4,"1101",bin+i*4); break;
			case 'E' :  copy(4,"1110",bin+i*4); break;
			case 'F' :  copy(4,"1111",bin+i*4); break;
		}	
}

void bin2hex(char *bin,char *hex)
{
	int value;
	for(int i=0;i<64;i=i+4)
	{
		value = 8*(bin[i]-'0') + 4*(bin[i+1]-'0') + 2*(bin[i+2]-'0') + (bin[i+3]-'0');

		if(value<10)
			hex[i/4]=value+'0';
		else
			hex[i/4]=value-10+'A';	
	}	
}

int main()
{
	char plaintext[10000]  = {'\0'};
	char ciphertext[10000] = {'\0'};
	char roundkey[16][48] = {'\0'};
	char key[65] = {'\0'};

	char plaintext_[10000] = {'\0'};
	
	int  input_len, i;

	char inBlockHEX[13]  = {'\0'};
	char outBlockHEX[13] = {'\0'};
	char inBlockBIN[49]  = {'\0'};
	char outBlockBIN[49] = {'\0'};

	random_keygen(key,64);
	
	round_key_gen(key,roundkey);
	
	printf("Enter the plaintext (in hex):\t");
	scanf("%s",plaintext);

	printf("\nkey = Ob%s\n",key);

	input_len = strlen(plaintext);

	if(input_len%16)
	{
		for(i=input_len;i<(((input_len/16)+1)*16);++i)
			plaintext[i]='0';
		plaintext[i]='\0';	
		printf("\n< plaintext is padded with 0 >\n");
		printf("\n\tPlainText :\tOx%s\n",plaintext);

		input_len = ((input_len/16)+1)*16;
	}


//ENCRYPTION 
	printf("\nENCRYPTION\n");	
	for(i=0;i<input_len;i=i+16)
	{
		copy(16,plaintext+i, inBlockHEX);
		hex2bin(inBlockHEX,inBlockBIN);
		cipher(inBlockBIN,roundkey,outBlockBIN);
		bin2hex(outBlockBIN,outBlockHEX);
		copy(16,outBlockHEX,ciphertext+i);
	}

	printf("\n\tCipherText :\tOx%s\n",ciphertext);

//DECRYPTION
	
	printf("\nDECRYPTION\n");
	//printf("PT:\t%s\n",plaintext_);
	for(i=0;i<input_len;i=i+16)
	{
		copy(16,ciphertext+i, inBlockHEX);
		hex2bin(inBlockHEX,inBlockBIN);
		decipher(inBlockBIN,roundkey,outBlockBIN);
		bin2hex(outBlockBIN,outBlockHEX);
		copy(16,outBlockHEX,plaintext_ +i);
	}

	printf("\n\tPlainText':\tOx%s\n",plaintext_);

return 0;
}