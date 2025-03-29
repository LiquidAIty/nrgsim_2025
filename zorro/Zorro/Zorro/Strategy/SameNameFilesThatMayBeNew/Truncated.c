// Truncated Indicators ////////////////
// Zorro 2.27+

//#define DO_CHIRP
#define DO_PLOT

var indicator(vars Data,int Period,var Param);

var truncate(function F,vars Data,int Period,var Param)
{
	indicator = F;
	var *Trunc = zalloc(UnstablePeriod*sizeof(var));
	var Ex;
	int i;
	for(i = UnstablePeriod-1; i >= 0; i--) {
		SeriesBuffer = Trunc;
		Ex = indicator(Data+i,Period,Param);
		shift(Trunc,0,UnstablePeriod);
	}
	return Ex;
}

#ifdef DO_CHIRP
function run()
{
	BarPeriod = 60;
	MaxBars = 800;
	LookBack = 100;
	asset(""); // dummy asset
	ColorUp = ColorDn = 0; // don't plot a price curve
	PlotHeight1 = 300;
	PlotHeight2 = 200;
	set(PLOTNOW);
	
	vars Chirp = series(genSine(3,60));
	int Color = BLACK;
	UnstablePeriod = 10;
	plot("Chirp",Chirp[0],0,Color);
	plot("BandPass,0.1",BandPass(Chirp,30,0.1),NEW,Color);
	plot("Truncated",truncate(BandPass,Chirp,30,0.1),NEW,Color);
	plot("BandPass,0.5",BandPass(Chirp,30,0.5),NEW,Color);
}
#endif

#ifdef DO_PLOT
function run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"Trunc",32+64+128+7,BLACK); 
   set(PARAMETERS,PLOTNOW);
   BarPeriod = 1440;
   StartDate = 20181101;
	PlotHeight2 = PlotHeight1;

   assetAdd("SPY","STOOQ:SPY.US"); // load price history from Stooq
   asset("SPY");
   
	UnstablePeriod = 10;
   vars Prices = series(priceClose());
	var Trunc = truncate(BandPass,Prices,20,0.1);
   plot("Bandpass,0.1",BandPass(Prices,20,0.1),NEW|LINE,RED);
   plot("Bandpass,0.5",BandPass(Prices,20,0.5),LINE,GREEN);
   plot("Truncated",Trunc,LINE,BLUE);
}
#endif
