// AM/FM Decoder ////////////////
// Zorro 2.35+

var AM(var Signal,int Period)
{
	var Envel = MaxVal(series(abs(Signal),4),4);
	return SMA(series(Envel),Period);
}

var FM(var Signal,int Period)
{
	var HL = clamp(10*Signal,-1,1);
	return Smooth(series(HL,2),Period);
}

	
void run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"AM/FM (SPY)",32+64+128+7,BLACK);  
   set(PLOTNOW);
   BarPeriod = 1440;
   StartDate = 20180701;
   EndDate = 20201201;
	//History = "*x.t6";

   assetAdd("SPY","STOOQ:*"); // load price history from Stooq
   asset("SPY");

	var Deriv = priceClose()-priceOpen();
   plot("Deriv",Deriv,NEW,BLACK);
   plot("AM",AM(Deriv,8),NEW,RED);
   plot("FM",FM(Deriv,30),NEW,BLUE);
}
