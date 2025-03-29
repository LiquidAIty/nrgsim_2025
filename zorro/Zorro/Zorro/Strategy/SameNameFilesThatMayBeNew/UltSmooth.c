// Ehlers' Ultimate Smoother ///////////////////////
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
	StartDate = 20230201; 
	EndDate = 20231201;
	ColorUp = WHITE;
	ColorDn = GREY;
	assetAdd("ES","YAHOO:ES=F");
	asset("ES");
	int Length = 30;
	plot("UltSmooth",SmoothUltimate(seriesC(),Length),LINE,MAGENTA);
	plot("Smooth",Smooth(seriesC(),Length),LINE,RED);
	plot("EMA",EMA(seriesC(),3./Length),LINE,BLUE);
}

