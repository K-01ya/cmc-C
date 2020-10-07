#include <stdio.h>

int is_prime(int n)
{
	for (int i = 2; i < n; i++)
		if ((n % i) == 0)
			return 0;
	return n;
}

void fibprimes(int n)
{
	int a = 1, b = 2, t, pr;

	while (b < n)
	{
		pr = is_prime(b);
		if (pr)
			printf("%d ", pr);
		t = b;
		b += a;
		a = t;
	}	
}

int main()
{
	int n;
	scanf("%d", &n);
	fibprimes(n);
	printf("\n");
	return 0;
}
