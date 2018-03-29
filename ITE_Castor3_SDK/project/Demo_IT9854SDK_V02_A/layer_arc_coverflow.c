#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "scene.h"
#include "ctrlboard.h"

#define LANG_DISPLAY_NUM	7

#ifndef ARCTABLE_H
#define ARCTABLE_H

static int arc_3[91] = {
	32767, 32609, 32441, 32263, 32075, 31878, 31670, 31453,
	31227, 30991, 30745, 30490, 30226, 29953, 29670, 29378,
	29078, 28769, 28451, 28124, 27788, 27445, 27092, 26732,
	26363, 25987, 25602, 25210, 24810, 24402, 23987, 23565,
	23136, 22699, 22256, 21805, 21349, 20885, 20415, 19940,
	19457, 18970, 18476, 17976, 17472, 16961, 16446, 15926,
	15401, 14871, 14336, 13798, 13255, 12708, 12157, 11602,
	11044, 10483, 9918, 9350, 8780, 8206, 7631, 7053,
	6473, 5890, 5306, 4721, 4134, 3546, 2956, 2366,
	1775, 1183, 592, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0 };

static int arc_5[91] = {
	32768, 32649, 32520, 32381, 32233, 32075, 31907, 31729,
	31541, 31344, 31138, 30922, 30696, 30461, 30217, 29964,
	29702, 29430, 29150, 28860, 28562, 28255, 27940, 27616,
	27283, 26943, 26594, 26237, 25872, 25499, 25118, 24730,
	24334, 23931, 23521, 23103, 22678, 22247, 21808, 21363,
	20912, 20454, 19990, 19519, 19043, 18561, 18073, 17580,
	17082, 16578, 16069, 15556, 15037, 14514, 13987, 13455,
	12920, 12380, 11836, 11289, 10739, 10185, 9628, 9068,
	8506, 7941, 7373, 6803, 6231, 5658, 5082, 4505,
	3926, 3347, 2766, 2185, 1602, 1020, 437, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0 };

static int arc_7[91] = {
	32768, 32672, 32566, 32451, 32326, 32190, 32045, 31891,
	31726, 31552, 31368, 31175, 30972, 30760, 30539, 30308,
	30068, 29818, 29560, 29293, 29016, 28731, 28437, 28135,
	27824, 27504, 27176, 26840, 26495, 26143, 25782, 25414,
	25038, 24654, 24263, 23865, 23459, 23046, 22626, 22199,
	21765, 21325, 20878, 20425, 19966, 19500, 19029, 18552,
	18069, 17580, 17087, 16588, 16084, 15575, 15061, 14543,
	14021, 13494, 12963, 12428, 11889, 11347, 10801, 10252,
	9699, 9144, 8586, 8026, 7463, 6897, 6330, 5761,
	5189, 4617, 4043, 3467, 2891, 2314, 1736, 1157,
	579, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0 };

#endif /* ARCTABLE_H */

#if (LANG_DISPLAY_NUM==3)
static int *arc_table = arc_3;
#elif (LANG_DISPLAY_NUM==5)
static int *arc_table = arc_5;
#elif (LANG_DISPLAY_NUM==7)
static int *arc_table = arc_7;
#endif

static int h, center, stepAngle = 180 / (LANG_DISPLAY_NUM + 1);
static char widgetName[30];
static ITUCoverFlow *arcCoverFlow = 0;
static ITUBackground *arcCoverFlowBackground[10] = { 0 };
static ITUContainer *arcCoverFlowCFContainer[10] = { 0 };
static ITUBackground *arcCoverFlowAllBackground = 0;

bool ArcCoverflowOnEnter(ITUWidget* widget, char* param)
{
	if (!arcCoverFlow)
	{
		int i;
		arcCoverFlow = ituSceneFindWidget(&theScene, "arcCoverFlow");
		assert(arcCoverFlow);

		arcCoverFlowAllBackground = ituSceneFindWidget(&theScene, "arcCoverFlowAllBackground");
		assert(arcCoverFlowAllBackground);

		for (i = 0; i < 10; i++){
			sprintf(widgetName, "arcCoverFlowBackground%d", i);
			arcCoverFlowBackground[i] = ituSceneFindWidget(&theScene, widgetName);
			assert(arcCoverFlowBackground[i]);

			sprintf(widgetName, "settingLangCFContainer%d", i);
			arcCoverFlowCFContainer[i] = ituSceneFindWidget(&theScene, widgetName);
			assert(arcCoverFlowCFContainer[i]);
		}
		h = ituWidgetGetHeight(arcCoverFlowCFContainer[0]);
		center = h * ((ituWidgetGetHeight(arcCoverFlowAllBackground) / h) / 2);
	}
	return true;
}

bool ArcCoverflowOnTimer(ITUWidget* widget, char* param)
{
	bool updated = false;
	if (ituWidgetUpdate(arcCoverFlowCFContainer[0], ITU_EVENT_LAYOUT, 0, 0, 0)) {
		int i;
		for (i = 0; i<10; i++) {
			// calculate arc height
			int y = ituWidgetGetY(arcCoverFlowCFContainer[i]);
			int theta = (y - center)*stepAngle / h;
			int arc_height;
			if (theta>90) theta = 90;
			if (theta<-90) theta = -90;
			arc_height = (arc_table[abs(theta)] * h) >> 15;
			if (arc_height) {
				ituWidgetSetVisible(arcCoverFlowBackground[i], true);
				ituWidgetSetHeight(arcCoverFlowBackground[i], arc_height);
				if (theta>0)
					ituWidgetSetY(arcCoverFlowBackground[i], y);
				else
					ituWidgetSetY(arcCoverFlowBackground[i], y + h - arc_height);
			}
			else
				ituWidgetSetVisible(arcCoverFlowBackground[i], false);
		}
		updated = true;
	}
	return updated;
}


