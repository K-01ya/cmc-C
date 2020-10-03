#include <stdio.h>
#include <stdlib.h>

char *arr1 = NULL, *arr2 = NULL;
int main()
{
	int ch, n1 = 0, n2 = 0, size = 0, i;
	while ((ch = getchar()) != EOF){
		while (ch != '\n'){
			if (n1 == size){
				size = size * 2 + 1;
				arr1 = realloc(arr1, size);
			}
			arr1[n1] = ch;
			n1 += 1;
			ch = getchar();
		}
		if (n2 < n1){
			free(arr2);
			arr2 = arr1;
			n2 = n1;
		}
		arr1 = malloc(0);
		n1 = 0;
		size = 0;
	}

	printf("%d\n", n2);

	for (i = 0; i < n2; i++)
		putchar(arr2[i]);
	free(arr1);
	free(arr2);
	printf("\n");
	return 0;
}
