/*------------------------------------------------------------------------------

    1. Projekt do KRY

    Autor: Radovan Dvorsky, xdvors08

------------------------------------------------------------------------------*/



#include "kry.h"


int main(int argc, char **argv) {

	kasiski_thread_result_t *kasisky_result;
	double friedman_result = 0.0;
	void *f_result;
	input_text_t c_text_s;
	int result = 1, i;
	int *password;

	if(argc != 1){
		printf("bad param\n");
		return 1;
	}

	c_text_s.text = (char *)malloc((INPUT_TEXT_MAX_LENGTH + 2) * sizeof(char));
	c_text_s.orig_text = strip_stdin(c_text_s.text);


#if DEBUG == 2
	printf("text: %s\n",c_text);
	printf("len: %d\n",c_text_s.len);
#endif



	f_result = friedman_test(c_text_s.orig_text);
	friedman_result = *((double*)f_result);
	c_text_s.friedman_res = friedman_result;
	kasisky_result = (kasiski_thread_result_t *)kasiski_test(&c_text_s,friedman_result);
	result = ic_passwd_len(c_text_s.orig_text,strlen(c_text_s.orig_text),friedman_result,kasisky_result);
	password = crack_paswd(c_text_s.orig_text,result,strlen(c_text_s.orig_text));


	printf("%2.4f;%d;%d;",friedman_result,*((int *)kasisky_result->values[0].key),result);
	/*printf("%2.4f;%d;%d;",friedman_result,result,result);*/
	for (i = 0; i < result; ++i) {
		printf("%c",password[i] + 'a');
	}
	printf("\n");

	free(f_result);
	free(kasisky_result->values);
	free(kasisky_result);
	free(password);
	free(c_text_s.text);
	free(c_text_s.orig_text);

	return 0;
}

/*
 * Z textu odstrani vsetky nepotrebne znaky
 *
 * Vracia dlzku vystupneho textu
 */
char *strip_stdin(char *output_text){

	int c, count = 0, new_size = INPUT_TEXT_MAX_LENGTH;
	char *orig_text = (char *)calloc(new_size,sizeof(char));

#ifdef PROFILE
	FILE *file;
	file = fopen("tests/inputs/mexico.txt","r");
	while((c = getc(file)) != EOF){
#else
	while((c = getchar()) != EOF){
#endif
		if(isalpha(c)){
			if(count >= new_size - 1){
				new_size = 3*count;
#if DEBUG == 2
				printf("reall new size: %d\n",new_size);
#endif
				orig_text = (char *)realloc(orig_text,new_size * sizeof(char));
			}
			c = tolower(c);
			orig_text[count] = c;

			if(count <= INPUT_TEXT_MAX_LENGTH){
				output_text[count] = c;
				if(count == INPUT_TEXT_MAX_LENGTH){
					output_text[count + 1] = 0;
				}
			}

			count++;
		}
	}

	orig_text[count] = 0;

	return orig_text;
}

