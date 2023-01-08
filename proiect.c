#include <ansi_c.h>
#include <formatio.h>
#include <advanlys.h>
#include <cvirte.h>		
#include <userint.h>
#include "proiect.h"


static int freqPanel = 0;
static int wavePanel = 0;

#define SAMPLE_RATE		0
#define NPOINTS			1

int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements

double sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
double *waveAnvelopa = 0;

double max = 0.0;
double min = 0.0;

int maxIdx = 0;
int minIdx = 0;

int zeros=0;
double mean = 0.0;
double median=0.0;
double variance=0.0;

static ssize_t hist[101];
static double axis[101];
int histogramIntervals=100;

double *filtruMediere16;
double *filtruMediere32;
double *filtruAlpha;
double alpha; // parametru pentru filtrul alpha

int secundaIdx=0; //index interval afisat pe graph-ul filtrat
int secunda[7]; // = nr secunde .wav +1 (6 secunde = 7 intervale)



int N=0;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	
	if ((wavePanel = LoadPanel (0, "proiect.uir", PANEL)) < 0)
		return -1;
	if (( freqPanel = LoadPanel (0, "proiect.uir", PANEL2)) < 0)
		return -1;
	
	DisplayPanel (wavePanel);
	RunUserInterface ();
	return 0;
}

double *FiltruMediere(int m){
	double *filtWave = (double *) calloc(npoints, sizeof(double));
	int i,j;
	
	for(i=m/2+secunda[secundaIdx]; i<secunda[secundaIdx+1]-(m/2); i++){
		double sum=0;
		for(j=i-m/2; j<i+m/2; j++){
			sum+=waveData[j];
		}
		filtWave[i]=sum/(double)m;
	}
	return filtWave;
} 	

double *FiltruOrdinI(double alpha){
	double *filtWave = (double *) calloc(npoints, sizeof(double));
	int i=secunda[secundaIdx];
	if(alpha==0)
		filtWave[i]=0;
	else
		filtWave[i]=waveData[i];
	
	for(i=secunda[secundaIdx]+1; i<secunda[secundaIdx+1]; i++){
		filtWave[i]=(1-alpha)*filtWave[i-1]+alpha*waveData[i];
	}
	return filtWave;
}

