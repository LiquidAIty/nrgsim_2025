// Ehlers' Ultimate Channels & Bands ///////////////////////
// Traders Tips 4/2024
////////////////////////////////////////////////////

var UltimateSmoother(var *Data,int Length)
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

var UltimateChannel(int Length,int STRLength,int NumSTRs)
{
	var TH = max(priceC(1),priceH());
	var TL = min(priceC(1),priceL());
	var STR = UltimateSmoother(series(TH-TL),STRLength);
	var Center = UltimateSmoother(seriesC(),Length);
	rRealUpperBand = Center + NumSTRs*STR;
	rRealLowerBand = Center - NumSTRs*STR;
	return Center;
}

var UltimateBands(int Length,int NumSDs)
{
	var Center = UltimateSmoother(seriesC(),Length);
	vars Diffs = series(priceC()-Center);
	var SD = sqrt(SumSq(Diffs,Length)/Length);
	rRealUpperBand = Center + NumSDs*SD;
	rRealLowerBand = Center - NumSDs*SD;
	return Center;
}

function run()
{
	set(PLOTNOW);
	BarPeriod = 1440; 
	StartDate = 20230301; 
	EndDate = 20240201;
	ColorUp = WHITE;
	ColorDn = GREY;
	assetAdd("ES","YAHOO:ES=F");
	asset("ES");
	// UltimateChannel(20,20,1);
	// plot("UltChannel1",rRealUpperBand,BAND1,BLUE);
	// plot("UltChannel2",rRealLowerBand,BAND2,BLUE|TRANSP);
	UltimateBands(20,1);
	plot("UltBands1",rRealUpperBand,BAND1,BLUE);
	plot("UltBands2",rRealLowerBand,BAND2,BLUE|TRANSP);
}

