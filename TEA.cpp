#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#define DELTA 0x9e3779b9

int main(int argc, char *argv[]) {
	FILE* fs;

	if (argv[1] == "-e") {
		if (argv[2] == "ecb") {
			TeaEncryptEcb();
		}
		else if (argv[2] == "cbc") {
			TeaEncryptCbc();
		}
	}

	if (argv[1] == "-d") {
		if (argv[2] == "ecb") {
			TeaDecryptEcb();
		}
		else if (argv[2] == "cbc") {
			TeaDecryptCbc();
		}
	}
}

void fileOpen(FILE* fs, char *argv[]) {

	//예시코드
	if ((fp = fopen(fileName, "r")) == NULL) {
		fprintf(stderr, "원본 파일 %s을 열 수 없습니다.\n", fileName);

		exit(1);
	}

	while ((c = fgetc(fp)) != EOF) {
		if (isprint(c))
			count++;
	}

	fclose(fp);

	return 0;
	//예시코드 끝




	FILE* pFILE;
	FILE* p_enFILE;

	char infile_path[100];

	printf("파일 이름을 입력하시오 : ");

	scanf("%s", infile_path);

	pFILE = fopen(infile_path, "rb");

	bool fileValidity = true;
	bool encryption_check = false;

	if (pFILE == NULL)
	{
		fileValidity = false;

		printf("Error in reading the File. %s doesn't either exist, is corrupt, or is not .txt file.\n", infile_path);
	}

	append(infile_path, ".tea");

	p_enFILE = fopen("outfile.txt", "w"); // ?

	while (fileValidity == true)
	{
		char ch = fgetc(pFILE);

		if (ch == EOF) {
			fileValidity = false;
		}

		char en_ch = rightEncrypt(ch, keyVal);

		fputc(en_ch, p_enFILE);

		encryption_check = true;
	}

	if (encryption_check == true) {
		printf("Usage: %s %s [encrypt %s %d | decrypt outfile.txt %d] \n", progName, infile_path, infile_path, keyVal, keyVal);
	}

	if (encryption_check == false) {
		printf("Encryption failed\n");
	}

	fclose(pFILE);
	fclose(p_enFILE);
}

int* TeaEncryptEcb() {
	if (key == NULL) {
		printf("Key is not defined!");
		return 0;
	}

	int left = plainText[0];
	int right = plainText[1];

	int sum = 0;

	for (int i = 0;i < 32;i++) {
		sum += DELTA;
		left += ((right << 4) + key[0]) ^ (right + sum) ^ ((right >> 5) + key[i]);
		right += ((left << 4) + key[2]) ^ (left + sum) ^ (left >> 5) + key[3]);
	}

	int block[2] = { 0 };
	block[0] = left;
	block[1] = right;

	return block;
}

int* TeaEncryptCbc() {
	if (key == NULL) {
		printf("Key is not defined!");
		return 0;
	}

	int left = plainText[0] ^ previous[0];
	int right = plainText[1] ^ previous[1];

	int sum = 0;

	for (int i = 0;i < 32;i++) {
		sum += DELTA;
		left += ((right << 4) + key[0]) ^ (right+sum) ^ ((right >> 5) + key[1]);
		right += ((left << 4) + key[2]) ^ (left + sum) ^ ((left >> 5) + key[3]);
	}

	int block[2] = { 0 };
	block[0] = left;
	block[1] = right;

	return block;
}

int* TeaDecryptEcb() {
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

int* TeaDecryptCbc() {
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