void incarcareFiltru(int panel){
	int filtruAles;
	GetCtrlVal(panel,PANEL_FILTER,&filtruAles);
	
	switch(filtruAles){
		case 1:
			SetCtrlAttribute(panel,PANEL_ALPHA_VAL,ATTR_DIMMED,0);
			DeleteGraphPlot(panel,PANEL_FILTERED_GRAPH,-1,VAL_IMMEDIATE_DRAW);
			
			GetCtrlVal(panel,PANEL_ALPHA_VAL,&alpha);
			filtruAlpha = FiltruOrdinI(alpha);
			PlotY(panel,PANEL_FILTERED_GRAPH,filtruAlpha,npoints,VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS,VAL_DK_RED);
		    break;
			
		case 2:
			SetCtrlAttribute(panel,PANEL_ALPHA_VAL,ATTR_DIMMED,1);
			DeleteGraphPlot(panel,PANEL_FILTERED_GRAPH,-1,VAL_IMMEDIATE_DRAW);
			
			filtruMediere16 = FiltruMediere(16);
			PlotY(panel,PANEL_FILTERED_GRAPH,filtruMediere16,npoints,VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS,VAL_GREEN);
		    break;
			
		case 3:
			SetCtrlAttribute(panel,PANEL_ALPHA_VAL,ATTR_DIMMED,1);
			DeleteGraphPlot(panel,PANEL_FILTERED_GRAPH,-1,VAL_IMMEDIATE_DRAW);
			
			filtruMediere32 = FiltruMediere(32);
			PlotY(panel,PANEL_FILTERED_GRAPH,filtruMediere32,npoints,VAL_DOUBLE,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,VAL_CONNECTED_POINTS,VAL_MAGENTA);
		    break;
			
		default:
		    break;
	}
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			waveAnvelopa = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul waveData.txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//incarcare din fisierul anvelopa.txt in memorie (vector)
			FileToArray("anvelopa.txt", waveAnvelopa, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe graph anvelopa
			PlotY(panel, PANEL_RAW_GRAPH, waveAnvelopa, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
			
			//afisare pe graph
			PlotY(panel, PANEL_RAW_GRAPH, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			MaxMin1D(waveData,npoints,&max,&maxIdx,&min,&minIdx);
			Mean(waveData,npoints,&mean);
			Median(waveData,npoints,&median);
			Variance(waveData,npoints,&mean,&variance);
			
			//calculare numar de zero
			for(int i=0;i<npoints-1;i++){
				if((waveData[i+1]>0 && waveData[i]<0)||(waveData[i+1]<0 && waveData[i]>0))
					zeros++;
			}
			
			Histogram(waveData,npoints,min-1,max+1,hist,axis,histogramIntervals);
			
			//incarcare intervale secunde
			for(int i=0;i<6;i++){
				secunda[i]=((int)npoints/6)*i;
			}
			secunda[6]=npoints;
			
			SetAxisRange (panel, PANEL_HISTOGRAM, VAL_MANUAL, min-1, max+1, VAL_MANUAL, 0, max+1);
	        DeleteGraphPlot (panel, PANEL_HISTOGRAM, -1, VAL_DELAYED_DRAW);
	        PlotXY (panel, PANEL_HISTOGRAM, axis,  hist, histogramIntervals, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_GREEN);
	         
			SetCtrlVal(panel,PANEL_MAX,max);
			SetCtrlVal(panel,PANEL_MIN,min);
			SetCtrlVal(panel,PANEL_IDX_MAX,maxIdx);
			SetCtrlVal(panel,PANEL_IDX_MIN,minIdx);
			SetCtrlVal(panel,PANEL_MEAN,mean);
			SetCtrlVal(panel,PANEL_MEDIAN,median);
			SetCtrlVal(panel,PANEL_VARIANCE,variance);
			SetCtrlVal(panel,PANEL_ZEROS,zeros);
			
			
			SetCtrlAttribute(panel,PANEL_PREV,ATTR_DIMMED,0);
			SetCtrlAttribute(panel,PANEL_NEXT,ATTR_DIMMED,0);
			SetCtrlAttribute(panel,PANEL_FILTER,ATTR_DIMMED,0);
			
			incarcareFiltru(panel);
			break;
	}
	return 0;
}

int CVICALLBACK OnFilterRing (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			incarcareFiltru(panel); // la selectia filtrului il incarcam
			break;
	}
	return 0;
}

int CVICALLBACK OnPrev (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(secundaIdx>0)
				secundaIdx--; //vom afisa intervalul precedent
			
			incarcareFiltru(panel);
			break;
	}
	return 0;
}

int CVICALLBACK OnNext (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(secundaIdx<5)
				secundaIdx++; //vom afisa intervalul urmator
			
			incarcareFiltru(panel);
			break;
	}
	return 0;
}

