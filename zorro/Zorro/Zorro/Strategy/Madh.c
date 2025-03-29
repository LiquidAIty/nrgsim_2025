// MADH indicator ///////////////////////////////////////
// Traders Tips 11/21
////////////////////////////////////////////////////
//#define DO_TRADE

vars hann(vars Data,int Length)
{
	vars Out = series();
	int i;
	for(i=0; i<Length; i++)
		Out[i] = Data[i] * (1-cos(2*PI*(i+1)/(Length+1)));
	return Out;
}

var MAD(vars Data,int ShortPeriod,int LongPeriod)
{ 
	return 100*(SMA(Data,ShortPeriod)/SMA(Data,LongPeriod)-1.);
}

var MADH(vars Data,int ShortPeriod,int LongPeriod)
{ 
	return 100*(SMA(hann(Data,ShortPeriod),ShortPeriod)/SMA(hann(Data,LongPeriod),LongPeriod)-1.);
}

void run()
{
	StartDate = 20191201; 
	EndDate = 20210701;
	BarPeriod = 1440; 
	PlotHeight2 = 0.8*PlotHeight1;
	
	set(PLOTNOW);
	assetAdd("SPY","STOOQ:*");
	asset("SPY");
	
	plot("MAD",MAD(seriesC(),8,23),NEW,RED);
	plot("MADH",MADH(seriesC(),8,23),NEW,BLUE);
#ifdef DO_TRADE	
	vars Signals = series(MADH(seriesC(),8,23));
	if(peak(Signals))
		enterShort();
	else if(valley(Signals))
		enterLong();
#endif	
}

