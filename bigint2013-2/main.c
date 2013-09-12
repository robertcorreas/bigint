#include<stdlib.h>
#include<stdio.h>
#include "bigint.h"

int assertsPassaram = 0;
int assertsFalharam = 0;

char * testName;

void assert(unsigned int esperado, unsigned int obtido)
{
	if (esperado == obtido)
	{
		assertsPassaram++;
	}
	else
	{
		assertsFalharam++;
		printf("[%s]: esperado %02x, mas foi obtido %02x\n", testName, esperado, obtido);
	}
}

void assertBytes(void * esperado, void * obtido, int size)
{
	int i;
	int erro = 0;
	unsigned char * bytesObt = (unsigned char *) obtido;
	unsigned char * bytesEsp = (unsigned char *) esperado;

	for ( i = 0 ; i < size ; i++ )
	{
		erro = (*bytesEsp++ != *bytesObt++);
		if(erro) break;
	}

	if(erro)
	{
		bytesObt = (unsigned char *) obtido;
		bytesEsp = (unsigned char *) esperado;

		assertsFalharam++;
		
		printf("[%s]: erro bytes errados:\nesperado > obtido\n", testName);
		for ( i = 0 ; i < size ; i++ )
		{
			printf("%02x > %02x\n", *bytesEsp++, *bytesObt++);
		}
	} else {
		assertsPassaram++;
	}
}

void assertBytesBI(void * esperado, void * obtido)
{
	assertBytes(esperado, obtido, NUM_BYTES);
}


void test(char * name)
{
	testName = name;
}

void reportarTestes()
{
	printf("Passaram: %d    Falharam: %d\n", assertsPassaram, assertsFalharam);
}

char * bytesFrom(char b1, char b2, char b3, char b4,
	char b5, char b6, char b7, char b8,
	char b9, char b10, char b11, char b12,
	char b13, char b14, char b15, char b16)
{
	char * bytes = (char *) malloc(sizeof(char)*16);
	char * bytesBegin = bytes;

	*(bytes++) = b1; *(bytes++) = b2; *(bytes++) = b3; *(bytes++) = b4;
	*(bytes++) = b5; *(bytes++) = b6; *(bytes++) = b7; *(bytes++) = b8;
	*(bytes++) = b9; *(bytes++) = b10; *(bytes++) = b11; *(bytes++) = b12;
	*(bytes++) = b13; *(bytes++) = b14; *(bytes++) = b15; *(bytes++) = b16;

	return bytesBegin;
}

int main() {
	BigInt big, a, b;
	unsigned int uint;
	int vint;

	test("cast_uint ap�s big_uval tem que permanecer o mesmo valor");
	big_uval(big, 33);
	uint = big_cast_uint(big);
	assert(33, uint);

	test("cast_int ap�s big_uval tem que permanecer o mesmo valor");
	big_val(big, 33);
	vint = big_cast_int(big);
	assert(33, vint);

	test("cast_int ap�s big_val com negativo tem que permanecer o mesmo valor");
	big_val(big, -33);
	vint = big_cast_int(big);
	assert(-33, vint);

	test("big_uval => 1 como: 0x00(15x) 0x01");
	big_uval(big, 1);
	assertBytesBI(bytesFrom(
		0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);
	
	test("big_val => 1 como: 0x00(15x) 0x01");
	big_val(big, 1);
	assertBytesBI(bytesFrom(
		0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);
	
	test("big_val => -1 como: 0xff(16x)");
	big_val(big, -1);
	assertBytesBI(bytesFrom(
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff
	), big);

	
	test("1 + 1 = 0x02 0x00(15x)");
	big_val(a, 1);
	big_sum(big, a, a);
	assertBytesBI(bytesFrom(
		0x02, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	
	test("uval: 255 + 1 = 0x00 0x01 0x00(14x)");
	big_uval(a, 0xff);
	big_uval(b, 0x01);
	big_sum(big, a, b);
	assertBytesBI(bytesFrom(
		0x00, 0x01, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);
	
	test("uval: 0xff + 1 = 256");
	big_uval(a, 0xff);
	big_uval(b, 0x01);
	big_sum(big, a, b);
	vint = 256;
	assertBytes(&vint, big, 4);
	
	test("val: 0xffffffff + 1 = 0x00 (16x)");
	big_val(a, 0xffffffff);
	big_val(b, 0x01);
	big_sum(big, a, b);
	assertBytesBI(bytesFrom(
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("val: -1 + -1 = 0xfe 0xff (15x)");
	big_val(a, -1);
	big_val(b, -1);
	big_sum(big, a, b);
	assertBytesBI(bytesFrom(
		0xfe, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff
	), big);

	test("val: -1 + -1 = 0xfe 0xff (15x)");
	big_val(a, -1);
	big_val(b, -1);
	big_sum(big, a, b);
	assertBytesBI(bytesFrom(
		0xfe, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff
	), big);
	
	test("uval: 1 - 1 = 0x00 (16x)");
	big_uval(a, 1);
	big_uval(b, 1);
	big_sub(big, a, b);
	assertBytesBI(bytesFrom(
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	
	test("val: 1 - 1 = 0x00 (16x)");
	big_val(a, 1);
	big_val(b, 1);
	big_sub(big, a, b);
	assertBytesBI(bytesFrom(
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	
	test("uval: 256 - 1 = 0xff 0x00 (15x)");
	big_uval(a, 256);
	big_uval(b, 1);
	big_sub(big, a, b);
	assertBytesBI(bytesFrom(
		0xff, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	
	test("val: 1 - 1 = 0x00 (16x)");
	big_val(a, 1);
	big_val(b, 1);
	big_sub(big, a, b);
	assertBytesBI(bytesFrom(
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);
	
	test("0xa1b2c3 << 4 = 0x30 0x2c 0x1b 0x0a (12x)");
	big_uval(a, 0xa1b2c3);
	big_shl(big, a, 4);
	assertBytesBI(bytesFrom(
		0x30, 0x2c, 0x1b, 0x0a,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	
	test("0x80 << 1 = 0x00 0x01 0x00 (13x)");
	big_uval(a, 0x80);
	big_shl(big, a, 1);
	assertBytesBI(bytesFrom(
		0x00, 0x01, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("0x1 << 8 = 0x00 0x01 0x00 (15x)");
	big_uval(a, 0x1);
	big_shl(big, a, 8);
	assertBytesBI(bytesFrom(
		0x00, 0x01, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("0xa1b2c3 >> 4 = 0x2c 0x1b 0x0a 0x00 (12x)");
	big_uval(a, 0xa1b2c3);
	big_shr(big, a, 4);
	assertBytesBI(bytesFrom(
		0x2c, 0x1b, 0x0a, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("0x10 >> 1 = 0x08 (15x)");
	big_uval(a, 0x10);
	big_shr(big, a, 1);
	assertBytesBI(bytesFrom(
		0x08, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);
	
	reportarTestes();

	return 0;
}