/**
 * This source code is licensed under the
 * Creative Commons Zero v1.0 Universal License
 * (CC0 1.0 Universal / CC0 1.0)
 * https://creativecommons.org/publicdomain/zero/1.0/legalcode
 *
 * Copyright 2023, Sheep Sun <sunxiaoyang2003@gmail.com>
 *
*/
#include <stdio.h>
#include <string.h>
#include "CH57x_common.h"
#include "main.h"

#define BIT(x)	(1 << (x))

//CH579 AES base address
#define AES_BASE						(0x4000C300)

//AES register definition
#define R32_AES_CTRL_CCMMOD				(*((PUINT32V)(AES_BASE + 0x00)))
#define R32_AES_CCMINT_EN				(*((PUINT32V)(AES_BASE + 0x04)))
#define R32_AES_CCMVT_INIT0				(*((PUINT32V)(AES_BASE + 0x08)))
#define R32_AES_CCMVT_INIT1				(*((PUINT32V)(AES_BASE + 0x0C)))
#define R32_AES_PKT_CNT0				(*((PUINT32V)(AES_BASE + 0x10)))
#define R32_AES_PKT_CNT1				(*((PUINT32V)(AES_BASE + 0x14)))
#define R32_AES_DATA0					(*((PUINT32V)(AES_BASE + 0x18)))
#define R32_AES_DATA1					(*((PUINT32V)(AES_BASE + 0x1C)))
#define R32_AES_DATA2					(*((PUINT32V)(AES_BASE + 0x20)))
#define R32_AES_DATA3					(*((PUINT32V)(AES_BASE + 0x24)))
#define R32_AES_KEY0					(*((PUINT32V)(AES_BASE + 0x28)))
#define R32_AES_KEY1					(*((PUINT32V)(AES_BASE + 0x2C)))
#define R32_AES_KEY2					(*((PUINT32V)(AES_BASE + 0x30)))
#define R32_AES_KEY3					(*((PUINT32V)(AES_BASE + 0x34)))
#define R32_AES_KEY4					(*((PUINT32V)(AES_BASE + 0x38)))
#define R32_AES_KEY5					(*((PUINT32V)(AES_BASE + 0x3C)))
#define R32_AES_KEY6					(*((PUINT32V)(AES_BASE + 0x40)))
#define R32_AES_KEY7					(*((PUINT32V)(AES_BASE + 0x44)))
#define R32_AES_RAND0					(*((PUINT32V)(AES_BASE + 0x48)))
#define R32_AES_RAND1					(*((PUINT32V)(AES_BASE + 0x4C)))
#define R32_AES_RAND2					(*((PUINT32V)(AES_BASE + 0x50)))
#define R32_AES_RAND3					(*((PUINT32V)(AES_BASE + 0x54)))

//R32_AES_CTRL_CCMMOD bit definition
#define	CTRL_START			BIT(0)
#define	CTRL_DECRYPT		BIT(1)
#define	CTRL_KEYLEN			BIT(4)

//R32_AES_CCMINT_EN bit definition
#define	INT_EN					BIT(0)
#define	INT_STATUS			BIT(1)

//AES encryption types
enum AES_TYPE
{
	CRYPT_AES_128,
	CRYPT_AES_256
};

//operations
enum AES_OP
{
	AES_ENCRYPT,
	AES_DECRYPT
};

void aes_set_key(const UINT8 *key, enum AES_TYPE crypt_type)
{
	UINT32 *keys = (UINT32 *)key;

	R32_AES_KEY0 = keys[0];
	R32_AES_KEY1 = keys[1];
	R32_AES_KEY2 = keys[2];
	R32_AES_KEY3 = keys[3];
	if(crypt_type == CRYPT_AES_256)
	{
		R32_AES_KEY4 = keys[4];
		R32_AES_KEY5 = keys[5];
		R32_AES_KEY6 = keys[6];
		R32_AES_KEY7 = keys[7];
	}
}

