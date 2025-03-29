// Stock Gap Statistics ////////////////////////////
// Traders Tips 6/2024
////////////////////////////////////////////////////

#define STOCKS	"AAPL","AMZN","BABA","BAC","NVDA","TSLA","WMT"
#define NA	7	// number of stocks
#define NR	7	// number of ranges

var Ranges[NR] = { 1,3,5,7,9,11,9999 };
int Gaps[NA][NR];
var Pullbacks[NA][NR],Closes[NA][NR];
int i,j;

void run()
{
	BarPeriod = 1440; 
	LookBack = 1;
	StartDate = 20100101; 
	EndDate = 20230201;
	History = "?.t6";
	assetList("AssetsSP50"); // the S&P 500 index

	if(is(INITRUN)) { // reset the arrays
		memset(Gaps,0,NA*NR*sizeof(int));
		memset(Pullbacks,0,NA*NR*sizeof(var));
		memset(Closes,0,NA*NR*sizeof(var));
	}
	
	for(j=0; 1; j++) { // calculate gaps, pullbacks, relative closes
		if(!asset(of(STOCKS))) break;  // select the stock
		if(Bar < AssetFirstBar) continue; 
		var Gap = 100*(priceO(0)/priceC(1)-1.);
		for(i=0; i<NR; i++) if(Gap < Ranges[i]) {
			Gaps[j][i]++; 
			Pullbacks[j][i] += priceL(0)/priceO(0)-1.; 
			Closes[j][i] += priceC(0)/priceO(0)-1.; 
			break;
		}
	}
	
	if(is(EXITRUN)) { // print the results to excel
		print(TO_CSV,"Range,< 1%%,1-3%%,3-5%%,5-7%%,7-9%%,9-11%%,> 11%%\n");
		for(j=0; 1; j++) {
			if(!asset(of(STOCKS))) break;
			print(TO_CSV,"%s Gaps",Asset);
			for(i=0; i<NR; i++) 
				print(TO_CSV,",%i",Gaps[j][i]);
			print(TO_CSV,"\n");
		}
		print(TO_CSV,"\n");
		for(j=0; 1; j++) {
			if(!asset(of(STOCKS))) break;
			print(TO_CSV,"%s PBs",Asset);
			for(i=0; i<NR; i++) 
				print(TO_CSV,",%.2f%%",100*Pullbacks[j][i]/fix0(Gaps[j][i]));
			print(TO_CSV,"\n");
		}
		print(TO_CSV,"\n");
		for(j=0; 1; j++) {
			if(!asset(of(STOCKS))) break;
			print(TO_CSV,"%s Cls",Asset);
			for(i=0; i<NR; i++) 
				print(TO_CSV,",%.2f%%",100*Closes[j][i]/fix0(Gaps[j][i]));
			print(TO_CSV,"\n");
		}
		exec("Data\\Gaps.csv",0,0); // start Excel and open the sheet
	}
}

