#include "utils.h"

int8_t parse_word(char *bigword, char *smallword)
{
	int len1 = strlen(bigword), len2 = strlen(smallword);
	int counter = 0;
	for (char *it = bigword; it < bigword + len1; it++) {
		if (compare_words(it, smallword)) {
			counter++;
			break;
		}
	}
	return counter;
}

int8_t compare_words(char *tr_word, char *srcword)
{
	int len1 = strlen(tr_word);
	int len2 = strlen(srcword);
	if (len1 < len2)
	    return 0;
	for (int i = 0; i < len2; i++) {
		tr_word[i] = tolower(tr_word[i]);
		if (tr_word[i] != srcword[i])
			return 0;
	}
	return 1;
}
