///////////////////////////////////
// Pi calculator
///////////////////////////////////

int main()
{
	printf("\n");
	int i, a=10000, c=14000, e=0, f[16000], n=0;
	for(i=0; i<c; i++) 
		f[i] = a/5; 
	for(; c>0; c-=14)
	{
		int d = 0, b = c*2, i = c;
		for(;;)
		{
			d += f[i]*a;
			b--;
			f[i] = d%b;
			d /= b;
			b--;
			if (--i == 0) break;
			d*=i;
		}
		printf("%.4d",e+d/a);
		e = d%a;
		if ((++n)%10 == 0) printf("\n");
	}
	printf("\n%d digits of pi calculated!",n*4);
}

 
