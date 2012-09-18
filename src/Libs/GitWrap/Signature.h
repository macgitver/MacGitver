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

#ifndef GIT_SIGNATURE_H
#define GIT_SIGNATURE_H

#include <QDebug>

#include <QString>
#include <QDateTime>

#include "GitWrap.h"

namespace Git
{

    /**
	 * @ingroup		GitWrap
	 * @brief		Commit signature
	 *
	 * A signature object is used to identify the originator and a time of a commit. Each commit has
	 * two signatures assigned to it:
	 * - __Author__ \n
	 *   The author is the person, who initially wrote a change set. (To be seen in terms of
	 *   copyright).
	 * - __Committer__ \n
	 *   The committer is the person, who committed the change set into the repository.
	 *
	 * This concept is specific to distributed VCSes: The person creating a change might not even
	 * have access to the repository where it should be committed to. The commit is transported to
	 * a person _who actually_ has that access. Depending on defined workflow, it may get reviewed,
	 * tested or otherwise validated along that way.
	 *
	 * Git itself has several ways to transport a change set from one place to another, which means
	 * there are different rules when committer and author will vary. I.e. if the transport is
	 * E-Mail, the committer information is not transported at all.
	 *
	 * OTOH, if you're using a integrated git hosting service, like github, you might see that a
	 * cross merge from one repository to another will leave the committer intact; while
	 * cherry-picking a commit, will not.
	 *
     */
	class GITWRAP_API Signature
	{
	public:
		Signature( const QString& name, const QString& email, const QDateTime& when )
			: mName( name )
			, mEMail( email )
			, mWhen( when )
		{
		}

		Signature( const QString& name, const QString& email )
			: mName( name )
			, mEMail( email )
			, mWhen( QDateTime::currentDateTime() )
		{
		}

		Signature()
		{
		}

		~Signature()
		{
		}

	public:
		QString name() const
		{
			return mName;
		}

		QString email() const
		{
			return mEMail;
		}

		QDateTime when() const
		{
			return mWhen;
		}

		QString fullName() const
		{
			return QString( QLatin1String( "%1 <%2>" ) )
					.arg( mName )
					.arg( mEMail );
		}

		QString toNaturalString() const
		{
			return QString( QLatin1String( "%1 <%2> %3" ) )
					.arg( mName )
					.arg( mEMail )
					.arg( mWhen.toString( Qt::ISODate ) );
		}

	private:
		QString		mName;
		QString		mEMail;
		QDateTime	mWhen;
	};

}

inline QDebug operator<<( QDebug debug, const Git::Signature& sig )
{
	return debug << "Sig(" << sig.toNaturalString() << ")";
}


#endif
