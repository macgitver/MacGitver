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

#include "GitWrap.h"

namespace Git
{

	namespace Internal
	{
		class ConfigPrivate;
	}

	typedef QHash< QString, QString > ConfigValues;

    /**
	 * @ingroup		GitWrap
	 * @brief		Provides access git configuration files
	 *
     */
	class GITWRAP_API Config
	{
	public:
		Config();
		Config( Internal::ConfigPrivate* cfg );
		Config( const Config& other );
		~Config();
		Config& operator=( const Config& other );

	public:
		bool addFile( const QString& fileName, int priority );

		ConfigValues values();

	public:
		static QString globalFilePath();
		static QString userFilePath();
		static Config global();
		static Config user();
		static Config file( const QString& fileName );
		static Config create();

	private:
		Internal::GitPtr< Internal::ConfigPrivate > d;
	};

}
