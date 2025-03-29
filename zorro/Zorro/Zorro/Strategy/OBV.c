// OBVM test script ///////////////////////

#define OBVPLOT
//#define CROSSTEST
//#define DIVTEST

var OBV(vars Data,var Volume)
{
	vars OBVs = series(Volume,2);
	if(Data[0] > Data[1])
		return OBVs[0] = OBVs[1]+Volume;
	else if(Data[0] < Data[1])
		return OBVs[0] = OBVs[1]-Volume;
	else
		return OBVs[0] = OBVs[1];
}

var OBVM(vars Prices,int Period)
{
	vars OBVData = series(OBV(Prices,marketVol()));
	return EMA(OBVData,Period);
}

var OBVMSignal(vars OBVMData,int Period)
{
	return EMA(OBVMData,Period);
}

#ifdef OBVPLOT
function run() 
{
	if(!is(SPONSORED)) { 
		quit("Need Zorro S for volume data"); return; }
	if(is(FIRSTRUN)) 
		plotText("#Title",60,25,"OBV Chart (SPX)",32+64+128+7,BLACK);	
	set(PLOTNOW);
	BarPeriod = 1440;
	StartDate = 20180215;
	EndDate = 20200215;

	assetAdd("SPX","STOOQ:^SPX"); // load price history from Stooq
	asset("SPX");

	vars Prices = series(priceClose());
	vars OBVMData = series(OBVM(Prices,7));
	vars OBVMSignals = series(OBVMSignal(OBVMData,10));
	plot("OBVM (Mio)",OBVMData[0]/1000000,AXIS2|LINE,BLUE);
	plot("OBVMSignal",OBVMSignals[0]/1000000,AXIS2|LINE,RED);
}
#else

function run() 
{
	if(!is(SPONSORED)) { quit("Need Zorro S for volume data"); return; }
	set(PARAMETERS,LOGFILE,PLOTNOW,TESTNOW);
	//setf(PlotMode,PL_BENCHMARK);
	EndDate = 20200215;
	BarPeriod = 1440;

	assetAdd("SPX","STOOQ:^SPX"); // load price history from Stooq
	assetAdd("SPY","STOOQ:SPY.US");
	asset("SPY");

	NumWFOCycles = 5; // activate walk forward analysis
	int ShortPeriod = optimize(7,3,20);
	int LongPeriod = ShortPeriod*optimize(1.3,1.1,2.0);
	
	asset("SPX");
	vars Prices = series(priceClose());
	vars OBVMData = series(OBVM(Prices,ShortPeriod));
	vars OBVMSignals = series(OBVMSignal(OBVMData,LongPeriod));

#ifdef CROSSTEST
	asset("SPY");
	if(crossOver(OBVMData,OBVMSignals))
		enterLong();
	else if(crossUnder(OBVMData,OBVMSignals))
		enterShort();
#endif
#ifdef DIVTEST
	vars Highs = series(priceHigh()), Lows = series(priceLow());
	int Div = Divergence(Highs,Lows,OBVMData,LongPeriod);
	asset("SPY");
	MaxLong = MaxShort = 1;	
	if(Div&1) // bullish divergence
		enterLong();
	else if(Div&2) // bearish divergence
		enterShort();	
#endif

}
#endif

