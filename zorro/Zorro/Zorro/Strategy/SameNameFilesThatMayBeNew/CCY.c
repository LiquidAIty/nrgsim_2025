// Correlation cycle indicator ///////////////////////

//#define DO_SINE
//#define DO_PLOT
//#define DO_NOSTATE
#define DO_STATE

var correlY(var Phase); // function pointer
var cosFunc(var Phase) { return cos(2*PI*Phase); }
var sinFunc(var Phase) { return -sin(2*PI*Phase); }

// correlation with a function
var correl(vars Data,int Length,function Func)
{
   correlY = Func; 
   var Sx = 0, Sy = 0, Sxx = 0, Sxy = 0, Syy = 0;
   int count;
   for(count = 0; count < Length; count++) {
      var X = Data[count];
      var Y = correlY((var)count/Length);
      Sx += X;
      Sy += Y;
      Sxx += X*X;
      Sxy += X*Y;
      Syy += Y*Y;
   }
   if(Length*Sxx-Sx*Sx > 0 && Length*Syy-Sy*Sy > 0)
      return (Length*Sxy-Sx*Sy)/sqrt((Length*Sxx-Sx*Sx)*(Length*Syy-Sy*Sy));
   else return 0;
}

var CCY(vars Data,int Length) { return correl(Data,Length,cosFunc); }
var CCYROC(vars Data,int Length) { return correl(Data,Length,sinFunc); }

// market state
var CCYState(vars Data,int Length,var Threshold)
{
   vars Angles = series(0,2);
   var Real = correl(Data,Length,cosFunc);
   var Imag = correl(Data,Length,sinFunc);
//Compute the angle as an arctangent function and resolve ambiguity
   if(Imag != 0) Angles[0] = 90 + 180/PI*atan(Real/Imag);
   if(Imag > 0) Angles[0] -= 180;
//Do not allow the rate change of angle to go negative
   if(Angles[1]-Angles[0] < 270 && Angles[0] < Angles[1])
      Angles[0] = Angles[1];
   //return Angles[0];
//Compute market state
   if(abs(Angles[0]-Angles[1]) < Threshold)
      return ifelse(Angles[0] < 0,-1,1);
   else return 0;
}

#ifdef DO_SINE
function run()
{
   set(PLOTNOW);
   MaxBars = 300;
   LookBack = 40;
   asset(""); // dummy asset
   ColorUp = ColorDn = 0; // don't plot a price curve
   
   vars Chirp = series(genSine(15,30));
   plot("Chirp",2*Chirp[0]-1,LINE,BLUE);
   plot("CCY",correl(Chirp,20,cosFunc),LINE,RED);
}
#endif


#ifdef DO_PLOT
function run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"CCY Chart (SPY)",32+64+128+7,BLACK);  
   set(PLOTNOW);
   BarPeriod = 1440;
   LookBack = 40;
   StartDate = 20190101;

   assetAdd("SPY","STOOQ:SPY.US"); // load price history from Stooq
   asset("SPY");

   vars Prices = series(priceClose());
//   plot("CCY(14)",correl(Prices,14,cosFunc)*0.97,NEW|LINE,RED);
//   plot("ROC(14)",correl(Prices,14,sinFunc)*0.97,LINE,GREEN);
   plot("State",CCIState(Prices,14)*0.9,NEW|LINE,BLUE);
}
#endif

#ifdef DO_NOSTATE
function run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"No State",32+64+128+7,BLACK); 
   set(PARAMETERS,PLOTNOW);
   BarPeriod = 1440;
   LookBack = 40;
   NumYears = 8;

   assetAdd("SPY","STOOQ:SPY.US"); // load price history from Stooq
   asset("SPY");
   
   NumWFOCycles = 4;
   int Cutoff = optimize(10,5,30,5);

   vars Prices = series(priceClose());
   vars Signals = series(LowPass(Prices,Cutoff));
   if(valley(Signals))
      enterLong();
   else if(peak(Signals))
      enterShort();
}
#endif

#ifdef DO_STATE
function run() 
{
   if(is(FIRSTRUN)) 
      plotText("#Title",60,25,"CCYState",32+64+128+7,BLACK); 
   set(PARAMETERS,PLOTNOW);
   BarPeriod = 1440;
   LookBack = 40;
   NumYears = 8;

   assetAdd("SPY","STOOQ:SPY.US"); // load price history from Stooq
   asset("SPY");
   
   NumWFOCycles = 4;
   int Cutoff = optimize(10,5,30,5);
   int Period = optimize(14,10,25,1);
   var Threshold = optimize(9,5,15,1);

   vars Prices = series(priceClose());
   var State = CCYState(Prices,Period,Threshold);
   plot("State",State*0.9,NEW|LINE,BLUE);
   vars Signals = series(LowPass(Prices,Cutoff));
   
   if(State != 0) {
      if(valley(Signals))
         enterLong();
      else if(peak(Signals))
         enterShort();
   }
   else {
      exitLong(); 
		exitShort();
   }
}
#endif
