// MAD indicator ///////////////////////////////////////
// Traders Tips 10/21
////////////////////////////////////////////////////
//#define DO_SINE

var MAD(vars Data,int ShortPeriod,int LongPeriod)
{ 
	return 100*(SMA(Data,ShortPeriod)/SMA(Data,LongPeriod)-1.);
}


#ifdef DO_SINE
function run()
{
   set(PLOTNOW);
   MaxBars = 200;
   LookBack = 40;
	BarPeriod = 60;
   asset(""); // dummy asset
   ColorUp = ColorDn = 0; // don't plot a price curve
   
   vars Sine = series(genSine(40,40));
	var Diff = SMA(Sine,5) - SMA(Sine,25); // 15 bars difference
   plot("Sine",Sine[0]-0.5,LINE,BLUE);
   plot("MAD",Diff,LINE,RED);
}
#else
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
}
#endif

