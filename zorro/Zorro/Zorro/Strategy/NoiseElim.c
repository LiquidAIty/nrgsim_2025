// Noise Elimination Technology ////////////////
// Zorro 2.30+

var MyRSI(vars Data,int Period)
{
	var CU = SumUp(Data,Period+1);
	var CD = -SumDn(Data,Period+1);
	return ifelse(CU+CD != 0,(CU-CD)/(CU+CD),0);
}
	
var NET(vars Data,int Period)
{
	int i,k;
	var Num = 0;
	for(i=1; i<Period; i++)
		for(k=0; k<i; k++)
			Num -= sign(Data[i]-Data[k]);
	var Denom = .5*Period*(Period-1);
	return Num/Denom;
}

function run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"NoiseElim",32+64+128+7,BLACK); 
   set(PLOTNOW);
	PlotHeight2 = 0.5*PlotHeight1;
   BarPeriod = 1440;
   StartDate = 20190101;

   assetAdd("SPY","STOOQ:SPY.US"); // load price history from Stooq
   asset("SPY");
   
   vars Prices = series(priceClose());
	var MyRsi = MyRSI(Prices,14);
	var Net = NET(series(MyRsi),14);
   plot("MyRSI",MyRsi,LINE|NEW,RED);
   plot("NET",Net,LINE,BLUE);
	plot("0",0,0,GREY);
}
