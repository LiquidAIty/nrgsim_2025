// Reflex / TrendFlex indicators S&C 2020/02 ///////////////////////

#define TEST

var ReFlex(vars Data,int Period)
{
	var a1,b1,c1,c2,c3,Slope,DSum;
	vars Filt = series(Data[0]), MS = series(0);
	int i;
	
//Gently smooth the data in a SuperSmoother
	a1 = exp(-1.414*2*PI/Period);
	b1 = 2*a1*cos(1.414*PI/Period);
	c2 = b1;
	c3 = -a1*a1;
	c1 = 1-c2-c3;
	Filt[0] = c1*(Data[0]+Data[1])/2 + c2*Filt[1] + c3*Filt[2];

//Period is assumed cycle period
	Slope = (Filt[Period]-Filt[0]) / Period;

//Sum the differences
	for(i=1,DSum=0; i<=Period; i++)
		DSum += (Filt[0] + i*Slope) - Filt[i];
	DSum /= Period;
	
//Normalize in terms of Standard Deviations
	MS[0] = .04*DSum*DSum + .96*MS[1];
	if(MS[0] > 0.) return DSum/sqrt(MS[0]);
	else return 0.;
}
	
var TrendFlex(vars Data,int Period)
{
	var a1,b1,c1,c2,c3,Slope,DSum;
	vars Filt = series(Data[0]), MS = series(0);
	int i;
	
//Gently smooth the data in a SuperSmoother
	a1 = exp(-1.414*2*PI/Period);
	b1 = 2*a1*cos(1.414*PI/Period);
	c2 = b1;
	c3 = -a1*a1;
	c1 = 1-c2-c3;
	Filt[0] = c1*(Data[0]+Data[1])/2 + c2*Filt[1] + c3*Filt[2];

//Sum the differences
	for(i=1,DSum=0; i<=Period; i++)
		DSum += Filt[0] - Filt[i];
	DSum /= Period;
	
//Normalize in terms of Standard Deviations
	MS[0] = .04*DSum*DSum + .96*MS[1];
	if(MS[0] > 0.) return DSum/sqrt(MS[0]);
	else return 0.;
}

#ifdef TEST
function run() 
{
	//require(2.22);
	set(LOGFILE,PLOTNOW);
	//setf(PlotMode,PL_TITLE);
	BarPeriod = 1440;
	StartDate = 20181220;
	EndDate = 20191231;
	assetList("AssetsIB");
	asset("SPY");
	vars Prices = series(priceClose());
	plot("ReFlex",ReFlex(Prices,20),NEW|LINE,RED);
	plot("TrendFlex",TrendFlex(Prices,20),LINE,BLUE);

// experiments with trading signals....	
/*	vars Signals = series(ReFlex(Prices,20));
	MaxLong = MaxShort = 1;
	var Threshold = 1.1;
*/
/*	if(Signals[0] < -Threshold) enterLong();
	if(Signals[0] > 0) exitLong();
	if(Signals[0] > Threshold) enterShort();
	if(Signals[0] < 0) exitShort();
*/	
/*	if(Signals[0] < -1 && valley(Signals)) enterLong();
	if(Signals[0] > 1 || peak(Signals)) exitLong();
	if(Signals[0] > 1 && peak(Signals)) enterShort();
	if(Signals[0] < -1 || valley(Signals)) exitShort();
*/	
//	if(crossOver(Signals,0)) enterLong();
//	if(crossUnder(Signals,0)) enterShort();
}
#endif