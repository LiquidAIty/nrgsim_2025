// Multi Timeframe Voting  ///////////////////////
// Zorro 2.30 / P.Volkova /////////////

//#define TRADETEST1
//#define TRADETEST2
//#define TRADETEST3

#define LEN	21
#define BUYVOTEMAX 7
#define SELLVOTEMAX 7

int BuyVote,SellVote,WeekNum;

// daily time frame 
var voteD()
{
	vars SeriesK = series((priceClose()-LL(LEN))/(HH(LEN)-LL(LEN)));
	if(MaxVal(SeriesK,5) > 0.8) SellVote++;
	if(MinVal(SeriesK,5) < 0.2) BuyVote++;
	return SeriesK[0];
}

// weekly time frames
var voteW(int Weeks)
{
	vars H = series(priceHigh(),-LEN), 
		L = series(priceLow(),-LEN), 
		C = series(priceClose(),-LEN);

// add new value every nth week
	if((dow() == FRIDAY) && (week() % Weeks == 0)) { 
		shift(H,priceHigh(),LEN);
		shift(L,priceLow(),LEN);
		shift(C,priceClose(),LEN);
	}
	//if(Weeks == 1) plot("W1",H[0],0,RED);
	//if(Weeks == 3) plot("W3",H[0],0,BLUE);
	var PercentK = (C[0]-MinVal(L,LEN))/(MaxVal(H,LEN)-MinVal(L,LEN));
	if(PercentK > 0.8) SellVote++;
	if(PercentK < 0.2) BuyVote++;
	return PercentK;
}

function run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"Voting",32+64+128+7,BLACK); 
	PlotHeight2 = PlotHeight1;
   set(PLOTNOW);
   BarPeriod = 1440;
   //StartDate = 20200101;
   //EndDate = 20200401;
   StartDate = 2007;
   EndDate = 2020;
	LookBack = 13*LEN;

// for alternatively using consecutive week numbers
	if(Init) WeekNum = 0;
	if(dow() == FRIDAY) WeekNum++; 
	
	assetAdd("HAL","STOOQ:HAL.US");
	asset("HAL");
	BuyVote = SellVote = 0; 
	voteD(); 	// daily
	voteW(1);	// weekly
	voteW(4);	// monthly (4 weeks)
	voteW(8);	// bimonthly 
	voteW(13);	// quarterly (13 weeks)
	voteW(3);	// quarter-quarterly
	voteW(6);	// half-quarterly

	plot("BuyVote",BuyVote,NEW|LINE,BLUE);
	plot("SellVote",SellVote,LINE,RED);
	
	MaxLong = 1; // trigger on first occasion
	vars SellVotes = series(SellVote), BuyVotes = series(BuyVote);
#ifdef TRADETEST1
	if(BuyVote == BUYVOTEMAX && SellVote == 0)
		enterLong();
	if(SellVote == SELLVOTEMAX && BuyVote == 0)
		exitLong();
#endif
#ifdef TRADETEST2
	if(BuyVotes[1] >= 6 && SellVotes[1] == 0 && SellVotes[0] > 0)
		enterLong();
	if(SellVotes[0] == 7 && BuyVote == 0)
		exitLong();
#endif
#ifdef TRADETEST3
	if(BuyVotes[1] >= 6 && SellVotes[1] == 0 && falling(BuyVotes))
		enterLong();
	if(SellVote == SELLVOTEMAX && BuyVote == 0)
		exitLong();
#endif
}










