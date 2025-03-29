// Compare Price Momentum Oscillator ///////////////////////
// Zorro 2.28+ needed //////////////////////////////////

var PMO(vars Data)
{
	return 10*EMA(EMA(ROC(Data,1),34),19);
}

function run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"CPMO Chart",32+64+128+7,BLACK);  
   set(PLOTNOW);
   BarPeriod = 1440;
   LookBack = 2;
   StartDate = 20180425;
   EndDate = 20190601;

   assetAdd("SPX","STOOQ:^SPX"); // load price history from Stooq
   asset("SPX");
   assetAdd("IXY","YAHOO:IXY");
   asset("IXY");
   vars PricesIXY = series(priceClose());
   assetAdd("IXR","YAHOO:IXR");
   asset("IXR");
   vars PricesIXR = series(priceClose());

   plot("IXY",PMO(PricesIXY),NEW|LINE,GREEN);
   plot("IXR",PMO(PricesIXR),LINE,RED);
	PlotHeight2 = PlotHeight1;
}

