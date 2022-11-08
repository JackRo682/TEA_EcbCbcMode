#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

void print_binary_8bit(char num){
	for(int i = 7; i >= 0; i--){
		printf("%d", num & (1 << i) ? 1 : 0);
	}
}

void print_binary_32bit(int num){
	for(int i = 31; i >= 0; i--){
		if(i % 8 == 7)
			printf(" ");
		printf("%d", num & (1 << i) ? 1 : 0);
	}
}

// long long tea(long long planeText, )

void ecb_en(char* file){
	FILE *fp;
	char* header = "TEA\0ECB\0";
	unsigned int left = 0;
	unsigned int right = 0;

	unsigned int header_left = 0; // 0x00000000
	unsigned int header_right = 0; // 0x00000000

	unsigned int planeText[8] = {};
	char inputStr[16] = {0, };
	unsigned int key_c = 0;
	unsigned int key[4] = {0, };
	char ch;
	int p = 0;
	unsigned int sum = 0;
	unsigned int delta = 0x9e3779b9;
	
	// Open file and Bring planeText from File 'test.pdf'
	fp = fopen(file, "r");

	if(fp != NULL){
		while(EOF != (ch = fgetc(fp))){
			planeText[p] = ch;
			p++;
		}
        printf("\n");
	}

	// print header
	printf("Header in Binary:\n");
	for(int i = 0; i < 8; i++){
		printf("header[%d]: ", i);
		print_binary_8bit(header[i]);
		printf("\n");
	}
    
	// arrange planeText into left and right
	for(int i = 0; i < 4; i++){
		left += planeText[i] << (8 * (7-i));
	}
	for(int i = 4; i < 8; i++){
		right += planeText[i] << (8 * (7-i));
	}

	// input key
    while(!(strlen(inputStr) >= 10 && strlen(inputStr) <= 16)){
        printf("Input Key(10 ~ 16 characters): ");
	    scanf("%s", inputStr);
    }
	// divide key string by array
	for(int i = 0; i < 16; i++){
		key_c = inputStr[i];
		key[i / 4] += key_c << 8 * (3 - (i - 4 * (i / 4)));
	}
	// // print key
	// for(int i = 0; i < 4; i++){
	// 	printf("key[%d]: ", i);
	// 	print_binary_32bit(key[i]);
	// 	printf("\n");
	// }

	// encoding
	for(int i = 0; i < 32; i++){
		sum += delta;
		left += ((right << 4) + key[0]) ^ (right + sum) ^ ((right >> 5) + key[1]);
		right += ((left << 4) + key[2]) ^ (left + sum) ^ ((left >> 5) + key[3]);
	}
	printf("left: ");
	print_binary_32bit(left);
	printf("\n");
	printf("right: ");
	print_binary_32bit(right);
	printf("\n");
	
	// delete original file and make encoded file
	fp = fopen(strcat(file, ".tea"), "w");
	
	for(int i = 0; i < 8; i++){
		planeText[i] = header[i];
	}
	for(int i = 0; i < 4; i++){
		header_left += planeText[i] << (8 * (7-i));
	}
	for(int i = 4; i < 8; i++){
		header_right += planeText[i] << (8 * (7-i));
	}
	sum = 0;
	for(int i = 0; i < 32; i++){
		sum += delta;
		header_left += ((header_right << 4) + key[0]) ^ (header_right + sum) ^ ((header_right >> 5) + key[1]);
		header_right += ((header_left << 4) + key[2]) ^ (header_left + sum) ^ ((header_left >> 5) + key[3]);
	}



	for(int i = 3; i >= 0; i--){
		ch = 0xff; // 11111111
		ch = ch & (left >> (8 * i));
		fputc(ch, fp); // 11111111
	}
	for(int i = 3; i >= 0; i--){
		ch = 0xff;
		ch = ch & (right >> (8 * i));
		fputc(ch, fp);
	}


    //file close
	fclose(fp);
}
void ecb_de(char* file){
	FILE* fp;
	fp = fopen(file, "r");


}
void cbc_en(char* file){
	
}
void cbc_de(char* file){
	
}

int main(int argc, char* argv[]){
	
	char* crypt = argv[1]; // [encryption/decryption]
	char* mode = argv[2]; // [mode]
	char* file = argv[3]; // [file_name]

	printf("argv[0]: %s / argv[1]: %s / argv[2]: %s / argv[3]: %s\n", argv[0], argv[1], argv[2], argv[3]);

	
	if((argc == 4)&&(access(file, F_OK) != -1)){ // check nums of argc and existance of file
		if((strcmp(mode, "ecb") == 0) && (strcmp(crypt, "-e") == 0)){ // mode: ECB, crypt: encryption
			ecb_en(file);	
		}else if((strcmp(mode, "ecb") == 0) && (strcmp(crypt, "-d") == 0)){ // mode: ECB, crypt: decryption 
			ecb_de(file);
		}else if((strcmp(mode, "cbc") == 0) && (strcmp(crypt, "-e") == 0)){ // mode: CBC, crypt: encryption
			cbc_en(file);
		}else if((strcmp(mode, "cbc") == 0) && (strcmp(crypt, "-d") == 0)){ // mode: CBC, crypt: decryption
			cbc_de(file);
		}else{
			printf("Wrong Mode Or Crypt!\n");
			return -1;
		}
	}else{
		printf("Wrong Input!\n");
		return -1;
	}

	return 0;
}