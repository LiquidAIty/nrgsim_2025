// Supertrend Indicator /////////////
// Traders Tips 7/2023
////////////////////////////////////////////////////

#define DO_TRADE

var STI(int Periods,var Mult)
{
	var Dis = Mult * ATR(Periods);
   var Top = MedPrice() + Dis;
	var Bot = MedPrice() - Dis;
	vars Tops = series(Top,2),
		Bots = series(Bot,2),
		Stis = series(priceC(0),2);
	Tops[0] = ifelse(Top < Tops[1] || priceC(1) > Tops[1],Top,Tops[1]);
	Bots[0] = ifelse(Bot > Bots[1] || priceC(1) < Bots[1],Bot,Bots[1]);
	return Stis[0] = ifelse(Stis[1] == Tops[1], 
		ifelse(priceC(0) <= Tops[0],Tops[0],Bots[0]),
		ifelse(Stis[1] == Bots[1], 
			ifelse(priceC(0) >= Bots[0],Bots[0],Tops[0]),Tops[0]));
}

function run()
{
	set(PLOTNOW,LOGFILE);
	BarPeriod = 1440; 
	assetAdd("GE","STOOQ:*");
#ifdef DO_TRADE
	StartDate = 2010; 
	EndDate = 2023;
	asset("GE");	
	vars Signals1 = series(STI(10,3));
	vars Signals2 = series(STI(10,2));
	if(crossOver(Signals2,Signals1))
		enterLong();
	if(crossUnder(Signals2,Signals1))
		enterShort();
#else
	StartDate = 20220601; 
	EndDate = 20221101;
	asset("GE");	
	plot("STI",STI(10,3),LINE,RED);
	plot("STI Fast",STI(10,2),LINE,BLUE);
	//plot("EMA",EMA(seriesC(),200),0,RED);
#endif	
}
