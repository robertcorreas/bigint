#include <stdlib.h>
#include <stdio.h>
#include "bigint.h"

/* Hugo Roque de Figueiredo 1311162 */
/* Robert Correa dos Santos 1210325 */

int assertsPassaram = 0;
int assertsFalharam = 0;

char * testName;



int big_cast_int(BigInt res);

unsigned int big_cast_uint(BigInt res)
{
	return *((unsigned int *) res);
}

int big_cast_int(BigInt res)
{
	return *((int *) res);
}

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

void reportarTestes(void)
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


void test_big_cmp_igualdade(void)
{
	int i, valor_inteiro;
	BigInt a, b;
	for(i = -2000; i < 2000; i++)
	{
		big_val(a, i);
		big_val(b, i);

		valor_inteiro = big_cmp(a,b);

		assertBytes(bytesFrom(
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
		), &valor_inteiro,4);
	}
}

void test_big_cmp_menor_que(void)
{
	int i, j, valor_inteiro;
	BigInt a, b;

	for(i = -2000, j= -1999; i < 2000; i++, j++)
	{
		big_val(a, i);
		big_val(b, j);

		valor_inteiro = big_cmp(a,b);

		assertBytes(bytesFrom(
			0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF
			), &valor_inteiro,4);
	}
}

void test_big_cmp_maior_que(void)
{
	int i, j, valor_inteiro;
	BigInt a, b;

	for(i = -1999, j= -2000; i < 2000; i++, j++)
	{
		big_val(a, i);
		big_val(b, j);

		valor_inteiro = big_cmp(a,b);

		assertBytes(bytesFrom(
			0x01, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00
			), &valor_inteiro,4);
	}
}

void test_big_cmp(void)
{
	
	test("bit_cmp a == b");
	test_big_cmp_igualdade();

	test("bit_cmp a < b");
	test_big_cmp_menor_que();

	test("bit_cmp a > b");
	test_big_cmp_maior_que();
}

void test_big_ucmp_igualdade(void)
{
	int i, valor_inteiro;
	BigInt a, b;
	for(i = 0; i < 2000; i++)
	{
		big_uval(a, i);
		big_uval(b, i);

		valor_inteiro = big_ucmp(a,b);

		assertBytes(bytesFrom(
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
		), &valor_inteiro,4);
	}
}

void test_big_ucmp_menor_que(void)
{
	int i, j, valor_inteiro;
	BigInt a, b;

	for(i = 0, j= 1; i < 2000; i++, j++)
	{
		big_uval(a, i);
		big_uval(b, j);

		valor_inteiro = big_ucmp(a,b);

		assertBytes(bytesFrom(
			0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF
			), &valor_inteiro,4);
	}
}

void test_big_ucmp_maior_que(void)
{
	int i, j, valor_inteiro;
	BigInt a, b;

	for(i = 1, j= 0; i < 2000; i++, j++)
	{
		big_uval(a, i);
		big_uval(b, j);

		valor_inteiro = big_ucmp(a,b);

		assertBytes(bytesFrom(
			0x01, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00
			), &valor_inteiro,4);
	}
}

void test_big_ucmp(void)
{
	test("bit_ucmp a == b");
	test_big_ucmp_igualdade();

	test("bit_ucmp a < b");
	test_big_ucmp_menor_que();

	test("bit_ucmp a > b");
	test_big_ucmp_maior_que();
}

int main(void) {
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
	
	test("0x1 << 120 = 0x00(15x) 0x01");
	big_uval(a, 0x1);
	big_shl(big, a, 120);
	assertBytesBI(bytesFrom(
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x01
	), big);
	
	
	test("-1 << 9 = 0x00 0xfe 0xff(14x)");
	big_val(a, -1);
	big_shl(big, a, 9);
	assertBytesBI(bytesFrom(
		0x00, 0xfe, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff
	), big);


	test("0xa1b2c3 >> 4 = 0x2c 0x1b 0x0a 0x00 (12x)");
	big_val(big, -1);
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

	test("-1 >> 127 = 0x01 0x00(15x)");
	big_val(a, -1);
	big_shr(big, a, 127);
	assertBytesBI(bytesFrom(
		0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);
	
	test("-1 >> 120 = 0xff 0x00(15x)");
	big_val(a, -1);
	big_shr(big, a, 120);
	assertBytesBI(bytesFrom(
		0xff, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("0x10 >> 8 = 0x01 0x00(15x)");
	big_uval(a, 0x100);
	big_shr(big, a, 8);
	assertBytesBI(bytesFrom(
		0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("0x10 >> 8 = 0x01 0x00(15x)");
	big_uval(a, 0x100);
	big_shr(big, a, 8);
	assertBytesBI(bytesFrom(
		0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);
	
	
	test("-1 >> 9 = 0x00 0xfe 0xff(14x)");
	big_val(a, -1);
	big_shr(big, a, 9);
	assertBytesBI(bytesFrom(
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0x7f, 0x00
	), big);


	//-----------------------------------------------------------------------------------

	test_big_cmp();

	test_big_ucmp();

	test("2*2 = 4");
	big_uval(a, 2);
	big_uval(b, 2);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0x04, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00

	), big);

	test("1*1 = 1");
	big_uval(a, 1);
	big_uval(b, 1);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("0*0 = 0");
	big_uval(a, 0);
	big_uval(b, 0);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("1*2 = 2");
	big_uval(a, 1);
	big_uval(b, 2);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0x02, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("3*3 = 9");
	big_uval(a, 3);
	big_uval(b, 3);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0x09, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("13*13 = 169");
	big_uval(a, 13);
	big_uval(b, 13);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0xA9, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("10000*10000 = 100000000");
	big_uval(a, 10000);
	big_uval(b, 10000);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0x00, 0xE1, 0xF5, 0x05,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("100000*100000 = 10000000000");
	big_uval(a, 100000);
	big_uval(b, 100000);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0x00, 0xE4, 0x0B, 0x54,
		0x02, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);


	test("99*99 = 9801");
	big_uval(a, 99);
	big_uval(b, 99);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0x49, 0x26, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);


	test("99999*99999 = 9999800001");
	big_uval(a, 99999);
	big_uval(b, 99999);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0xC1, 0xD6, 0x08, 0x54,
		0x02, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("99999*0 = 0");
	big_uval(a, 99999);
	big_uval(b, 0);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	test("99999*1 = 99999");
	big_uval(a, 99999);
	big_uval(b, 1);
	big_umul(big, a, b);
	assertBytesBI(bytesFrom(
		0x9F, 0x86, 0x01, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);


	// mult com sinal
	//--------------------------------------------------------------------------------------

	test("-1*2 = -2");
	big_val(a, -1);
	big_val(b, 2);
	big_val(big, 0);
	big_mul(big, a, b);
	assertBytesBI(bytesFrom(
		0xFE, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF
	), big);


	test("2*-1 = -2");
	big_val(a, 2);
	big_val(b, -1);
	big_val(big, 0);
	big_mul(big, a, b);
	assertBytesBI(bytesFrom(
		0xFE, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF
	), big);

	test("5*-2 = -10");
	big_val(a, 5);
	big_val(b, -2);
	big_val(big, 0);
	big_mul(big, a, b);
	assertBytesBI(bytesFrom(
		0xF6, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF
	), big);

	test("-2*-2 = 4");
	big_val(a, -2);
	big_val(b, -2);
	big_val(big, 0);
	big_mul(big, a, b);
	assertBytesBI(bytesFrom(
		0x04, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	), big);

	reportarTestes();

	return 0;
}