double logsum(int n)
{
	if(n <= 1) return 0.;
	else return log((double)n)+logsum(n-1);
}

int cbi_count(double* Data,int TestTime,double DDDepth,int DDLength)
{
	int S = TestTime-DDLength+1;
	int i, N=0; 
	for(i=0; i<S; i++)
		if(Data[i+DDLength-1]-Data[i] <= -DDDepth) N++;
	return N;
}

double cbi(double* Data,int TestTime,int LiveTime,double DDDepth,int DDLength,int Repeats)
{
	int T = LiveTime-DDLength+1;
	int S = (TestTime-DDLength+1) * Repeats;
	int N = cbi_count(Data,TestTime,DDDepth,DDLength); 
	double* Shuffled = (double*)malloc(TestTime*sizeof(double));
	int i; 
	for(i=0; i<Repeats-1; i++) {
		randomize(SHUFFLE,Shuffled,Data,TestTime);
		N += cbi_count(Shuffled,TestTime,DDDepth,DDLength);
	}
	free(Shuffled);
	if(LiveTime > DDLength)
		return 1.-exp(logsum(S-N)+logsum(S-T)-logsum(S)-logsum(S-N-T));
	else
		return ((double)N)/S;
}

// cbi test program
void main()
{
	seed(365); // for randomize()
	double Data[10] = { 1,2,3,4,3,2,1,2,3,4 };
	printf("\nCBI: %.3f",cbi(Data,10,8,2,5,50));
}