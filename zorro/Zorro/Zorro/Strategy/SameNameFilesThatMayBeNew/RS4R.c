// Relative Strength in 4D  ///////////////////////
// Tested with Zorro 2.29 / P.Volkova /////////////

//#define TRADETEST
#define GOLDTEST
#define PERIOD 3

int NumIndex = 0;
var RS2(var Close,string Index)
{
   assetAdd(strf("IDX%i",NumIndex),strf("YAHOO:%s",Index)); 
	if(!asset(Asset)) return quit("Index not found!");
	if(NumIndex) NumIndex++;
	var RS2 = Close/priceClose(0);
	var Fast = EMA(series(RS2),10*PERIOD);
	var Med = SMA(series(Fast),7*PERIOD);
	var Slow = SMA(series(Fast),15*PERIOD); // error in description?
	var VSlow = SMA(series(Slow),30*PERIOD);
/*	plot("Fast",Fast,NEW,ORANGE);
	plot("Med",Med,0,GREEN);
	plot("Slow",Slow,0,RED);
	plot("VSlow",VSlow,0,BLUE);*/
	if(Fast >= Med && Med >= Slow && Slow >= VSlow)
		return 10;
	else if(Fast >= Med && Med >= Slow && Slow < VSlow)
		return 9;
	else if(Fast < Med && Med >= Slow && Slow >= VSlow)
		return 9;
	else if(Fast < Med && Med >= Slow && Slow < VSlow)
		return 5;
	else
		return 0;
}

var RS4(var Close)
{
	NumIndex = 1;
	var RS3 = RS2(Close,"FXAIX") // SPX 500
		+ RS2(Close,"FNCMX") // Nasdaq
		+ RS2(Close,"FSMDX") // Mid Cap
		+ RS2(Close,"FSSNX") // Small Cap
		+ RS2(Close,"FSPGX") // Large Growth
		+ RS2(Close,"FLCOX") // Large Val
		+ RS2(Close,"FPADX") // Emerging Market
		+ RS2(Close,"FSRNX") // Real Estate
		+ RS2(Close,"FSAGX") // Gold
		+ RS2(Close,"FSUTX") // Utilities
		+ RS2(Close,"FTBFX"); // Total Bond
	return 10*RS3/11; 
}

function run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"RS4Osc",32+64+128+7,BLACK);  
   set(PLOTNOW);
   BarPeriod = 1440;
   EndDate = 20200415;
	LookBack = 200;
#ifdef CHINATEST	
   StartDate = 20170315;
	assetAdd("China","YAHOO:FHKCX");
	asset("China");
	assetSelect();
	RS2(priceClose(0),"FXAIX");
	PlotHeight2 = PlotHeight1; 
#endif
#ifdef GOLDTEST	
   StartDate = 20181201;
	assetAdd("Gold","YAHOO:FSAGX");
	asset("Gold");
	assetSelect();
	var RS4Osc = SMA(series(RS4(priceClose(0))),3*PERIOD);
	var Mv = SMA(series(RS4Osc),5*PERIOD);
	plot("RS4Osc",RS4Osc,NEW|LINE,BLUE);
	plot("Mv",Mv,LINE,RED);
#endif
#ifdef TRADETEST
   StartDate = 2012;
	EndDate = 2020;
	Capital = 10000;
	set(LOGFILE);
	var Weights[10],Strengths[10];
	string Stock,Stocks[10];
	assetList("assetsIB");
	while(Stock = loop("AAPL","MSFT","XOM","AMZN","JNJ","FB","GE","TSLA","JPM","PG"))
	{
		if(Init) assetHistory(Stock,FROM_STOOQ);
		asset(Stock);
		Stocks[Itor1] = Stock;
		//Strengths[Itor1] = RET(30*PERIOD);
		Strengths[Itor1] = RS4(priceClose(0));
	}
	if(month(0) != month(1))
	{
		distribute(Weights,Strengths,10,4,0);
		int i;
		for(i = 0; i<10; i++) {
			asset(Stocks[i]);
			int NewShares = Weights[i]*Balance/priceClose(0) - LotsPool;
			if(NewShares > 0)
				enterLong(NewShares);
			else if(NewShares < 0)
				exitLong("",-NewShares);
		}
	}	
#endif
}










