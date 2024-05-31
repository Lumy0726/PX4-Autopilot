#include "mc.h"


#define PRIVATE 0
#define PUBLIC 1

#define MAX_AES_KEY_IDX 32
#define MAX_AES_KEY_SIZE 16

int publicKey[2] = { 14351, 11 };
int privateKey[2] = { 14351, 1283 };

byte AES_key[MAX_AES_KEY_IDX][16] = {};

static void dump(byte* buf, int len) {
	int i;
	for (i = 0; i < len; i++) {
		printf("%d", (int)buf[i]);
		printf(" ");
	}
	printf("\n");
}

int Init_SE() {
	return 1;
}

int Generate_AES128Key(int keyNum) {
	for (int i = 0; i < MAX_AES_KEY_SIZE; i++) {
		AES_key[keyNum][i] = rand() % 255;
	}
	return 1;
}

int Encrypt_AES128(int keyNum, uint8_t* plain_data, int plain_len, uint8_t* enc_data, int* enc_len) {
	AES aes;

	plain_data[plain_len++] = 0x00;
	aes.do_aes_encrypt(plain_data, plain_len, enc_data, AES_key[keyNum], 128);
	*enc_len = (plain_len) % 16 ? ((plain_len / 16) + 1) * 16: plain_len;

	return 1;
}

int Decrypt_AES128(int keyNum, uint8_t* enc_data, int enc_len, uint8_t* dec_data, int* dec_len) {
	AES aes;

	aes.do_aes_decrypt(enc_data, enc_len, dec_data, AES_key[keyNum], 128);
	*dec_len = strlen((char *)dec_data);

	return 1;
}

int Generate_RSA1024Key(int keyNum) {
	return 1;
}

int Encrypt_RSA1024(int key_num, uint8_t* plain_data, int plain_len, uint8_t* enc_data, int* enc_len) {
	rsa.encrypt(plain_data, plain_len, enc_data, enc_len, publicKey);

	if (enc_len == 0)
		return 0;
	return 1;
}

int Decrypt_RSA1024(int key_num, uint8_t* enc_data, int enc_len, uint8_t* plain_data, int* plain_len) {
	rsa.decrypt(plain_data, plain_len, enc_data, enc_len, privateKey);

	if (plain_len == 0)
		return 0;
	return 1;
}

int Sign_RSA1024(int key_num, uint8_t* plain_data, int plain_len, uint8_t* sign_data, int* sign_len) {
	uint8_t hash[32];
	int hashlen = 32;


	SHA_256(plain_data, plain_len, hash, &hashlen);

	rsa.encrypt(hash, hashlen, sign_data, sign_len, privateKey);
	return 1;
}

int Verify_RSA1024(int key_num, uint8_t* sign_data, int sign_len, uint8_t* org_data, int* org_len) {
	uint8_t dec_data[32];
	int dec_datalen;
	rsa.decrypt(dec_data, &dec_datalen, sign_data, sign_len, publicKey);

	uint8_t hash[32];
	int hashlen = 32;
	SHA_256(org_data, *org_len, hash, &hashlen);

	for (int i = 0; i < 32; i++) {
		if (hash[i] != dec_data[i])
			return 0;
	}

	return 1;
}

int SHA_256(uint8_t* plain_data, int plain_len, uint8_t* digest, int* digest_len) {
	SHA256_CTX ctx;
	ctx.datalen = 0;
	ctx.bitlen = 512;

	sha256_init(&ctx);
	sha256_update(&ctx, (uint8_t*)plain_data, plain_len);
	sha256_final(&ctx, (uint8_t*)digest);

	*digest_len = 32;

	return 1;
}
