// Supertrend Indicator /////////////
// Traders Tips 10/2023
////////////////////////////////////////////////////
#include <contract.c>

void run()
{
  StartDate = 2020;
  EndDate = 2023;
  BarPeriod = 1440;
  PlotHeight2 = PlotHeight1;
  set(PLOTNOW);
  
  assetAdd("GOLD","YAHOO:GC=F");
  asset("GOLD");
  string COT_Code = "088691"; // gold COT report
  var ComNet = COT_CommercialPos(1,COT_Code); 
  plot("Net Commercial",ComNet,NEW,RED);
  var COTCI = COT_CommercialIndex(1,COT_Code,26); 
  //var COTCI = normalize(ComNet,26);
  plot("COT Index",COTCI,NEW,BLUE);
}
