// Open/Close Average //////////////////////////////
// Traders Tips 3/23
////////////////////////////////////////////////////

function run()
{
	BarPeriod = 15; 
	StartDate = 20220629;
	EndDate = 20220712;
	set(PLOTNOW);
	asset("SPX500");

	vars OC = series((priceO()+priceC())/2);
	plot("RSI(Close)",RSI(seriesC(),14),NEW,RED);
	plot("RSI(OC)",RSI(OC,14),0,BLUE);
	
	// vars RSIs = series(RSI(OC,14));
	// if(crossUnder(RSIs,70))
		// enterShort();
	// if(crossOver(RSIs,30))
		// enterLong();
}
