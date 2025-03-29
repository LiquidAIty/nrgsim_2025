// Windowing ///////////////////////////////////////
// Traders Tips 9/21
////////////////////////////////////////////////////
//#define DO_ROC

var ROC_SMA(vars Data,int Length)
{ 
	vars Filt = series(SMA(Data,Length),2);
	return Length/(2*PI)*(Filt[0]-Filt[1]);
}

vars triangle(vars Data,int Length)
{
	vars Out = series();
	int i; 
	for(i=0; i<Length; i++)
		Out[i] = Data[i] * ifelse(i<Length/2,i+1,Length-i);
	return Out; 
}

vars hamming(vars Data,int Length,var Pedestal)
{
	vars Out = series();
	int i;
	for(i=0; i<Length; i++)
		Out[i] = Data[i] * sin(Pedestal+(PI-2*Pedestal)*(i+1)/(Length-1)); 
	return Out;
}

vars hann(vars Data,int Length)
{
	vars Out = series();
	int i;
	for(i=0; i<Length; i++)
		Out[i] = Data[i] * (1-cos(2*PI*(i+1)/(Length+1)));
	return Out;
}

void run()
{
	StartDate = 20191101; 
	EndDate = 20210101;
	BarPeriod = 1440; 
	
	set(PLOTNOW);
	PlotHeight1 = 1.5*PlotHeight2;
	History = "*d.t6";
	assetAdd("SPY","STOOQ:*");
	asset("SPY");
	
	vars Deriv = series(priceClose() - priceOpen());
#ifdef DO_ROC
	plot("ROC_SMA",ROC_SMA(Deriv,20),NEW,RED);
	plot("Triangle",ROC_SMA(triangle(Deriv,20),20),NEW,RED);
	plot("Hamming",ROC_SMA(hamming(Deriv,20,10*PI/360),20),NEW,RED);
	plot("Hann",ROC_SMA(hann(Deriv,20),20),NEW,RED);
#else
	plot("FIR_SMA",SMA(Deriv,20),NEW,RED);
	plot("Triangle",SMA(triangle(Deriv,20),20),NEW,RED);
	plot("Hamming",SMA(hamming(Deriv,20,10*PI/360),20),NEW,RED);
	plot("Hann",SMA(hann(Deriv,20),20),NEW,RED);
#endif
}
