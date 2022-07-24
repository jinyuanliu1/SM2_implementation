#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

char* hmac(const char* algorithm, const char* msg, size_t msgLen, const char* key, size_t keyLen) {
	if (algorithm == NULL || msg == NULL || key == NULL) {
		printf("%s %d %s: parameter error\n", __FILE__, __LINE__, __func__);
		exit(1);
	}

	const EVP_MD* md = EVP_get_digestbyname(algorithm);
	if (md == NULL) {
		printf("%s %d %s: unknown message digest: %s\n", __FILE__, __LINE__, __func__, algorithm);
		exit(1);
	}

	unsigned char md_value[EVP_MAX_MD_SIZE] = "";
	unsigned int md_len = 0;

#if !defined(OPENSSL_VERSION_NUMBER) || OPENSSL_VERSION_NUMBER < 0x10100000L
	HMAC_CTX ctx;
	HMAC_CTX_init(&ctx);
	HMAC_Init_ex(&ctx, key, keyLen, md, NULL);
	HMAC_Update(&ctx, msg, msgLen);
	HMAC_Final(&ctx, md_value, &md_len);
	HMAC_CTX_cleanup(&ctx);
#else
	HMAC_CTX* ctx;
	ctx = HMAC_CTX_new();
	HMAC_Init_ex(ctx, key, keyLen, md, NULL);
	HMAC_Update(ctx, (unsigned char*)msg, msgLen);
	HMAC_Final(ctx, md_value, &md_len);
	HMAC_CTX_free(ctx);
#endif

	static const char bin2chars[] = "0123456789abcdef";
	char* result = (char*)malloc(md_len * 2 + 1);
	if (result == NULL) {
		exit(0);
	}
	else {
		result[md_len * 2] = 0;
		for (unsigned int i = 0; i < md_len; i++) {
			result[i * 2] = bin2chars[md_value[i] >> 4];
			result[i * 2 + 1] = bin2chars[md_value[i] & 0x0f];
		}
	}
	printf("%s %d %s: %s\t\t%s\n", __FILE__, __LINE__, __func__, algorithm, result);
	return result;
}

char* sha1(const unsigned char* d, size_t n, unsigned char* md) {
	SHA1(d, strlen((const char*)d), md);
	char temp[41] = { 0 };
	char temp2[2];
	char temp3[2];
	int i;

	for (i = 0; i < 20; i++)
	{
		_itoa_s(int(md[i]) / 16, temp2, 16);
		temp2[1] = '\0';
		_itoa_s(int(md[i]) % 16, temp3, 16);
		temp3[1] = '\0';
		strcat_s(temp, temp2);
		strcat_s(temp, temp3);
	}
	
	return temp;
}


bool my_strcmp(const char p[], const char q[]) {
	char pp[5] = { 0 };
	char qq[5] = { 0 };
	strncpy(pp, p, 4);
	strncpy(qq, q, 4);
	int a = strtol(pp, 0, 16);
	int b = strtol(qq, 0, 16);
	if (a < b)
		return false;
	else
		return true;
}

/*generate a 160-bit private key for ECDSA*/
char* generate_key(const char q[]) {
	while (1) {
		char key[161] = { 0 };
		char temp0[2] = { '0','\0' };
		char temp1[2] = { '1','\0' };
		srand((unsigned)(time(NULL)));
		for (int i = 0; i < 160; i++) {
			int number = rand();
			if (number % 2 == 0)
				strcat(key, temp0);
			else
				strcat(key, temp1);
		}
		char _key[41] = { 0 };
		for (int i = 0; i < 40; i++) {
			int temp = 0;
			temp = int(key[4 * i] - '0') * 8 + int(key[4 * i + 1] - '0') * 4 + int(key[4 * i + 2] - '0') * 2 + int(key[4 * i + 3] - '0');
			switch (temp)
			{
			case 0:
				strcat(_key, "0");
				break;
			case 1:
				strcat(_key, "1");
				break;
			case 2:
				strcat(_key, "2");
				break;
			case 3:
				strcat(_key, "3");
				break;
			case 4:
				strcat(_key, "4");
				break;
			case 5:
				strcat(_key, "5");
				break;
			case 6:
				strcat(_key, "6");
				break;
			case 7:
				strcat(_key, "7");
				break;
			case 8:
				strcat(_key, "8");
				break;
			case 9:
				strcat(_key, "9");
				break;
			case 10:
				strcat(_key, "a");
				break;
			case 11:
				strcat(_key, "b");
				break;
			case 12:
				strcat(_key, "c");
				break;
			case 13:
				strcat(_key, "d");
				break;
			case 14:
				strcat(_key, "e");
				break;
			case 15:
				strcat(_key, "f");
				break;
			}
		}
		/*if _key < q , return _key*/
		if (!my_strcmp(_key, q)) {
			return _key;
		}
		else
			continue;
	}
}

