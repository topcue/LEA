/*
 * @author topcue
 */

#include <stdio.h>
#include <stdlib.h>

#define Nr_128 24
#define Nr_192 28
#define Nr_256 32

#define flip(a) a = (a >> 24) | (a >> 8 & 0x0000ff00) | (a << 8 & 0x00ff0000) | (a << 24 & 0xff000000) 
#define inv_flip(a) a = (a << 24 & 0xff000000) | (a << 8 & 0xff0000) | (a >> 8 & 0xff00) | (a >> 24 & 0xff)
#define flip_4Arr(a, b, c, d) flip(a), flip(b), flip(c), flip(d)
#define flip_6Arr(a, b, c, d, e, f) flip(a), flip(b), flip(c), flip(d), flip(e), flip(f)
#define inv_flip_4Arr(a, b, c, d) inv_flip(a), inv_flip(b), inv_flip(c), inv_flip(d)

typedef unsigned int uint;

uint delta[8] = { 0xc3efe9db, 0x44626b02, 0x79e27c8a, 0x78df30ec,
				  0x715ea49e, 0xc785da0a, 0xe04ef22a, 0xe5c40957};

void show4Array(uint* a)
{
	int i = 0;
	for (i = 0; i < 4; i++)
		printf("%08x ", *(a + i));
	printf("\n");
}

void show6Array(uint* a)
{
	int i = 0;
	for (i = 0; i < 6; i++)
		printf("%08x ", *(a + i));
	printf("\n");
}

uint rot(uint a, int n)
{
	int i = 0;
	for (i = 0; i < n; i++)
		a = (a >> 31 & 0x1) | (a << 1 & 0xffffffff);
	return a;
}

void roundEncryption_128(uint* T, uint* X, uint* RK, int i)
{
	// Key_schedule
	T[0] = rot(T[0] + rot(delta[i % 4], i + 0), 1 );
	T[1] = rot(T[1] + rot(delta[i % 4], i + 1), 3 );
	T[2] = rot(T[2] + rot(delta[i % 4], i + 2), 6 );
	T[3] = rot(T[3] + rot(delta[i % 4], i + 3), 11);
	RK[0] = T[0]; RK[1] = T[1]; RK[2] = T[2]; RK[3] = T[1]; RK[4] = T[3]; RK[5] = T[1];

	// Round_encrypt
	uint Temp = X[0];
	X[0] = rot( (X[0] ^ RK[0]) + (X[1] ^ RK[1]), 9 );
	X[1] = rot( (X[1] ^ RK[2]) + (X[2] ^ RK[3]), 27);
	X[2] = rot( (X[2] ^ RK[4]) + (X[3] ^ RK[5]), 29);
	X[3] = Temp;
}

void roundEncryption_192(uint* T, uint* X, uint* RK, int i)
{
	// Key_schedule
	T[0] = rot(T[0] + rot(delta[i % 6], i + 0), 1 );
	T[1] = rot(T[1] + rot(delta[i % 6], i + 1), 3 );
	T[2] = rot(T[2] + rot(delta[i % 6], i + 2), 6 );
	T[3] = rot(T[3] + rot(delta[i % 6], i + 3), 11);
	T[4] = rot(T[4] + rot(delta[i % 6], i + 4), 13);
	T[5] = rot(T[5] + rot(delta[i % 6], i + 5), 17);
	RK[0] = T[0]; RK[1] = T[1]; RK[2] = T[2]; RK[3] = T[3]; RK[4] = T[4]; RK[5] = T[5];

	// Round_encrypt
	uint Temp = X[0];
	X[0] = rot( (X[0] ^ RK[0]) + (X[1] ^ RK[1]), 9 );
	X[1] = rot( (X[1] ^ RK[2]) + (X[2] ^ RK[3]), 27);
	X[2] = rot( (X[2] ^ RK[4]) + (X[3] ^ RK[5]), 29);
	X[3] = Temp;
}

void roundEncryption_256(uint* T, uint* X, uint* RK, int i)
{
	// Key_schedule
	T[(6 * i + 0) % 8] = rot(T[(6 * i + 0) % 8] + rot(delta[i % 8], i + 0), 1 );
	T[(6 * i + 1) % 8] = rot(T[(6 * i + 1) % 8] + rot(delta[i % 8], i + 1), 3 );
	T[(6 * i + 2) %  8] = rot(T[(6 * i + 2) % 8] + rot(delta[i % 8], i + 2), 6 );
	T[(6 * i + 3) % 8] = rot(T[(6 * i + 3) % 8] + rot(delta[i % 8], i + 3), 11);
	T[(6 * i + 4) % 8] = rot(T[(6 * i + 4) % 8] + rot(delta[i % 8], i + 4), 13);
	T[(6 * i + 5) % 8] = rot(T[(6 * i + 5) % 8] + rot(delta[i % 8], i + 5), 17);
	RK[0] = T[(6 * i + 0) % 8]; RK[1] = T[(6 * i + 1) % 8]; RK[2] = T[(6 * i + 2) % 8];
	RK[3] = T[(6 * i + 3) % 8]; RK[4] = T[(6 * i + 4) % 8]; RK[5] = T[(6 * i + 5) % 8];
	
	// Round_encrypt
	uint Temp = X[0];
	X[0] = rot( (X[0] ^ RK[0]) + (X[1] ^ RK[1]), 9 );
	X[1] = rot( (X[1] ^ RK[2]) + (X[2] ^ RK[3]), 27);
	X[2] = rot( (X[2] ^ RK[4]) + (X[3] ^ RK[5]), 29);
	X[3] = Temp;
}

