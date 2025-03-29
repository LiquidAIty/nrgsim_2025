// VPN Indicator ////////////////

//#define DO_PLOT
#define DO_TRADE

// VPN=EMA(100*(VP–VN)/VTOTAL)
var VPN(vars Volumes,int Period)
{
	var Dist = 0.1*ATR(Period);
	vars TypPrices = series(TypPrice());
	var Vp=0,Vn=0,Vtotal= 0;	int i; 
	for(i=0; i<Period; i++) {
		if(TypPrices[i] > TypPrices[i+1] + Dist)
			Vp += Volumes[i];
		if(TypPrices[i] < TypPrices[i+1] - Dist)
			Vn += Volumes[i];
		Vtotal += Volumes[i];
	}
	if(Vtotal == 0) Vtotal = 1; // prevent division by zero
	return EMA(100*(Vp-Vn)/Vtotal,3);
}	

/*VPN SYSTEM
//Copyright Markos Katsanos 2020
SetBacktestmode( backtestregular );
Posize =10000;SetPositionSize( posize, spsValue );
PosQty = 10; Setoption( "maxopenPositions", PosQty );
IE= 100000;SetoPTIoN( "INITIALEQUITY", IE );
roundLotSize = 10;
Setoption("AllowPositionShrinking", True );
Setoption("Commissionmode",3);
Setoption("CommissionAmount",.01); //0.01$ per share
Setoption("minPosValue",1000);
Setoption("minShares",10);
SetTradeDelays( 1, 1, 1, 1 );
BuyPrice =oPEN;
SellPrice = open;
timeexit =optimize("exit",15,5,70,5);
Period =optimize("VPNPErIoD",30,30,40,10 );
VPNCrIT=optimize("VPNCrIT",10,10,20,10); //
//Price , Volume and Liquidity Filter
minC = Param( "minimum Share Price", 1, 0, 2000, 1 );
minVol = Param( "minimum Volume", 100000, 0, 1000000, 50000 );
minVC = Param( "min Turnover(Vol*C/1E6)", .5, 0, 10, 1 );
LQD=C>minc AND mA(V,5)>minVol AND mA(C*V,5)/1000000>minVC ;
//INDEX
IndexCode = ParamStr( "Index", "IWm" );
IND = Foreign( IndexCode, "C" );
//VPN
mAV=mA(V,PErIoD);
V=Nz(V,1);
mAV=IIF(mAV>0,mAV,1);
mF = Avg - ref( Avg, -1 );
mC=.1*ATr(PErIoD);
VmP = IIf( mF > mC, V, 0 ) ;
VP = Sum( VmP , PErIoD);
VmN = IIf( mF < -mC, V, 0 ) ;
VN = Sum( VmN , PErIoD);
VPN=(VP-VN)/mAV/PErIoD*100;
VPN=EmA(VPN,3);

mAVL=ref(mA(V,PErIoD),-3);
BUY = LQD AND mA(V,50) >ref(mA(V,50),-50) AND
Cross(VPN,VPNCrIT) AND rSI(5)<90 AND C>mA(C,PErIoD);
Sell=(IND<mA(IND,150) AND roC(IND,10)<-15) or
(Cross(mA(VPN,PErIoD),VPN) AND C<HHV(C,5)-3*ATr(PErIoD));
Short=False;
Cover=False;
ApplyStop( stopTypeNBar, stopmodeBars, timeexit ); // time exit
mAVL=ref(mA(V,PErIoD),-3);//mAVL=Nz(mAVL,1);
PoSITIoNSCorE=V/mAVL;
*/

#ifdef DO_PLOT	
void run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,35,"VPN",32+64+128+7,BLACK); 
   set(PLOTNOW|LOGFILE);
	PlotHeight2 = PlotHeight1/2;
   BarPeriod = 1440;
   StartDate = 20190501;
   EndDate = 20201231;

   assetAdd("IDEX","STOOQ:IDEX.US");
   asset("IDEX");
	vars Volumes = series(marketVol(0));
   plot("TPR",VPN(Volumes,30),NEW,RED);	
   plot("Threshold",10,0,BLACK);	
}
#endif

#ifdef DO_TRADE
void run() 
{
	if(!require(-2.36)) return;
   if(is(FIRSTRUN)) 
      plotText("#Title",60,35,"VPN / R2000",32+64+128+7,BLACK); 
   set(PLOTNOW|LOGFILE);
	setf(TrainMode,SETFACTORS); // skip OptimalF calculation
	setf(PlotMode,PL_ALL|PL_BENCHMARK);
	set(PARAMETERS);
	NumWFOCycles = 5;
	NumCores = -2;
   BarPeriod = 1440;
   StartDate = 2011;
   EndDate = 2021;
	LookBack = 150;
	UpdateDays = 5; // don't download all again every day
	
	assetList("AssetsRussell2000");
	assetAdd("IWM","STOOQ:IWM.US"); // Russell2000 index ETF	

	int Period = optimize(30,30,40,10);
	var Threshold = optimize(10,10,20,10);

// Sell after 15 days, 
// or when a Russell2000 15% drop and a value below the 150-day SMA indicates meltdown. 			
	LifeTime = optimize(15,5,70,5);
	asset("IWM");
	int Meltdown = ROC(seriesC(),10) < -15 && priceClose() < SMA(seriesC(),150);

// Use index as benchmark	
	static var StartPrice;
	if(is(LOOKBACK)) StartPrice = priceClose();
	var Benchmark = 10*10000/priceClose() * (priceClose()-StartPrice);
	plot("Russell2000",Benchmark,AXIS2,PURPLE);

	for(listed_assets)
//	while(asset(loop("CTSO")))
	{
		asset(Asset);
		if(Asset == "IWM") continue; // don't trsde the index
		vars Volumes = series(marketVol(0),100);
		vars Prices = series(priceClose(),30);
		vars Signals = series(VPN(Volumes,Period),2);

// Filter out when price was below 1 
// or 5-day average volume below 100000 
// or the 5-day average product of price and volume below 500000.		
		bool Filter = Prices[0] < 1 
			|| SMA(Volumes,5) < 100000 
			|| SMA(series(Volumes[0]*Prices[0],5),5) < 500000;
		
// buy when the VPN crosses over 10% 
// and its 50-day volume average is above its previous 50-day average, 
// its 5-day RSI is below 90, and the price is above its 30-day SMA. 		
		bool Buy = crossOver(Signals,Threshold)
			&& SMA(Volumes,50) > SMA(Volumes+50,50)
			&& RSI(Prices,5) < 90
			&& Prices[0] > SMA(Prices,30);
			
// Sell when the VPN crosses below its 30-day SMA while the price is more than 3 ATR below its 5-day high, 
// or when as market meltdown is immiment
		bool Sell = (crossUnder(Signals,SMA(Signals,30)) 
			&& Prices[0] < HH(5) - 3*ATR(5)) || Meltdown;

		if(!Filter && Buy && !Sell && (Train || NumLongTotal < 10))
			enterLong(10000/Prices[0]);
		else if(Sell)
			exitLong();
	}
}