void aes_ecb_crypt(const UINT8 *in, UINT8 *out, enum AES_TYPE crypt_type, enum AES_OP operation)
{
	UINT32 *ins = (UINT32 *)in;
	UINT32 *outs = (UINT32 *)out;
	
	R32_AES_DATA0 = ins[0];
	R32_AES_DATA1 = ins[1];
	R32_AES_DATA2 = ins[2];
	R32_AES_DATA3 = ins[3];

	R32_AES_CTRL_CCMMOD = 0;
	
	if(operation == AES_DECRYPT)
	{
		R32_AES_CTRL_CCMMOD |= CTRL_DECRYPT;
	}
	if(crypt_type == CRYPT_AES_256)
	{
		R32_AES_CTRL_CCMMOD |= CTRL_KEYLEN;
	}
	
	R32_AES_CCMINT_EN &= ~INT_STATUS;
	R32_AES_CCMINT_EN |= INT_EN;
	
	R32_AES_CTRL_CCMMOD |= CTRL_START;
	
	while(!(R32_AES_CCMINT_EN & INT_STATUS));
	
	outs[0] = R32_AES_DATA0;
	outs[1] = R32_AES_DATA1;
	outs[2] = R32_AES_DATA2;
	outs[3] = R32_AES_DATA3;
}

void platform_init(void)
{
	//SystemClock_Init
	PWR_UnitModCfg(ENABLE, UNIT_SYS_PLL);
	DelayMs(3); 
	SetSysClock(CLK_SOURCE_PLL_40MHz);

	//DebugUart1_Init
	GPIOA_SetBits(GPIO_Pin_9);
	GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
	GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
	UART1_DefInit();
}

int main(void)
{
	const UINT8 aes128_plain[16] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
	const UINT8 aes128_cipher[16] = {0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97};
	const UINT8 aes128_key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	const UINT8 aes256_plain[16] = {0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51};
	const UINT8 aes256_cipher[16] = {0x59, 0x1c, 0xcb, 0x10, 0xd4, 0x10, 0xed, 0x26, 0xdc, 0x5b, 0xa7, 0x4a, 0x31, 0x36, 0x28, 0x70};
	const UINT8 aes256_key[32] = {0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
																0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4};
	UINT8 cipher_buf[16] = {0};

	platform_init();

	printf("AES-128 test...\r\n");
	aes_set_key(aes128_key, CRYPT_AES_128);
	aes_ecb_crypt(aes128_plain, cipher_buf, CRYPT_AES_128, AES_ENCRYPT);
	if(memcmp(cipher_buf, aes128_cipher, 16))
	{
		printf("AES-128 encrypt test fail\r\n");
	}
	else
	{
		printf("AES-128 encrypt test success\r\n");
	}
	aes_ecb_crypt(cipher_buf, cipher_buf, CRYPT_AES_128, AES_DECRYPT);
	if(memcmp(cipher_buf, aes128_plain, 16))
	{
		printf("AES-128 decrypt test fail\r\n");
	}
	else
	{
		printf("AES-128 decrypt test success\r\n");
	}

	printf("AES-256 test...\r\n");
	aes_set_key(aes256_key, CRYPT_AES_256);
	aes_ecb_crypt(aes256_plain, cipher_buf, CRYPT_AES_256, AES_ENCRYPT);
	if(memcmp(cipher_buf, aes256_cipher, 16))
	{
		printf("AES-256 encrypt test fail\r\n");
	}
	else
	{
		printf("AES-256 encrypt test success\r\n");
	}
	aes_ecb_crypt(cipher_buf, cipher_buf, CRYPT_AES_256, AES_DECRYPT);
	if(memcmp(cipher_buf, aes256_plain, 16))
	{
		printf("AES-256 decrypt test fail\r\n");
	}
	else
	{
		printf("AES-256 decrypt test success\r\n");
	}
	
	while(1);
}
