// Trend Persistence Range Indicator ////////////////
// Zorro 2.33+

//#define DO_TRADE
//#define OOS // out-of-sample

var TPR(vars Data,var Threshold,int TPRPeriod,int SMAPeriod)
{
	vars SMAs = series(SMA(Data,SMAPeriod));
	int i, CtrP=0, CtrM = 0;
	for(i=0; i<TPRPeriod; i++) {
		var Slope = SMAs[i]-SMAs[i+1];
		if(Slope > Threshold) CtrP++;
		else if(Slope < -Threshold) CtrM++;
	}
	return 100. * abs(CtrP-CtrM)/TPRPeriod;
}
	
void run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,35,"Trend Filters",32+64+128+7,BLACK); 
   set(PLOTNOW|LOGFILE);
	PlotHeight2 = 0.5*PlotHeight1;
   BarPeriod = 60;
#ifdef DO_TRADE
   StartDate = 2010;
   EndDate = 2020;
	LookBack = 2500; 
//	Spread = Commission = RollLong = RollShort = Slippage = 0; // no trading cost
#else
   StartDate = 20200602;
   EndDate = 20200605;
#endif
   asset("EUR/USD");
	
#ifdef DO_TRADE
#ifdef OOS
	NumWFOCycles = 10;
	DataSplit = 75;
	NumCores = -2;
#endif
	while(algo(loop("None","TPR","CCY")))
	{
		set(PARAMETERS);
		int Period = 2500; //optimize(1500,500,3000,250);
		vars Smas = series(SMA(seriesC(),Period));
		int Color = BLACK;
		bool Filter = 1;
		if(Algo == "TPR") {
			Period = optimize(15,10,20,1);
			var Threshold = optimize(10,0,20,2);
			var Tpr = TPR(seriesC(),1*PIP,Period,5);
			Filter = Tpr > Threshold;
			Color = BLUE;
		} else if(Algo == "CCY") {
			Period = optimize(15,10,30,5);
			var Threshold = optimize(10,0,40,5);
			var State = CCYIState(seriesC(),Period,Threshold);
			Filter = State != 0;
			Color = PURPLE;
		}

		if(Filter && crossOver(Smas,seriesC()))
			enterLong();
		else if(Filter && crossUnder(Smas,seriesC()))
			enterShort();

		plot(Algo,ProfitClosed+ProfitOpen,MAIN|LINE|AXIS2,Color);
	}
#else
	var Tpr = TPR(seriesC(),1*PIP,15,5);
	var TprSmooth = EMA(series(Tpr),5);
   plot("TPR",TprSmooth,NEW,RED);	
#endif	
}
