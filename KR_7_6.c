#include <stdio.h>
#define MAXLINE 100


int compare (char *str1, char *str2)
{
	for (int i = 0; (str1[i] != '\0') && (str2[i] != '\0'); i++)
	{
		if (str1[i] != str2[i]){
			return 1;}
	}
	return 0;

}

int main()
{
	FILE *f1, *f2;
	char l1[MAXLINE], l2[MAXLINE];

	f1 = fopen("fin1.txt", "r");
	f2 = fopen("fin2.txt", "r");
	
	while ((fgets(l1, MAXLINE, f1) != NULL) && (fgets(l2, MAXLINE, f2) != NULL))
	{
		if (compare(l1, l2)){
			printf("first file: %ssecond file: %s", l1, l2);
			return 0;
		}
	}
	printf("They are same\n");
	return 0;
}
