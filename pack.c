#include <stdint.h>
#include <stdio.h>
#include "stringutil.h"

/*
Pack incoming lines.
format is
TOKEN
DATABYTES

tokens- 
0 = NOP
1 = i8
2 = u8
3 = i16
4 = u16
5 = i32
6 = u32
7 - 255 = token
*/

void main_unpack(){
	while(!feof(stdin)){
		unsigned char tok = fgetc(stdin);
		if(tok == 0) {
			printf("nop\n");
			continue;
		}
		if(tok > 6) {
			printf("tok %u\n", tok);
			continue;
		}
		if(tok == 1){ /*i8 data type*/
			uint8_t val = fgetc(stdin); /*No conversion required.*/
			int8_t val_signed;
			memcpy(&val_signed, &val, sizeof(int8_t));
			printf("i8 %d\n", (int)val_signed);
			continue;
		}
		if(tok == 2){ /*u8 data type*/
			uint8_t val = fgetc(stdin);
			printf("u8 %u\n", (unsigned int)val);
			continue;
		}
		if(tok == 3){ /*i16 data type*/
			uint16_t val = 0;
			val = ((uint16_t)fgetc(stdin)) * 256; /*Upper half*/
			val += ((uint16_t)fgetc(stdin)); /*Lower half.*/
			int16_t val_signed;
			memcpy(&val_signed, &val, sizeof(uint16_t));
			printf("i16 %d\n", (int)val_signed);
			continue;
		}
		if(tok == 4){ /*u16 data type*/
			uint16_t val = 0;
			val = ((uint16_t)fgetc(stdin)) * 256; /*Upper half*/
			val += ((uint16_t)fgetc(stdin)); /*Lower half.*/
			printf("u16 %u\n", (unsigned int)val);
			continue;
		}

		if(tok == 5){ /*i32 data type*/
			uint32_t val = 0;
			val = ((uint32_t)fgetc(stdin))  * 256 * 256 * 256; /*Upper half*/
			val += ((uint32_t)fgetc(stdin)) * 256 * 256; /*Upper half*/
			val += ((uint32_t)fgetc(stdin)) * 256; /*Upper half*/
			val += ((uint32_t)fgetc(stdin)); /*Bottom.*/
			int32_t val_signed;
			memcpy(&val_signed, &val, sizeof(uint32_t));
			printf("i32 %d\n", (int)val_signed);
			continue;
		}
		if(tok == 6){ /*u32 data type*/
			uint32_t val = 0;
			val = ((uint32_t)fgetc(stdin))  * 256 * 256 * 256; /*Upper half*/
			val += ((uint32_t)fgetc(stdin)) * 256 * 256; /*Upper half*/
			val += ((uint32_t)fgetc(stdin)) * 256; /*Upper half*/
			val += ((uint32_t)fgetc(stdin)); /*Bottom.*/
			printf("u32 %u\n", (unsigned int)val);
			continue;
		}
	}
	puts("#END OF AUTOMATICALLY PARSED FILE");
}


void main_pack(){
	long signed_int;
	unsigned long unsigned_int;
	unsigned long lenout;
	while(!feof(stdin)){
		char* line = read_until_terminator_alloced(stdin, &lenout, '\n', 10);
		if(line == NULL) return; //Failed malloc.
		if(strprefix("#", line)) goto end;
		if(strlen(line) < 1 || strprefix("NOP", line) ||
		strprefix("nop", line)){
			fputc(0, stdout);
			goto end;
		}
	if(strprefix("tok", line)){
		if(strlen(line) < 4) goto end; /*Invalid line- must contain at least 3 characters.*/
		unsigned_int = strtoull(line+3, NULL, 0) & 0xff; /*Attempt */
		if(unsigned_int < 7 || unsigned_int > 255) goto end;; /*Syntax Error*/
		fputc(unsigned_int, stdout);
		goto end;
	}

	if(strprefix("i8",line)){
		if(strlen(line) < 3) goto end; /*Invalid line- must contain at least 3 characters.*/
		signed_int = strtoll(line+3, NULL, 0); /*Attempt */
		memcpy(&unsigned_int, &signed_int, sizeof(long));
		/*write out token*/
		fputc(1, stdout);
		fputc((unsigned_int)&0xff, stdout);
		goto end;
	}
	if(strprefix("u8",line)){
		if(strlen(line) < 3) goto end; /*Invalid line- must contain at least 3 characters.*/
		unsigned_int = strtoull(line+3, NULL, 0); /*Attempt */
		/*write out token*/
		fputc(2, stdout);
		fputc((unsigned_int)&0xff, stdout);
		goto end;
	}

		
		if(strprefix("i16",line)){
			if(strlen(line) < 4) goto end; /*Invalid line- must contain at least 4 characters.*/
			signed_int = strtoll(line+4, NULL, 0); /*Attempt */
			memcpy(&unsigned_int, &signed_int, sizeof(long));
			unsigned_int &= 0xFFFF;
			/*write out token*/
			fputc(3, stdout);
			fputc((unsigned_int/(256*1*1))&0xff, stdout);
			fputc((unsigned_int)&0xff, stdout);
			goto end;
		}
		if(strprefix("u16",line)){
			if(strlen(line) < 4) goto end; /*Invalid line- must contain at least 4 characters.*/
			unsigned_int = strtoull(line+4, NULL, 0); /*Attempt */
			unsigned_int &= 0xFFFF;
			/*write out token*/
			fputc(4, stdout);
			fputc((unsigned_int/(256*1*1))&0xff, stdout);
			fputc((unsigned_int)&0xff, stdout);
			goto end;
		}

		
		if(strprefix("i32",line)){
			if(strlen(line) < 4) goto end; /*Invalid line- must contain at least 4 characters.*/
			signed_int = strtoll(line+4, NULL, 0); /*Attempt */
			memcpy(&unsigned_int, &signed_int, sizeof(long));
			/*write out token*/
			fputc(5, stdout);
			fputc((unsigned_int/(256*256*256))&0xff, stdout);
			fputc((unsigned_int/(256*1*256))&0xff, stdout);
			fputc((unsigned_int/(256*1*1))&0xff, stdout);
			fputc((unsigned_int)&0xff, stdout);
			goto end;
		}
		if(strprefix("u32",line)){
			if(strlen(line) < 4) goto end; /*Invalid line- must contain at least 4 characters.*/
			unsigned_int = strtoull(line+4, NULL, 0); /*Attempt */
			/*write out token*/
			fputc(6, stdout);
			fputc((unsigned_int/(256*256*256))&0xff, stdout);
			fputc((unsigned_int/(256*1*256))&0xff, stdout);
			fputc((unsigned_int/(256*1*1))&0xff, stdout);
			fputc((unsigned_int)&0xff, stdout);
			goto end;
		}
		fputc(0, stdout); /*Write out a NOP if we didnt find anything.*/
		end:
		free(line);
	}
	
}


int main(int argc, char** argv){
	(void)argv;
	if(argc < 2){
		main_pack();
		return 0;
	}
	puts("#AUTOMATICALLY UNPACKED FILE- GENERATED FROM TOKENIZED FORM");
	main_unpack(); return 0;
}
