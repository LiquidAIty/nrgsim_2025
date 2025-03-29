// Covid Vaxxed vs Unvaxxed survival calculator ///////////////////////////

void click(int Row,int Col)
{
	if(Row != -1) return; // [Result] clicked?
	sound("click.wav");
	printf("\nTotal: Vaxx %.0f%% Unvaxx %.0f%%",
		slider(1),100-slider(1));
	printf("\nCases: Vaxx %.0f%% Unvaxx %.0f%%",
		slider(2),100-slider(2));
	var IncidVaxx = slider(3)*slider(2)/slider(1);
	var IncidUnvaxx = slider(3)*(100-slider(2))/(100-slider(1));
	printf("\nIncidence: Vaxx %.0f Unvaxx %.0f",
		IncidVaxx,IncidUnvaxx);
	printf("\nRisk/week: Vaxx %.2f%% Unvaxx %.2f%%",
		IncidVaxx/1000,IncidUnvaxx/1000);
	printf("\nSurvival/weeks: Vaxx %.0f Unvaxx %.0f",
		log(0.5)/log(1.-IncidVaxx/100000),
		log(0.5)/log(1.-IncidUnvaxx/100000));
	printf("\n");
}

void main()
{
	slider(1,70,1,99,"Total %","Vaccinated in population");
	slider(2,30,1,99,"Cases %","Vaccinated cases");
	slider(3,1000,1,2000,"Incidence","Weekly cases per 100,000"); 
	while(wait(100)); // keep script alive
}