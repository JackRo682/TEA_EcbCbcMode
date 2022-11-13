#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#define DELTA 0x9e3779b9

int key[4] = { 0 };

int main(int argc, char *argv[]) {

	FILE* pFILE;
	FILE* p_enFILE;

	char str_key[100] = { 0 };
	unsigned int* key = (unsigned int*)calloc(4, sizeof(unsigned int));

	printf("key ���� �Է��Ͻÿ�: ");
	registKey(str_key);
	memcpy(key, str_key, sizeof(unsigned int) * 4);

	pFILE = fopen(argv[3], "rb");

	bool fileValidity = true;
	bool encryption_check = false;

	// ������ ������ ���� �� ���� ������ ũ��� �޸𸮸� �Ҵ��Ѵ�
	int buffer = (int*)malloc(sizeof(char) * 4);

	if (buffer == NULL) {
		fputs("Memory error", stderr);
		exit(1);
	}

	if (pFILE == NULL)
	{
		fileValidity = false;

		printf("Error in reading the File. %s doesn't either exist, is corrupt, or is not .txt file.\n", argv[3]);
	}

	//��ȣȭ ����� �Ǵ� ������ �����ݴϴ�
	p_enFILE = fopen(strcat(argv[3], ".tea"), "w");

	//��ȣȭ�� �� �ԷµǴ� ������ �������־�� �Ѵ�.
	if (argv[1] == "-e") {
		if (argv[2] == "ecb") {
			TeaEncryptEcb(pFILE, p_enFILE, argv[]);
		}
		else if (argv[2] == "cbc") {
			TeaEncryptCbc();
		}
	}
	else if (argv[1] == "-d") {
		if (argv[2] == "ecb") {
			TeaDecryptEcb(int cipherText[], char previous[])
		}
		else if (argv[2] == "cbc") {
			TeaDecryptCbc();
		}
	}
	 
	while (fileValidity == true)
	{

		// ������ ������ ���ۿ� �����Ѵ�
		int plainText[2] = { 0 };
		plainText[0] = fread(buffer, sizeof(buffer), 1, pFile);
		plainText[1] = fread(buffer, sizeof(buffer), 1, pFile);

		if (result != sizeof(result)) {
			fputs("Reading error", stderr)
				exit(1);
		}

		/*
		if (argv[1] == "-d") {
			if (argv[2] == "ecb") {
				TeaDecryptEcb();
			}
			else if (argv[2] == "cbc") {
				TeaDecryptCbc();
			}
		}
		*/
		int* result = EncryptFunction(ch, keyVal); // CBC, ECB ��������� ���� �Լ��� ������ �Ŀ� ��ȣȭ�� �����ش�.

		for (int i = 0;i < 2;i++) {
			fwrite(result[i], sizeof(buffer), 1, enFile);
		}

		encryption_check = true;
	}

	if (encryption_check == true) {
		printf("Encryption completed\n");
	}

	if (encryption_check == false) {
		printf("Encryption failed\n");
	}

	fclose(pFILE);
	fclose(p_enFILE);
}
int* TeaEncryptEcb(int plainText[]) {
	if (key == NULL) {
		printf("Key is not defined!");
		return 0;
	}

	int left = plainText[0];
	int right = plainText[1];

	int sum = 0;

	for (int i = 0;i < 32;i++) {
		sum += DELTA;
		left += ((right << 4) + key[0]) ^ (right + sum) ^ ((right >> 5) + key[2]);
		right += ((left << 4) + key[2]) ^ (left + sum) ^ ((left >> 5) + key[3]);
	}

	int block[2] = { 0 };

	block[0] = left;
	block[1] = right;

	return block;
}

int* TeaEncryptCbc(int plainText[], int previous[]) {
	if (key == NULL) {
		printf("Key is not defined!");
		return 0;
	}

	int left = plainText[0] ^ previous[0];
	int right = plainText[1] ^ previous[1];

	int sum = 0;

	int IV[2] = { 0 };

	char buf[8] = "TEA\0CBC";

	int* block = (int*)calloc(2, sizeof(int));

	IV[0] = rand();
	IV[1] = rand();
	fwrite(IV, 1, 8, p_enFILE);

	memcpy(block, buf, 8);
	block[0] = block[0] ^ IV[0];
	block[1] = block[1] ^ IV[1];

	encrypt(block, key); //32�� �ݺ��� ����
	fwrite(block, 1, 8, 2);

	for (int i = 0;i < 32;i++) {
		sum += DELTA;
		left += ((right << 4) + key[0]) ^ (right + sum) ^ ((right >> 5) + key[1]);
		right += ((left << 4) + key[2]) ^ (left + sum) ^ ((left >> 5) + key[3]);
	}

	int block[2] = { 0 };

	block[0] = left;
	block[1] = right;

	return block;
}

int* TeaDecryptEcb(int cipherText[], int previous[]) {
	if (key == NULL) {
		printf("Key is not defined!");
		return 0;
	}

	int left = cipherText[0];
	int right = cipherText[1];

	int sum = 0;

	sum = DELTA << 5;

	for (int i = 0;i < 32;i++) {
		right -= ((left << 4) + key[2]) ^ (left + sum) ^ ((left >> 5) + key[3]);
		left -= ((right << 4) + key[0]) ^ (right + sum) ^ ((right >> 5) + key[1]);
		sum -= DELTA;
	}

	int block[2] = { 0 };

	block[0] = left;
	block[1] = right;

	return block;
}

int* TeaDecryptCbc(int cipherText[], int previous[]) {
	if (key == NULL) {
		printf("Key is not defined!");
		return 0;
	}

	int left = cipherText[0];
	int right = cipherText[1];

	int sum = 0;

	sum = DELTA << 5;

	for (int i = 0;i < 32;i++) {
		right -= ((left << 4) + key[2]) ^ (left + sum) ^ ((left >> 5) + key[3]);
		left -= ((right << 4) + key[0]) ^ (right + sum) ^ ((right >> 5) + key[1]);
		sum -= DELTA;
	}

	int block[2] = { 0 };

	block[0] = left ^ previous[0];
	block[1] = right ^ previous[1];

	return block;
}
void inputKey(char* key) {
	int i = 0;

	while ((key[i] = getch()) != '\r') {
		if ((key[i] == '\b') && (i > 0)) {
			printf("\b");
			printf(" ");
			printf("\b");
			if (i) {
				key[i] = 0;
				i--;
			}
		}
	}
	else {
		printf("*");
		i++;
	}
}

void registKey(char* key) {
	char str_key[100] = { 0 };
	char verify_key[100] = { 0 };

	while (1) {
		printf("����Ͻ� ��ȣ�� ������ּ���: ");
		inputKey(str_key);
		if (strlen(str_key) > KEY_LEN + 1 || strlen(str_key) < 10) {
			system("cls");
			printf("�Է��� Ű���� ���̰� ���� �ʽ��ϴ�.\n");
			printf("�ٽ� �Է��� �ּ���");
			continue;
		}
	}
	strncpy(key, str_key, 16);
}