int CVICALLBACK OnFreqPanelCB (int panel, int event, void *callbackData,
									  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnWavePanelCB (int panel, int event, void *callbackData,
									  int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnSwitchPanelCB (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == wavePanel)
			{
				SetCtrlVal(freqPanel, PANEL_IDC_SWITCH_PANEL, 1);
				DisplayPanel(freqPanel);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(wavePanel, PANEL2_IDC_SWITCH_PANEL, 0);
				DisplayPanel(wavePanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}



int CVICALLBACK OnFilterButtonCB (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	int sec;
	double *rawData;
	int winType;
	double mulArrayOutput[npoints/6];
	double window[npoints/6];

	int signalType;
	static WindowConst winConst;
	
	double *powerSpectrum;
	double *frequencyArray;
	
	char unit[32]="V";
	double df=0.0; 
		
	powerSpectrum=(double*)calloc(npoints/12,sizeof(double));
	frequencyArray=(double*)calloc(npoints/12,sizeof(double));
	
	switch (event)
	{
		case EVENT_COMMIT:
			DeleteGraphPlot (freqPanel,PANEL2_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			
			GetCtrlVal(freqPanel,PANEL2_IDC_SEC,&sec);
			rawData=(double*)calloc(npoints/6,sizeof(double));
			for(int i=0;i<npoints/6;i++)
			{
				rawData[i]=waveData[sec*npoints/6+i];
			}	 
			
			GetCtrlVal(freqPanel,PANEL2_WINDOW_TYPE,&winType);
			
			
			DeleteGraphPlot (freqPanel, PANEL2_GRAPH_RAW_DATA, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, PANEL2_GRAPH_RAW_DATA, rawData, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
			
			LinEv1D(rawData,npoints/6,0,1,window);	   
			switch (winType)
			{
				case 0:
					TriWin(window,npoints/6);
					break;
				case 1:
					FlatTopWin(window,npoints/6);
					break;
			}
			
			DeleteGraphPlot (freqPanel,PANEL2_GRAPH_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel,PANEL2_GRAPH_WINDOW, window, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
			
			Mul1D(rawData,window,npoints/6,mulArrayOutput);
			DeleteGraphPlot (freqPanel,PANEL2_GRAPH_RAW_WINDOW, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel,PANEL2_GRAPH_RAW_WINDOW, mulArrayOutput, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_GREEN);
			
			
			GetCtrlVal(freqPanel,PANEL2_FILTER_TYPE,&signalType);
			
			double *filteredSignal;
			filteredSignal = (double *) calloc(npoints, sizeof(double));
			
			switch(signalType)
			{
				case 0:
					Ch_BPF(mulArrayOutput,npoints/6,sampleRate,2./5*sampleRate,1./2*sampleRate,0.5,5,filteredSignal);
					break;
					
				case 1:
					InvCh_BPF(mulArrayOutput,npoints/6,sampleRate,2./5*sampleRate,1./2*sampleRate,40,5,filteredSignal);
					break;	   
			}
			
			DeleteGraphPlot (freqPanel, PANEL2_GRAPH_FILTER, -1, VAL_IMMEDIATE_DRAW);
			PlotY (freqPanel, PANEL2_GRAPH_FILTER, mulArrayOutput, npoints/6, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
			DeleteGraphPlot (freqPanel,PANEL2_GRAPH_SPECTRUM_FILTER, -1, VAL_IMMEDIATE_DRAW);
        	AutoPowerSpectrum (filteredSignal, npoints/6, 1.0/sampleRate, powerSpectrum, &df);
			SpectrumUnitConversion(powerSpectrum, npoints/12, 0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VPK, df, winConst,frequencyArray, unit);
			PlotY (freqPanel,PANEL2_GRAPH_SPECTRUM_FILTER, frequencyArray, npoints/12, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					
	}
	return 0;
}

int CVICALLBACK OnFrequencyButtonCB (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	GetCtrlVal(freqPanel,PANEL2_IDC_N, &N);
	
	WindowConst winConst;
	double *convertedSpectrum;
	double *autoSpectrum;
	double df=0.0;
	double freqPeak=0.0;
	double powerPeak=0.0;
	char unit[32]="V";
	
	convertedSpectrum=(double*)calloc(npoints/6,sizeof(double));
	autoSpectrum=(double*)calloc(npoints/6,sizeof(double));
	
	switch (event)
	{
		case EVENT_COMMIT:
			ScaledWindowEx (waveData,N, RECTANGLE, 0, &winConst);
			AutoPowerSpectrum(waveData,npoints/6, 1.0/sampleRate, autoSpectrum, &df);
			PowerFrequencyEstimate(autoSpectrum,npoints/6,-1.0,winConst,df,7,&freqPeak,&powerPeak);
			
			SetCtrlVal(freqPanel,PANEL2_FREQ_PEAK,freqPeak);
			SetCtrlVal(freqPanel,PANEL2_POWER_PEAK,powerPeak);
			
			SpectrumUnitConversion(autoSpectrum, N,0, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, winConst, convertedSpectrum, unit);
			
			DeleteGraphPlot(panel,PANEL2_GRAPH_SPECTRU,-1,VAL_IMMEDIATE_DRAW);
			PlotWaveform(panel,  PANEL2_GRAPH_SPECTRU, convertedSpectrum, npoints/12 ,VAL_DOUBLE, 1.0, 0.0, 0.0, df,VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID,  VAL_CONNECTED_POINTS, VAL_RED);	
			break;
	}
	return 0;
}