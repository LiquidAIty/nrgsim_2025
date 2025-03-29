// Moving Average Bands ////////////////////////////
// Traders Tips 8/21
////////////////////////////////////////////////////

var MAB(int Period1,int Period2,var Mltp)
{
	var MA1 = EMA(seriesC(),Period1);
	var MA2 = EMA(seriesC(),Period2);
	var Dst = MA1-MA2;
	var Dv = SMA(series(Dst*Dst),Period2);
	var Dev = sqrt(Dv)*Mltp;
	rRealMiddleBand = MA1;
	rRealUpperBand = MA1+Dev;
	rRealLowerBand = MA1-Dev;
	return MA2;
}

function run()
{
	StartDate = 20111201; 
	EndDate = 20130815;
	BarPeriod = 1440; 
	LookBack = 300;
	
	set(PLOTNOW);
	asset("NAS100");

	var Signal = MAB(200,50,1);

	plot("MA2",Signal,LINE,BLUE);
	plot("MAB1",rRealUpperBand,BAND1,RED);
	plot("MAB2",rRealLowerBand,BAND2,RED|TRANSP);
}
