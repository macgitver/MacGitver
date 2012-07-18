/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef HEAVEN_WINDOW_STATE_H
#define HEAVEN_WINDOW_STATE_H

#include <QSharedData>
#include <QVector>
#include <QSet>

#include "Heaven/HeavenApi.h"

namespace Heaven
{

	class View;
	class ViewFactory;
	class ViewContainer;
	class ViewContainerContent;
	class TopLevelWidget;

	class HEAVEN_API WindowStateBase : public QSharedData
	{
	public:
		enum Type
		{
			WSSplitter,
			WSTab,
			WSView,

			WSRoot
		};

	protected:
		struct ApplyContext
		{
			TopLevelWidget*		tlw;
			ViewContainer*		container;
			ViewFactory*		factory;
			QSet< View* >		previousViews;
		};

	public:
		typedef QExplicitlySharedDataPointer< WindowStateBase > Ptr;

	public:
		WindowStateBase( WindowStateBase* parent );
		virtual ~WindowStateBase();

	public:
		virtual Type type() const = 0;

		int childrenCount() const;
		WindowStateBase::Ptr childAt( int index ) const;
		QVector< WindowStateBase::Ptr > children() const;

	protected:
		virtual void apply( ApplyContext& ctx ) = 0;

		static bool import( ViewContainerContent* vc, WindowStateBase* parent );

	private:
		WindowStateBase*				mParent;
		QVector< WindowStateBase::Ptr >	mChildren;
	};

	class HEAVEN_API WindowStateSplitter : public WindowStateBase
	{
	public:
		typedef QExplicitlySharedDataPointer< WindowStateSplitter > Ptr;

	public:
		WindowStateSplitter( WindowStateBase* parent );
		WindowStateSplitter( ViewContainer* vc, WindowStateBase* parent );

	public:
		virtual Type type() const;

	protected:
		virtual void apply( ApplyContext& ctx );

	private:
		QVector< int >				mWidths;
	};

	class HEAVEN_API WindowStateTab : public WindowStateBase
	{
	public:
		typedef QExplicitlySharedDataPointer< WindowStateTab > Ptr;

	public:
		WindowStateTab( ViewContainer* vc, WindowStateBase* parent );

	public:
		virtual Type type() const;

	protected:
		virtual void apply( ApplyContext& ctx );
	};

	class HEAVEN_API WindowStateView : public WindowStateBase
	{
	public:
		typedef QExplicitlySharedDataPointer< WindowStateView > Ptr;

	public:
		WindowStateView( View* view, WindowStateBase* parent );

	public:
		virtual Type type() const;

	protected:
		virtual void apply( ApplyContext& ctx );

	private:
		QString						mViewId;
		QByteArray					mBinaryViewState;
	};

	class HEAVEN_API WindowStateRoot : public WindowStateBase
	{
	public:
		typedef QExplicitlySharedDataPointer< WindowStateRoot > Ptr;

	public:
		WindowStateRoot();
		explicit WindowStateRoot( TopLevelWidget* tlWidget );

		~WindowStateRoot();

	public:
		Type type() const;

		void apply( TopLevelWidget* tlWidget, ViewFactory* factory );

	protected:
		virtual void apply( ApplyContext& ctx );
	};

}

#endif
