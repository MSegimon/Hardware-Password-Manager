#include "sha256.c"
#include <stdio.h>
int main(){
	uint8_t hash[32];
	calc_sha_256(hash, "abc", strlen("abc"));
	for(int i = 0; i < 32; i++){
		printf("%x",hash[i]);
	}
	printf("\n");
	return 0;
}
