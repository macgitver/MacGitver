
#ifndef MGV_HEAVEN_HEAVEN_H
#define MGV_HEAVEN_HEAVEN_H

#include "Heaven/HeavenApi.h"

namespace Heaven
{

	enum Positions
	{
		Left,
		Right,
		Top,
		Bottom,
		Central
	};

	enum UiObjectTypes
	{
		ActionType,
		ContainerType,
		MergePlaceType,
		MenuType,
		MenuBarType,
		ToolBarType,
		WidgetActionType,

		SeparatorType
	};

};

#endif
