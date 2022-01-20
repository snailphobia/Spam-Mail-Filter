#include "utils.h"
#include <time.h>
int main(void)
{
	int N = 0, init_n, t0 = clock();
	char **keywords = load_keywords(&N, &init_n);
	int *wordc = calloc(10000, sizeof(int));
	DIR *directory;
	struct dirent *entry;
	directory = opendir("data/emails/");
	if (!directory) {
		printf(RED "failed to retrieve ./data/emails\n" DEF);
		exit(69);
	}
	int **keywordapp = calloc(10000, sizeof(int *));
	int index = 0, avg_words = 0;
	while ((entry = readdir(directory)) != NULL) {
		if (entry->d_name[0] != '.') {
			char *fullname = calloc(272, sizeof(char));
			snprintf(fullname, 272, "data/emails/%d", index);
			FILE *mail = load_mail(fullname);
			free(fullname);
			pass_to_body(mail);
			keywordapp[index] = calloc(N, sizeof(int));
			wordc[index] = parse_mail(mail, keywords, keywordapp, N, index);
			avg_words += wordc[index];
			index++;
		}
	}
	avg_words /= index;
	closedir(directory);
	FILE *out = fopen("statistics.out", "w");
	if (!out) {
		printf(RED "failed to create out file\n" DEF);
		exit(420);
	}
	double stdev, *s_stdev = calloc(N + 20, sizeof(double));
	for (int i = 0; i < N; i++) {
		int sum = 0;
		for (int j = 0; j < index; j++)
			sum += keywordapp[j][i];
		stdev = sqrt(stdev_calc(keywordapp, index, i));
		s_stdev[i] = stdev;
		if (i < init_n)
			fprintf(out, "%s %d %lf\n", keywords[i], sum, stdev);
	}
	//  end task1
	FILE *out2 = fopen("prediction.out", "w");
	if (!out2) {
		printf(RED "failed to create out file\n" DEF);
		exit(420);
	}
	SPAMMERS * sp = calloc(1000, sizeof(SPAMMERS));
	int spamindex = load_spammers(sp), s = 0;
	for (int i = 0; i < index; i++) {
		char *fullname = calloc(272, sizeof(char));
		snprintf(fullname, 272, "data/emails/%d", i);
		FILE *mail = load_mail(fullname);
		pass_to_email(mail);
		char *address = malloc(100);
		fscanf(mail, "%s", address);
		s = score_calc(mail, keywordapp, sp, address, s_stdev, spamindex,
					   i, N, (double)avg_words / (double)wordc[i]);
		free(address);
		if (s >= 45)
			fprintf(out2, "1\n");
		else
			fprintf(out2, "0\n");
		fclose(mail);
		free(fullname);
	}
	for (int i = 0; i < N; i++)
		free(keywords[i]);
	free(keywords);
	for (int i = 0; i < index; i++)
		free(keywordapp[i]);
	free(keywordapp);
	for (int i = 0; i < spamindex; i++)
		free(sp[i].email);
	free(wordc); free(sp); fclose(out); fclose(out2); free(s_stdev);
	printf("%f\n", (float)(clock() - t0) / CLOCKS_PER_SEC);
	return 0;
}
