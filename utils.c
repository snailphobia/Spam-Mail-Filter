#include "utils.h"

char **load_keywords(int *n, int *init_n)
{
	FILE *f1;
	f1 = fopen("data/keywords", "r");
	if (!f1) {
		printf(RED "failed to load keywords . . .\n" DEF);
		exit(-1);
	}
	fscanf(f1, "%d", n);
	*init_n = *n;
	char **words = NULL;
	words = calloc(*n + 30, sizeof(int *));
	for (int i = 0; i < *n; i++) {
		words[i] = calloc(20, sizeof(char));
		fscanf(f1, "%20s", words[i]);
	}
	for (int i = *n; i < *n + 30; i++)
		words[i] = calloc(20, sizeof(char));
	snprintf(words[*n], 20, "subscrip"); *n += 1;
	snprintf(words[*n], 20, "offer"); *n += 1;
	snprintf(words[*n], 20, "dollar"); *n += 1;
	snprintf(words[*n], 20, "bonus"); *n += 1;
	snprintf(words[*n], 20, "premium"); *n += 1;
	snprintf(words[*n], 20, "demo"); *n += 1;
	snprintf(words[*n], 20, "valu"); *n += 1;
	snprintf(words[*n], 20, "price"); *n += 1;
	snprintf(words[*n], 20, "client"); *n += 1;
	snprintf(words[*n], 20, "rates"); *n += 1;
	snprintf(words[*n], 20, "bills"); *n += 1;
	snprintf(words[*n], 20, "wealth"); *n += 1;
	snprintf(words[*n], 20, "earning"); *n += 1;
	snprintf(words[*n], 20, "bank"); *n += 1;
	snprintf(words[*n], 20, "transaction"); *n += 1;
	for (int i = 1; i < 10; i++) {
		snprintf(words[*n], 20, "$%d", i);
		*n += 1;
	}
	for (int i = *n; i < *init_n + 30; i++)
		free(words[i]);
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
		if (parse_word(word, "body:"))
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
	int position = ftell(mail);
	fscanf(mail, "%s", word);
	while (1) {
		int len = strlen(word);
		for (char *c_it = word; c_it < word + len; c_it++)
			if (*c_it == '@') {
				fseek(mail, position, SEEK_SET);
				goto exit;
			}
		position = ftell(mail);
		fscanf(mail, "%s", word);
		if (compare_words(word, "body:")) {
			fseek(mail, position, SEEK_SET);
			break;
		}
	}
exit:
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
		printf(RED "failed to retrieve spammers file . . .\n" DEF);
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
	double caps_spam = 0.0;
	char *word = calloc(260, sizeof(char));
	while (fscanf(mail, "%s", word) != -1) {
		word_count++;
		int len = strlen(word), test_full_caps = 0;
		for (int i = 0; i < len; i++) {
			if ('A' <= word[i] && 'Z' >= word[i])
				capital_char++, test_full_caps++;
			total_char_count++;
		}
		if (test_full_caps == len)
			caps_spam += 0.33;
	}
	free(word);
	double delta = (double)(capital_char) / (double)(total_char_count);
	if (delta > 0.10)
		return (delta - 0.10) * 100.0 + caps_spam;
	return 0;
}

int score_calc(FILE *mail, int **keywordapp, SPAMMERS *sp,
			   char *email, double *stdev,
			   int s_ind, int i, int N, double gamma)
{
			double score1 = 0, score2 = 0, score3 = 0, score;
			for (int K = 0; K < s_ind; K++)
				if (parse_word(email, sp[K].email) >= 1) {
					score3 = (double)sp[K].score * 45.0 / 35.0;
					break;
				}
				pass_to_body(mail);
			for (int j = 0; j < N; j++) {
				if (keywordapp[i][j] <= 1)
					score1 += (double)keywordapp[i][j];
				if (keywordapp[i][j] == 2)
					score1 += 1.6;
				if (keywordapp[i][j] >= 3)
					score1 += sqrt((double)keywordapp[i][j]);
			}
			gamma = log2(gamma + 1.0);
			score1 *= 10.0 * gamma;
			score2 = mail_trial(mail);
			score = score1 + score2 + score3;
			return (int)score;
}
