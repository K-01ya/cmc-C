#include <stdio.h>

int is_prime(int n)
{
	for (int i = 2; i < n; i++)
		if ((n % i) == 0)
			return 0;
	return n;
}

void primes(int n)
{
	int pr;

	for (int i = 2; i < n; i++)
	{
		pr = is_prime(i);
		if (pr)
			printf("%d ", pr);
	}	
}

int main()
{
	int n;
	scanf("%d", &n);
	primes(n);
	return 0;
}
