/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: OnWavePanelCB */
#define  PANEL_IDC_SWITCH_PANEL           2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  PANEL_HISTOGRAM                  3       /* control type: graph, callback function: (none) */
#define  PANEL_FILTERED_GRAPH             4       /* control type: graph, callback function: (none) */
#define  PANEL_RAW_GRAPH                  5       /* control type: graph, callback function: (none) */
#define  PANEL_LOADBUTTON                 6       /* control type: command, callback function: OnLoadButtonCB */
#define  PANEL_IDX_MAX                    7       /* control type: numeric, callback function: (none) */
#define  PANEL_IDX_MIN                    8       /* control type: numeric, callback function: (none) */
#define  PANEL_ZEROS                      9       /* control type: numeric, callback function: (none) */
#define  PANEL_VARIANCE                   10      /* control type: numeric, callback function: (none) */
#define  PANEL_MEDIAN                     11      /* control type: numeric, callback function: (none) */
#define  PANEL_MEAN                       12      /* control type: numeric, callback function: (none) */
#define  PANEL_ALPHA_VAL                  13      /* control type: numeric, callback function: (none) */
#define  PANEL_MAX                        14      /* control type: numeric, callback function: (none) */
#define  PANEL_MIN                        15      /* control type: numeric, callback function: (none) */
#define  PANEL_FILTER                     16      /* control type: ring, callback function: OnFilterRing */
#define  PANEL_NEXT                       17      /* control type: command, callback function: OnNext */
#define  PANEL_PREV                       18      /* control type: command, callback function: OnPrev */

#define  PANEL2                           2       /* callback function: OnFreqPanelCB */
#define  PANEL2_IDC_SWITCH_PANEL          2       /* control type: binary, callback function: OnSwitchPanelCB */
#define  PANEL2_FREQ_BUTTON               3       /* control type: command, callback function: OnFrequencyButtonCB */
#define  PANEL2_IDC_N                     4       /* control type: ring, callback function: (none) */
#define  PANEL2_FREQ_PEAK                 5       /* control type: numeric, callback function: (none) */
#define  PANEL2_POWER_PEAK                6       /* control type: numeric, callback function: (none) */
#define  PANEL2_GRAPH_SPECTRU             7       /* control type: graph, callback function: (none) */
#define  PANEL2_GRAPH_RAW_DATA            8       /* control type: graph, callback function: (none) */
#define  PANEL2_GRAPH_WINDOW              9       /* control type: graph, callback function: (none) */
#define  PANEL2_GRAPH_RAW_WINDOW          10      /* control type: graph, callback function: (none) */
#define  PANEL2_GRAPH_FILTER              11      /* control type: graph, callback function: (none) */
#define  PANEL2_GRAPH_SPECTRUM_FILTER     12      /* control type: graph, callback function: (none) */
#define  PANEL2_FILTER_BUTTON             13      /* control type: command, callback function: OnFilterButtonCB */
#define  PANEL2_WINDOW_TYPE               14      /* control type: ring, callback function: (none) */
#define  PANEL2_FILTER_TYPE               15      /* control type: ring, callback function: (none) */
#define  PANEL2_IDC_SEC                   16      /* control type: ring, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnFilterButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFilterRing(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrequencyButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNext(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPrev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSwitchPanelCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnWavePanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif