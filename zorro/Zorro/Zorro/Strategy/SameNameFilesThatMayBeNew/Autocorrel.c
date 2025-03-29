// Autocorrelation ///////////////////////
// Traders Tips 11/2022
////////////////////////////////////////////////////

var SmoothUltimate(var *Data,int Length)
{
	var f = (1.414*PI) / Length;
	var a1 = exp(-f);
	var c2 = 2*a1*cos(f);
	var c3 = -a1*a1;
	var c1 = (1+c2-c3)/4;
	vars US = series(*Data,4);
	return US[0] = (1-c1)*Data[0] + (2*c1-c2)*Data[1] - (c1+c3)*Data[2]
		+ c2*US[1] + c3*US[2];
}

function run()
{
	set(PLOTNOW);
	BarPeriod = 1440; 
	StartDate = 20231001; 
	EndDate = 20240901;
	History = "?.t6";
	LookBack = 150;
	assetAdd("SPY","STOOQ:SPY.US");
	asset("SPY");
	int Length = 20, Lag;
	vars Prices = series(SmoothUltimate(seriesC(),Length));
	if(!is(LOOKBACK)) {
		//var Corr = Correlation(Prices+Length,Prices+Length+Lag,Length);
		//plot("Correl",Corr,NEW,RED);
		for(Lag=1; Lag<100; Lag++) {
			int Row = dataRow(1,dataAppendRow(1,3));
			dataSet(1,Row,0,Correlation(Prices+Length,Prices+Length+Lag,Length));
			dataSet(1,Row,1,wdate());
			dataSet(1,Row,2,(var)Lag);
		}
	}
	if(is(EXITRUN))
		dataChart(1,0,CONTOUR,NULL);
}

