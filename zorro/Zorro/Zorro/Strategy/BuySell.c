// Buy & Sell Strategy ////////////////////////////////////
// Traders Tips 6/21
////////////////////////////////////////////////////

// User definitions ////////////////////////////////

#define ATR_PERIOD		30
#define POISE_FLOOR		15
#define PANIC_CEILING 	18
#define TIME_DECISIONS	30
#define INVEST 			1000

#define STARTDATE	20140101  // Backtest start 
#define ENDDATE	20161231  // Backtest end 


var StochK(int FastPeriod,int SlowPeriod)
{
	StochF(FastPeriod,SlowPeriod,MAType_SMA);
	return rFastD;
}

function run()
{
	if(!require(-2.35)) return;
	StartDate = 20101231; 
	EndDate = 20201231;
	BarPeriod = 7*1440; // 1 week
	LookBack = 100;
	Capital = 100000;
	
	set(PARAMETERS,TESTNOW,PLOTNOW);
	//set(PLOTNOW);
	setf(PlotMode,PL_ALL);
	PlotScale = 8;

	assetList("AssetsIB");
	History = "*.t6";
	assetAdd("VIX","YAHOO:^VIX");
	asset("VIX");
	var VIXdn = (price(0)/HH(15,1)-1)*100;
	var VIXup = (price(0)/LL(15,1)-1)*100;
	var VIXDnMin = -optimize(30,20,40,10);
	var VIXUpMin = optimize(100,80,120,10);
	plot("VIX",price(0),NEW|LINE,BLACK);

	History = "*un.t6"; // for unadjusted data
	if(Init) assetHistory("SPY",FROM_STOOQ|UNADJUSTED);
	asset("SPY");
	vars SPYs = series(priceClose());
	vars Vols = series(marketVol()*5/1000000); // Mio units
	vars ATRs = series(ATR(2));
	var ATRDn = (ATRs[0]/MaxVal(ATRs+1,15)-1)*100;
	var ATRUp = (ATRs[0]/MinVal(ATRs+1,15)-1)*100;
	var Up = (HH(2)/LL(4)-1)*100;
	var Dn = (LL(2)/HH(4)-1)*100;
	var Cch = (LL(10)/HH(100)-1)*100;
	vars StochK14s = series(StochK(14,3));
	vars StochK40s = series(StochK(40,3));
	int HHb = MaxIndex(SPYs,4);
	int LLb = MinIndex(SPYs,4);
	var VOLUp = (Vols[0]/SMA(Vols+HHb+1,50)-1)*100;
	
	int BuyInitial = once(
		SMA(SPYs,10) >= SMA(SPYs+1,10) and 
		StochK(3,3) < 40);
	int Buy = Up > 6 and 
		(VIXdn < VIXDnMin or ATRDn < 2*VIXDnMin) and
		Cch < -15 and
		MinVal(StochK40s,LLb) < 25 and
		MinVal(StochK14s,LLb) < 25 and
		StochK14s[0] >= StochK40s[0];
	int Sell = SPYs[0] < SMA(SPYs,20) and 
		Dn < -6 and
		(VIXup > VIXUpMin or ATRUp > 2*VIXUpMin) and
		VOLUp > 80 and
		MaxVal(StochK40s,HHb) > 85 and
		MaxVal(StochK14s,HHb) > 85 and 
		StochK14s[0] <= StochK40s[0];

//plot tiny triangles above / below price for the buy and sell signals
	if(Buy) plot("Buy",0.9*priceLow(),MAIN|TRIANGLE,GREEN);
	if(Sell) plot("Sell",1.1*priceHigh(),MAIN|TRIANGLE4,RED);
	plot("Volume",Vols,BARS|NEW,BLUE);
	plot("ATR(2)",ATRs,LINE|NEW,RED);
		
	MaxLong = 1;
	Leverage = 1;
	Margin = Equity; // invest all money
	Fill = 3; // enter/exit at open
	if(Sell)
		exitLong();
	else if(BuyInitial or Buy)
		enterLong();
}
