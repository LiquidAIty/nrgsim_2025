// Stochastic Distance Oscillator /////////////
// Traders Tips 6/2023
///////////////////////////////////////////////
//#include <profile.c>

var SDO(int LBPeriod,int Period,int Pds)
{
	var Dist = priceC(0)-priceC(Period);
	vars Dists = series(abs(Dist));
	var D = (Dists[0]-MinVal(Dists,LBPeriod))/fix0(MaxVal(Dists,LBPeriod)-MinVal(Dists,LBPeriod));
	return EMA(series(sign(Dist)*D),Pds)*100;
}

function run()
{
	set(PLOTNOW);
	BarPeriod = 1440; 
	LookBack = 200;
	History = "?.t6";
	StartDate = 20040601; 
	EndDate = 20070301;
	PlotHeight2 = 400;
	assetAdd("SPY","YAHOO:*");
	asset("SPY");
	plot("SDO",SDO(200,40,3),NEW|LINE,RED);
	plot("+40",40,0,BLUE);
	plot("-40",-40,0,BLUE);
}
