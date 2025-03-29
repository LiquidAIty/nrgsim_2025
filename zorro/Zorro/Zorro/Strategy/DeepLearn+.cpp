// Deep Learning Test ///////////////////////////////////////////

//#define DO_SIGNALS  // generate sample set in Train mode

///////////////////////////////////////////////////////////////////////
#include <zorro.h>
#include <pynet.cpp>

//TODO: Nur Short?

var change(int n)
{
	return scale((priceC(0) - priceC(n))/priceClose(0),100)/100;
}

var range(int n)
{
	return scale((HH(n,0) - LL(n,0))/priceC(0),100)/100;
}

///////////////////////////////////////////////////////////////////////

DLLFUNC void run()
{
	//NumCores = -1;
	if(Init) print(TO_WINDOW,"\nPython and PyTorch required"); 
	Script = "DeepLearnTorch";
	StartDate = 2023;
	BarPeriod = 60;	// 1 hour
	LookBack = 100;

	WFOPeriod = 252*24; // 1 year
	DataSplit = 95;

	assetList("AssetsFix");
	asset("EUR/USD");
	set(RULES);
	Spread = RollLong = RollShort = Commission = Slippage = 0;
	LifeTime = 3;
	if(Train) Hedge = 2;
	
///////////////////////////////////////////////////////////	
	var Signals[8] = {
		change(1),change(2),change(3),change(4),
		range(1),range(2),range(3),range(4) };	
	var Threshold = 0.5;
		
#ifdef DO_SIGNALS
	SelectWFO = -1; // use the last WFO cycle for calibrating the neural net
	var vLong = adviseLong(SIGNALS+BALANCED+RETURNS,0,Signals,8);
#else
	set(LOGFILE|PLOTNOW);
	var vLong = adviseLong(NEURAL+BALANCED+RETURNS,0,Signals,8);
	var vShort = adviseShort(NEURAL+BALANCED+RETURNS,0,Signals,8);
#endif
	if(vLong > Threshold) enterLong();
	plot("Long",vLong,NEW|LINE,BLACK);
#ifndef DO_SIGNALS
	if(vShort > Threshold) enterShort();
	plot("Short",vShort,LINE,GREY);
#endif
}
