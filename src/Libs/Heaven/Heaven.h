
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

	/**
	 * @brief	These types classify a Heaven::View
	 *
	 *			The basic view type is Single. It's a view that can be opened and closed. When it's
	 *			closed, it will forget about it's data. A global view is an addition to that. When
	 *			it gets closed, it will remember it's data for a period of time. A type of Multiple
	 *			means that more than one view will reference the same data.
	 *
	 *			Contrary to all of that, a Contextual view will provide additional value to the user
	 *			which makes only sense in a specific context of another view.
	 */
	enum ViewTypes
	{
		SingleViewType,
		GlobalViewType,
		MultipleViewType,
		ContextualViewType
	};

}

#endif
