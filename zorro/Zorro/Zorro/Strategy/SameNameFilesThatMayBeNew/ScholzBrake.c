// Scholz Brake Test ///////////////////

function run()
{
	BarPeriod = 60;
	LookBack = 300;
	require(2.23);
	
	asset("EUR/USD");
	set(LOGFILE,PLOTNOW); // log all trades
	if(is(INITRUN) || year(0) != year(1)) // any new year
		ScholzBrake = 10000; // reset the Scholz Brake

	vars Prices = series(priceClose());
	vars Trends = series(LowPass(Prices,300));
	Lots = 10;

	if(valley(Trends))
		enterLong();
	else if(peak(Trends))
		enterShort();	
}