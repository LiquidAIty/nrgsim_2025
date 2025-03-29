// ROC Bands ////////////////
// Zorro 2.33+

var RocDev;

var ROCWM(vars Data,int Period1,int Period2,int Period3)
{
	var Roc = ROC(Data,Period1);
	vars RocSqrs = series(Roc*Roc);
	RocDev = sqrt(Sum(RocSqrs,Period3)/Period3);
	return EMA(Roc,Period2);
}
	
function run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"ROC Bands",32+64+128+7,BLACK);  
   set(PLOTNOW);
   BarPeriod = 1440;
   StartDate = 20130801;
   EndDate = 20140601;

   assetAdd("SPX","STOOQ:^SPX"); // load price history from Stooq
   asset("SPX");

   plot("ROC",ROCWM(seriesC(),12,3,12),AXIS2|LINE,BLUE);
   plot("RB1",RocDev,AXIS2|LINE,RED);
   plot("RB2",-RocDev,AXIS2|LINE,RED);
}
