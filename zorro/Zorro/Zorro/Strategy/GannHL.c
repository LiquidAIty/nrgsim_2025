// Gann Hi-Low Indicator  ///////////////////////
// Zorro 2.30 / P.Volkova /////////////

//#define TRADETEST

var ColorGHLA;
var GHLA(int HPeriod,int LPeriod)
{
	vars H = series(priceHigh());
	vars L = series(priceLow());
	vars MaH = series(SMA(H,HPeriod),2);
	vars MaL = series(SMA(L,LPeriod),2);
	vars State = series();
	if(priceClose() > MaH[1]) State[0] = 1;
	else if(priceClose() < MaL[1]) State[0] = -1;
	else State[0] = State[1];
	ColorGHLA = State[1];
	return ifelse(State[0] < 0,MaH[0],MaL[0]);
}

var DMI(int Period)
{
	return PlusDI(Period) - MinusDI(Period);
}

var SMI(int DLength,int KLength)
{
	var Hi = HH(KLength), Lo = LL(KLength);
	var Diff = Hi-Lo;
	var RelDiff = priceClose()-(Hi+Lo)/2;
	var AvgRel = EMA(EMA(RelDiff,DLength),DLength);
	var AvgDiff = EMA(EMA(Diff,DLength),DLength);
	return ifelse(AvgDiff!=0,AvgRel/AvgDiff*200,0);
}

function run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"GannHL",32+64+128+7,BLACK);  
   set(PLOTNOW);
   BarPeriod = 1440;
   StartDate = 20150201;
   EndDate = 20200410;
	assetAdd("NVDA","STOOQ:NVDA.US");
	asset("NVDA");
	var Ghla = GHLA(3,3);
	var Smi = SMI(3,8);
	var Dmi = DMI(10);
	plot("GHLA",Ghla,LINE,BLUE);
	plot("DMI",Dmi,NEW|BARS,BLUE);
	plot("SMI",Smi,NEW|LINE,BLUE);
	plot("Zero",0,0,BLACK);
#ifdef TRADETEST
	MaxLong = MaxShort = 1;
	if(ColorGHLA > 0 && Dmi > 0 && Smi > 0)
		enterLong();
	if(ColorGHLA < 0 && Dmi < 0 && Smi < 0)
		enterShort();
#endif
}