void LEA_128()
{
	uint* P = NULL;		P = (uint*)calloc(4, sizeof(uint));
	uint* X = NULL;		X = (uint*)calloc(4, sizeof(uint));
	uint* K = NULL;		K = (uint*)calloc(4, sizeof(uint));
	uint* T = NULL;		T = (uint*)calloc(4, sizeof(uint));
	uint* RK = NULL;	RK = (uint*)calloc(6, sizeof(uint));

	// KISA LEA-128 Test Vector
	K[0] = 0x0f1e2d3c;	K[1] = 0x4b5a6978;	K[2] = 0x8796a5b4;	K[3] = 0xc3d2e1f0;
	P[0] = 0x10111213;	P[1] = 0x14151617;	P[2] = 0x18191a1b;	P[3] = 0x1c1d1e1f;

	printf("P : ");	show4Array(P);
	printf("K : ");	show4Array(K);

	T[0] = K[0]; T[1] = K[1]; T[2] = K[2]; T[3] = K[3]; T[4] = 0x00; T[5] = 0x00;
	flip_4Arr(T[0], T[1], T[2], T[3]);

	X[0] = P[0]; X[1] = P[1]; X[2] = P[2]; X[3] = P[3]; X[4] = 0x00; X[5] = 0x00;
	flip_4Arr(X[0], X[1], X[2], X[3]);

	// Encryption
	int i = 0;
	for (i = 0; i < Nr_128; i++)
		roundEncryption_128(T, X, RK, i);

	inv_flip_4Arr(X[0], X[1], X[2], X[3]);
	printf("C : ");	show4Array(X);

	free(P); free(K); free(T); free(X); free(RK);

	return;
}


void LEA_192()
{
	uint* P = NULL;		P = (uint*)calloc(4, sizeof(uint));
	uint* X = NULL;		X = (uint*)calloc(4, sizeof(uint));
	uint* K = NULL;		K = (uint*)calloc(6, sizeof(uint));
	uint* T = NULL;		T = (uint*)calloc(6, sizeof(uint));
	uint* RK = NULL;	RK = (uint*)calloc(6, sizeof(uint));

	// KISA LEA-196 Test Vector
	K[0] = 0x0f1e2d3c;	K[1] = 0x4b5a6978;	K[2] = 0x8796a5b4;
	K[3] = 0xc3d2e1f0;	K[4] = 0xf0e1d2c3;	K[5] = 0xb4a59687;
	P[0] = 0x20212223;	P[1] = 0x24252627;	P[2] = 0x28292a2b;	P[3] = 0x2c2d2e2f;

	printf("P : ");	show4Array(P);
	printf("K : ");	show6Array(K);

	T[0] = K[0]; T[1] = K[1]; T[2] = K[2]; T[3] = K[3]; T[4] = K[4]; T[5] = K[5];
	flip_6Arr(T[0], T[1], T[2], T[3], T[4], T[5]);

	X[0] = P[0]; X[1] = P[1]; X[2] = P[2]; X[3] = P[3];
	flip_4Arr(X[0], X[1], X[2], X[3]);

	// Encryption
	int i = 0;
	for (i = 0; i < Nr_192; i++)
		roundEncryption_192(T, X, RK, i);

	inv_flip_4Arr(X[0], X[1], X[2], X[3]);
	printf("C : ");	show4Array(X);

	free(P); free(K); free(T); free(X); free(RK);

	return;
}

void LEA_256()
{
	uint* P = NULL;		P = (uint*)calloc(4, sizeof(uint));
	uint* X = NULL;		X = (uint*)calloc(4, sizeof(uint));
	uint* K = NULL;		K = (uint*)calloc(8, sizeof(uint));
	uint* T = NULL;		T = (uint*)calloc(8, sizeof(uint));
	uint* RK = NULL;	RK = (uint*)calloc(6, sizeof(uint));

	// KISA LEA-256 Test Vector
	K[0] = 0x0f1e2d3c;	K[1] = 0x4b5a6978;	K[2] = 0x8796a5b4;	K[3] = 0xc3d2e1f0;
	K[4] = 0xf0e1d2c3;	K[5] = 0xb4a59687;	K[6] = 0x78695a4b;	K[7] = 0x3c2d1e0f;
	P[0] = 0x30313233;	P[1] = 0x34353637;	P[2] = 0x38393a3b;	P[3] = 0x3c3d3e3f;

	printf("P : ");	show4Array(P);
	printf("K : ");	show4Array(K); printf("    "); show4Array((K+4));

	T[0] = K[0]; T[1] = K[1]; T[2] = K[2]; T[3] = K[3]; T[4] = K[4]; T[5] = K[5];
	flip_4Arr(T[0], T[1], T[2], T[3]);
	flip_4Arr(T[4], T[5], T[6], T[7]);

	X[0] = P[0]; X[1] = P[1]; X[2] = P[2]; X[3] = P[3];
	flip_4Arr(X[0], X[1], X[2], X[3]);

	// Encryption
	int i = 0;
	for (i = 0; i < 2; i++)
		roundEncryption_256(T, X, RK, i);

	inv_flip_4Arr(X[0], X[1], X[2], X[3]);
	printf("C : ");	show4Array(X);

	free(P); free(K); free(T); free(X); free(RK);

	return;
}

int main()
{
	int mode = -1;
	printf("\n Select mode\n [1] 128-bit\n [2] 192-bit\n [3] 256-bit\n [4] exit\n");
	scanf("%d", &mode);
	switch(mode){
		case 1: printf("\n [LEA_128]\n"); LEA_128(); break;
		case 2: printf("\n [LEA_192]\n"); LEA_192(); break;
		case 3: printf("\n [LEA_256]\n"); LEA_256(); break;
		case 4: mode = -1; break;
	}
	return 0;
}


// EOF

