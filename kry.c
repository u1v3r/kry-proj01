/*------------------------------------------------------------------------------

    1. Projekt do KRY

    Autor: Radovan Dvorsky, xdvors08

------------------------------------------------------------------------------*/


#include "kry.h"

int main(int argc, char **argv) {

	void *kasisky,*friedman;
	int kasisky_i = 0;
	double friedman_f = 0.0;

	input_text_t c_text_s;
	pthread_t kasisky_thread, friedman_thread;

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

	pthread_create(&kasisky_thread,NULL,kasiski_test,&c_text_s);
	pthread_create(&friedman_thread,NULL,friedman_test,c_text_s.orig_text);

	pthread_join(kasisky_thread,&kasisky);
	pthread_join(friedman_thread,&friedman);

	kasisky_i = *((int*)(kasisky));
	friedman_f = *((double*)(friedman));

	printf("kasisky: (%d)\n",kasisky_i);
	printf("friedman: (%f)\n",friedman_f);

	free(kasisky);
	free(friedman);
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

	while((c = getchar()) != EOF){
		if(isalpha(c)){
			if(count >= new_size - 1){
				new_size = 2*count;
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