int main() {

	OpenSSL_add_all_digests();

	//const char* msg = "0123456789ABCDEF";
	//const char* key = "test1280";

	/* msg和key都是可读明文，不包含0x00，因此可以用strlen */
	/* 考虑到通用情况，不应该使用strlen，因为原数据可能包含0x00 */
	//hmac("SHA1", msg, strlen((char*)msg), key, strlen(key));
	//hmac("SHA224", msg, strlen((char*)msg), key, strlen(key));
	//hmac("SHA256", msg, strlen((char*)msg), key, strlen(key));
	//hmac("SHA384", msg, strlen((char*)msg), key, strlen(key));
	//hmac("SHA512", msg, strlen((char*)msg), key, strlen(key));
	//hmac("MD5", msg, strlen((char*)msg), key, strlen(key));

	/* Call this once before exit. */
	//EVP_cleanup();

	char v[41] = { 0 };
	char temp1[3] = { '0','1','\0' };
	char k[41] = { 0 };
	char temp0[3] = { '0','0','\0' };
	for (int i = 0; i < 20; i++) {
		strcat(k, temp0);         //K = 0x00 0x00 0x00...  0x00 
		strcat(v, temp1);         //V = 0x01 0x01 0x01...  0x01
	}
	char input[300] = { 0 };
	strcat(input, v);
	strcat(input, temp0);

	/*Generate relevant parameters in advance by Python*/
	/*parameter p:at least 1024bit*/
	/*parameter q:in most cases 160bit && q|p-1*/
	/*parameter private_key:range from 0 to q*/
	const char q[] = "d475c7e3f40656a4cc9f120aa96f5bde3f8bbbeb";
	const char p[] = "34fa5437a7d2ffc97d8ca89cf674fdf9036e1da9a3b8a7aefe230f613dee282735aa1ba7c32e31076d2de36edbd775e5d2388277fc14651c2b23c6c7aa774fe10c960748892cca876d049b2a128217d7fe308e3334ce728fc5de181f19bb4051023f838813ec2a1d5521348f14be87562e8a071016e6e1e7df9242eadd57ea11bb";
	char* private_key = (char*)malloc(40);
	char _private_key[41] = { 0 };
	private_key = generate_key(q);
	strcpy(_private_key, private_key);
	strcat(input, private_key);

	unsigned const char ibuf[] = "compute k";       //input message
	unsigned char* obuf = (unsigned char*)malloc(20);
	char* h1 = (char*)malloc(40);
	char _h1[41] = { 0 };
	h1 = (char*)sha1(ibuf, strlen((const char*)ibuf), obuf);
	strcpy(_h1, h1);
	strcat(input, h1);
	
	/*
	K = HMAC_K(V || 0x00 || int2octets(x) || bits2octets(h1))
	V = HMAC_K(V)
	*/
	char* temp_k = (char*)malloc(40);
	temp_k = hmac("SHA1", input, strlen((char*)input), k, strlen(k));
	strcpy(k, temp_k);
	char* temp_v = (char*)malloc(40);
	temp_v = hmac("SHA1", v, strlen((char*)v), k, strlen(k));
	strcpy(v, temp_v);

	/*
	K = HMAC_K(V || 0x01 || int2octets(x) || bits2octets(h1))
	V = HMAC_K(V)
	*/
	strcpy(input, v);
	strcat(input, temp1);
	strcat(input, _private_key);
	strcat(input, _h1);
	temp_k = hmac("SHA1", input, strlen((char*)input), k, strlen(k));
	strcpy(k, temp_k);
	temp_v = hmac("SHA1", v, strlen((char*)v), k, strlen(k));
	strcpy(v, temp_v);

	char T[41] = { 0 };
	temp_v = hmac("SHA1", v, strlen((char*)v), k, strlen(k));
	strcpy(v, temp_v);
	strcpy(T, temp_v);
	while (1) {
		if (!my_strcmp(T, q)) {
			printf("构造算法生成的伪随机数k为 % s", T);
			break;
		}
		else
		{
			strcpy(input, v);
			strcat(input, temp0);
			temp_k = hmac("SHA1", input, strlen((char*)input), k, strlen(k));
			strcpy(k, temp_k);
			temp_v = hmac("SHA1", v, strlen((char*)v), k, strlen(k));
			strcpy(v, temp_v);
			temp_v = hmac("SHA1", v, strlen((char*)v), k, strlen(k));
			strcpy(v, temp_v);
			strcpy(T, temp_v);
		}
	}

	return 0;
}
