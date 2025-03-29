// Correlation trend indicator ///////////////////////
#include <profile.c>

//#define DO_PLOT
#define DO_PROFILE

var CTI(vars Data,int Length)
{
	int count;
	var Sx = 0, Sy = 0, Sxx = 0, Sxy = 0, Syy = 0;
	for(count = 0; count < Length; count++) {
		var X = Data[count];
		var Y = -count;
		Sx = Sx + X;
		Sy = Sy + Y;
		Sxx = Sxx + X*X;
		Sxy = Sxy + X*Y;
		Syy = Syy + Y*Y;
	}
	if(Length*Sxx - Sx*Sx > 0 && Length*Syy - Sy*Sy > 0)
		return (Length*Sxy - Sx*Sy) / sqrt((Length*Sxx - Sx*Sx)*(Length*Syy - Sy*Sy));
	else return 0;
}

#ifdef DO_PLOT
function run() 
{
	if(is(FIRSTRUN)) 
		plotText("#Title",60,25,"CTI Chart (SPY)",32+64+128+7,BLACK);	
	set(PLOTNOW);
	BarPeriod = 1440;
	LookBack = 40;
	StartDate = 20181201;

	assetAdd("SPY","STOOQ:SPY.US"); // load price history from Stooq
	asset("SPY");

	vars Prices = series(priceClose());
	plot("CTI(40)",CTI(Prices,40),NEW|LINE,BLUE);
	plot("CTI(10)",CTI(Prices,10),LINE,RED);
	plot("#0",0,LINE,BLACK);
}
#endif

#ifdef DO_PROFILE
function run() 
{
	if(is(FIRSTRUN)) 
		plotText("#Title",60,25,"Profile (SPY)",32+64+128+7,BLACK);	
	BarPeriod = 1440;
	LookBack = 40;
	StartDate = 2010;

	assetAdd("SPY","STOOQ:SPY.US"); // load price history from Stooq
	asset("SPY");

	vars Prices = series(priceClose());
	vars Signals = series(CTI(Prices,20));
	if(crossOver(Signals,0))
		plotPriceProfile(40,0);
	else if(crossUnder(Signals,0))
		plotPriceProfile(40,2);
}
#endif
