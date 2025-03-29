// Elegant oscillator ///////////////////////////////////////
// Traders Tips 2/22
////////////////////////////////////////////////////

//#define DO_TRADE

var SumSq(vars Data,int Length)
{
	var Ex = 0;
	int i;
	for(i=0; i<Length; i++)
		Ex += Data[i]*Data[i];
	return Ex;
}

var EO(vars Data,int Length)
{ 
	vars Derivs = series(priceClose(0)-priceClose(2));
	var RMS = sqrt(SumSq(Derivs,Length)/Length);
	var NDeriv = Derivs[0]/RMS;
	vars IFishs = series(FisherInv(&NDeriv));
	return Smooth(IFishs,20);
}

var Fish(vars Data,int Length)
{ 
	vars Derivs = series(priceClose(0)-priceClose(2));
	var RMS = sqrt(SumSq(Derivs,Length)/Length);
	var NDeriv = Derivs[0]/RMS;
	vars IFishs = series(FisherInv(&NDeriv));
	return FIR6(IFishs);
}

var HardClip(vars Data,int Length)
{ 
	vars Derivs = series(priceClose(0)-priceClose(2));
	var RMS = sqrt(SumSq(Derivs,Length)/Length);
	vars Clips = series(clamp(Derivs[0],-1,1));
	return FIR6(Clips);
}

void run()
{
	StartDate = 20200301; 
	EndDate = 20210501;
	BarPeriod = 1440; 
	PlotHeight2 = PlotHeight1;
	
	set(PLOTNOW);
	assetAdd("SPY","STOOQ:*");
	asset("SPY");
	
#ifdef DO_TRADE	
	vars Signals = series(EO(seriesC(),50));
	//vars Signals = series(Fish(seriesC(),50));
	var Threshold = 0.5;
	//MaxLong = MaxShort = 1;
	
	if(Signals[0] > Threshold && peak(Signals))
		enterShort();
	else if(Signals[0] < -Threshold && valley(Signals))
		enterLong();
#else	
	plot("EO",EO(seriesC(),50),NEW|LINE,RED);
	plot("0",0,0,BLACK);
	plot("IFish",Fish(seriesC(),50),NEW|LINE,RED);
	plot("Clip",HardClip(seriesC(),50),LINE,BLUE);
	plot("Ref",0,0,BLACK);
#endif
}
