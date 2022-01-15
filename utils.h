#pragma once
#define RED "\e[31m"
#define GREEN "\e[32m"
#define DEF "\e[0m"
#define PREC_MULT 10000000
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <ctype.h>

typedef struct {
	char *email;
	int score;
} SPAMMERS;

char **load_keywords(int *n);
FILE *load_mail(char *name);
void pass_to_body(FILE *mail);
void pass_to_email(FILE *mail);
int parse_mail(FILE *mail, char **keywords, int **keywords_app,
			   int N, int index);
int8_t parse_word(char *big_word, char *small_word);
int8_t compare_words(char *tr_word, char *srcword);
double stdev_calc(int **keywords_app, int N, int word_n);
int load_spammers(SPAMMERS *sp);
int mail_trial(FILE *mail);
int score_calc(FILE *mail, int **keywordapp, SPAMMERS *sp,
			   char *email, int s_ind, int i, int N, double gamma);
