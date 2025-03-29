// Backtest Reality Check ///////////////////////
// Start in [Train] mode ////////////////////////
#include <profile.c>

#define CYCLES	1000  // comment out for simple backtests

void hold()
{
   BarPeriod = 1440;
   StartDate = 20170601;

	assetList("AssetsIB"); // simulate IB margin account 
   asset("SPY");
	
	if(!NumOpenLong)
		enterLong();
	else if(is(EXITRUN))
		exitLong();
}

void moon()
{
   BarPeriod = 1440;
   StartDate = 20170601;

   if(is(FIRSTINITRUN)) 
      plotText("#Title",60,35,"Moon (SPY)",32+64+128+7,BLACK); 
	assetList("AssetsIB"); // simulate IB margin account 
   asset("SPY");
	
	set(PARAMETERS);
	setf(TrainMode,GENETIC); 
	int MoonPhase = fmod(wdate(0),29.5306);
   int WaxingDay = optimize(1,1,14,1);
   int WaningDay = optimize(15,15,29,1);
   
	if(MoonPhase == WaxingDay)
		enterLong();
	else if(MoonPhase == WaningDay)
		enterShort();
}

void trend()
{
	set(PARAMETERS);
   BarPeriod = 1440;
   NumYears = 10;
   NumWFOCycles = 5;

   if(is(FIRSTINITRUN)) 
      plotText("#Title",60,35,"Trend (SPY)",32+64+128+7,BLACK); 
	assetList("AssetsIB"); // simulate IB margin account 
   asset("SPY");
   
   int Cutoff = optimize(10,5,30,5);
   vars Signals = series(EMA(seriesC(),Cutoff));
   
	if(valley(Signals))
		enterLong();
	else if(peak(Signals))
		enterShort();
}

function run()
{
#ifdef CYCLES	
	NumTotalCycles = CYCLES;
#endif
	if(TotalCycle > 1) 
		Detrend = SHUFFLE;
	set(PRELOAD,TESTNOW);
	trend();
	//moon();
	//hold();
}

#ifdef CYCLES	
function evaluate()
{
	static var OriginalProfit, Probability;
	var PF = ifelse(LossTotal > 0,WinTotal/LossTotal,10);
	if(TotalCycle == 1) {
		OriginalProfit = PF;
		Probability = 0;
	} else {
		if(PF < 2*OriginalProfit) // clip image
			plotHistogram("Random",PF,OriginalProfit/50,1,RED);	
		if(PF > OriginalProfit)
			Probability += 100./NumTotalCycles;
	}
	if(TotalCycle == NumTotalCycles) { // last cycle
		PlotLabels = 2;
		plotHistogram("Original",OriginalProfit,OriginalProfit/50,sqrt(NumTotalCycles),BLACK);	
		plotHistogram("",0,OriginalProfit/50,1,-1);	// plot labels
		printf("\n-------------------------------------------");
		printf("\nP-Value %.1f%%",Probability);
		if(Probability <= 1)
			printf("\nResult is significant");
		else if(Probability <= 5)
			printf("\nResult is possibly significant");
		else 
			printf("\nResult is statistically insignificant");
		printf("\n-------------------------------------------");
	} 
}
#endif