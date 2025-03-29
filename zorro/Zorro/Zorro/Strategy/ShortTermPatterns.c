// Short-Term Patterns ////////////////
// Zorro 2.32+

// a higher high followed by a lower close. 
var cdlKeyReversal()
{
	if(priceHigh(0) > priceHigh(1) && priceLow(0) < priceLow(1))
	{
		if(priceClose(0) < priceLow(1)) return -100; // sell
		if(priceClose(0) > priceHigh(1)) return 100; // buy
	} 
	return 0;
}


// recent 3 days each have a true range smaller than the 4th previous day.
var cdl3DayCompression()
{
	vars TRs = series(TrueRange(),4);
	if(TRs[0] < TRs[3] && TRs[1] < TRs[3] && TRs[2] < TRs[3])
		return 100;
	else
		return 0;
}

// a gap higher followed by a lower close, but not filling the gap. 
var cdlIslandReversal()
{
	if(priceLow(0) > priceHigh(1) && priceClose(0) < priceOpen(0)) 
		return -100; // sell
	if(priceLow(1) > priceHigh(0) && priceClose(0) > priceOpen(0)) 
		return 100; // buy
	else return 0;
}

// a higher high and a lower low, close in upper or lower 25% of the range.
var cdlOutsideDay()
{
	if(priceHigh(0) > priceHigh(1) && priceLow(0) < priceLow(1))
	{
		if(priceClose(0) < 0.75*priceLow(0) + 0.25*priceHigh(0)) 
			return -100; // sell
		if(priceClose(0) > 0.25*priceLow(0) + 0.75*priceHigh(0)) 
			return 100; // buy
	} 
	return 0;	
}

//  same as outside days, but true range must exceed 1.5 × 20-day ATR
var cdlWideRangeDays()
{
	if(TrueRange() > 1.5*ATR(20))
		return cdlOutsideDay();
	else
		return 0;
}

// gap must be larger than 0.5 × 20-day ATR.
var cdlGapOpening()
{
	var Ratio = (priceOpen(0) - priceClose(1))/ATR(20);
	if(Ratio >= 0.5) return 100;
	if(Ratio <= -0.5) return -100;
	return 0;	
}

// just enter a trade when you feel like it
var cdlTotalRandom()
{
	int Pattern = random(100);
	if(Pattern > 90) return 100;
	if(Pattern < 10) return -100;
	return 0;
}

var pattern();

function run() 
{
	pattern = cdlKeyReversal;
	bool WithTrend = 0;
	string Name = "KeyRev";
	
	//set(LOGFILE);
	require(2.32);
   BarPeriod = 1440;
   StartDate = 20100101; // TSLA went public in 2010
	EndDate = 2020; // test until 2020
	var Investment = 10000;
	TradesPerBar = 40; // 8 assets * 5 algos

	char File[100];
	if(is(EXITRUN)) {
		sprintf(File,"Log\\%s.htm",strf("%s%s",
			Name,ifelse(WithTrend,"WithTrend","NoTrend")));
		printf("\nStoring results in %s",File);
		file_write(File,strf("<p>%s:</p>\n<table><tr>",File),0);
// write the table header
		file_append(File,strf("<td>Asset</td>"),0);
		file_append(File,strf("<td>Long/Short</td>"),0);
		file_append(File,strf("<td>Day 1</td>"),0);
		file_append(File,strf("<td>Day 2</td>"),0);
		file_append(File,strf("<td>Day 3</td>"),0);
		file_append(File,strf("<td>Day 4</td>"),0);
		file_append(File,strf("<td>Day 5</td></tr>"),0);
	}

   assetAdd("SPY","STOOQ:*.US"); // load price history from Stooq
   assetAdd("QQQ","STOOQ:*.US"); 
   assetAdd("IWM","STOOQ:*.US"); 
   assetAdd("AAPL","STOOQ:*.US"); 
   assetAdd("AMZN","STOOQ:*.US"); 
   assetAdd("GE","STOOQ:*.US"); 
   assetAdd("WMT","STOOQ:*.US"); 
   assetAdd("TSLA","STOOQ:*.US"); 
	
	while(asset(loop("SPY","QQQ","IWM","AAPL","AMZN","GE","WMT","TSLA"))) {
	vars Trend = series(SMA(seriesC(),80));
	while(algo(loop("Day1","Day2","Day3","Day4","Day5"))) {
		
	Lots = Investment/priceClose();
	LifeTime = Itor2+1; // life time in days
	int Signal = pattern();
	if(Signal > 0 && (!WithTrend || rising(Trend)))
		enterLong();
	else if(Signal < 0  && (!WithTrend || falling(Trend)))
		enterShort();
	if(is(EXITRUN)) {
		if(Algo == "Day1") // first loop run
			file_append(File,strf("\n<tr><td>%s</td><td>%i/%i</td>",
				Asset,NumWinLong+NumLossLong,NumWinShort+NumLossShort),0);
		string Color = ifelse(WinLong-LossLong+WinShort-LossShort > 0,
			"bgcolor=\"lime\"","bgcolor=\"red\"");
		file_append(File,strf("<td %s>%.0f/%.0f</td>",
			Color,WinLong-LossLong,WinShort-LossShort),0);
		if(Algo == "Day5")
			file_append(File,"</tr>",0);
	}

	}} // loops
	if(is(EXITRUN)) {
		file_append(File,"</table>",0);
		file_append(File,strf("<p>Total Profit: %.0f (%.0f%%)</p>",
			WinTotal-LossTotal,100*(WinTotal-LossTotal)/Investment),0);
	}
}
