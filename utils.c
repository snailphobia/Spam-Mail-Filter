#include "utils.h"

char **load_keywords(int *n)
{
	FILE *f1;
	f1 = fopen("data/Keywords", "r");
	if (!f1) {
		printf(RED "failed to load Keywords . . .\n" DEF);
		exit(-1);
	}
	fscanf(f1, "%d", n);
	char **words = NULL;
	words = calloc(*n + 30, sizeof(int *));
	for (int i = 0; i < *n; i++) {
		words[i] = calloc(30, sizeof(char));
		fscanf(f1, "%30s", words[i]);
	}
	for (int i = *n; i < *n + 20; i++)
		words[i] = calloc(30, sizeof(char));
	snprintf(words[*n], 30, "subscri"); *n += 1;
	// snprintf(words[*n], 30, "benefit"); *n += 1;
	snprintf(words[*n], 30, "offer"); *n += 1;
	snprintf(words[*n], 30, "dollar"); *n += 1;
	snprintf(words[*n], 30, "bonus"); *n += 1;
	snprintf(words[*n], 30, "newsletter"); *n += 1;
	//snprintf(words[*n], 30, "www."); *n += 1;
	fclose(f1);
	return words;
}

FILE *load_mail(char *name)
{
	FILE *mail;
	mail = fopen(name, "r");
	if (!mail) {
		printf(RED "warning: failed to load mail %s . . .\n" DEF, name);
		return NULL;
	}
	return mail;
}

void pass_to_body(FILE *mail)
{
	char *word = malloc(300);
	while (fscanf(mail, "%s", word) != -1) {
		if (parse_word(word, "body"))
			break;
	}
	free(word);
}

void pass_to_email(FILE *mail)
{
	char *word = malloc(300);
	while (fscanf(mail, "%s", word) != -1) {
		if (parse_word(word, "from:"))
			break;
	}
	free(word);
}

int parse_mail(FILE *mail, char **keywords, int **keywordapp,
			   int N, int index)
{
	char *word = calloc(260, sizeof(char));
	int word_count = 0;
	while (fscanf(mail, "%s", word) != -1) {
		word_count++;
		for (int i = 0; i < N; i++)
			keywordapp[index][i] += parse_word(word, keywords[i]);
	}
	free(word);
	fclose(mail);
	return word_count;
}

double stdev_calc(int **keywordapp, int N, int word_N)
{
	double avg = 0;
	for (int j = 0; j < N; j++)
		avg += keywordapp[j][word_N];
	avg /= (double)N;
	double stdev = 0;
	for (int j = 0; j < N; j++)
		stdev += pow((keywordapp[j][word_N] - avg), 2);
	stdev /= (double)N;
	return stdev;
}

int load_spammers(SPAMMERS *sp)
{
	FILE *spam = fopen("data/spammers", "r");
	if (!spam) {
		printf(RED "failed to retrieve ./data/spammers file . . .\n" DEF);
		exit(69);
	}

	int num;
	fscanf(spam, "%d", &num);
	for (int i = 0; i < num; i++) {
		sp[i].email = calloc(200, sizeof(char));
		fscanf(spam, "%s %d", sp[i].email, &sp[i].score);
	}

	fclose(spam);
	return num;
}

int mail_trial(FILE *mail)
{
	int word_count = 0, capital_char = 0, total_char_count = 0;
	char *word = calloc(260, sizeof(char));
	while (fscanf(mail, "%s", word) != -1) {
		word_count++;
		int len = strlen(word);
		for (int i = 0; i < len; i++) {
			if ('A' <= word[i] && 'Z' >= word[i])
				capital_char++;
			total_char_count++;
		}
	}
	free(word);
	double delta = (double)(capital_char) / (double)(total_char_count);
	if (delta > 0.10)
		return (delta - 0.10) * 95;
	return 0;
}

int score_calc(FILE *mail, int **keywordapp, SPAMMERS *sp,
			   char *email, int s_ind, int i, int N, double gamma)
{
			double score1 = 0, score2 = 0, score3 = 0, score;
			for (int K = 0; K < s_ind; K++)
				if (!strcmp(sp[K].email, email)) {
					score3 = (double)sp[K].score * 40 / 35;
					break;
				}
				pass_to_body(mail);
			for (int j = 0; j < N; j++)
				score1 += keywordapp[i][j];
			score1 *= 10 * gamma;
			score2 = mail_trial(mail);
			score = score1 + score2 + score3;
			return (int)score;
}
