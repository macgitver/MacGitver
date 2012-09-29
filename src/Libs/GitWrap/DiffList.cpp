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

#include "GitWrapPrivate.h"
#include "ChangeListConsumer.h"
#include "PatchConsumer.h"
#include "DiffList.h"
#include "Repository.h"
#include "RepositoryPrivate.h"
#include "DiffListPrivate.h"

namespace Git
{

    namespace Internal
    {

        DiffListPrivate::DiffListPrivate( RepositoryPrivate* repo, git_diff_list* difflist )
            : RepoObject( repo )
            , mDiffList( difflist )
        {
            Q_ASSERT( difflist );
        }

        DiffListPrivate::~DiffListPrivate()
        {
            Q_ASSERT( mRepo );

            if( mDiffList )
            {
                git_diff_list_free( mDiffList );
            }
        }

        static int patchFileCallBack( void* cb_data, const git_diff_delta* delta, float )
        {
            PatchConsumer* pc = (PatchConsumer*) cb_data;

            if( pc->startFile( QString::fromUtf8( delta->old_file.path ),
                               QString::fromUtf8( delta->new_file.path ),
                               PatchConsumer::Type( delta->status ),
                               delta->similarity,
                               delta->binary ) )
            {
                return GIT_ERROR;
            }

            return GIT_OK;
        }

        static int patchHunkCallBack( void* cb_data,
                                      const git_diff_delta* delta,
                                      const git_diff_range* range,
                                      const char* header, size_t header_len )
        {
            PatchConsumer* pc = (PatchConsumer*) cb_data;

            if( pc->startHunk( range->new_start, range->new_lines,
                               range->old_start, range->old_lines,
                               header ? QString::fromUtf8( header, int( header_len ) ) : QString() ) )
            {
                return GIT_ERROR;
            }

            return GIT_OK;
        }

        static int patchDataCallBack( void* cb_data,
                                      const git_diff_delta* delta,
                                      const git_diff_range* range,
                                      char line_origin,
                                      const char *content,
                                      size_t content_len )
        {
            PatchConsumer* pc = (PatchConsumer*) cb_data;

            QString ct;
            if( content && content_len )
            {
                if( content[ content_len - 1 ] == '\n' )
                    --content_len;

                ct = QString::fromUtf8( content, int( content_len ) );
            }

            switch( line_origin )
            {
            case GIT_DIFF_LINE_CONTEXT:
                if( pc->appendContext( ct ) )
                {
                    return GIT_ERROR;
                }
                break;

            case GIT_DIFF_LINE_ADDITION:
                if( pc->appendAddition( ct ) )
                {
                    return GIT_ERROR;
                }
                break;

            case GIT_DIFF_LINE_DELETION:
                if( pc->appendDeletion( ct ) )
                {
                    return GIT_ERROR;
                }
                break;

            default:
                qDebug( "Foo: t=%i", int(line_origin) );

            }

            return GIT_OK;
        }

        static int changeListCallBack( void* cb_data, const git_diff_delta* delta, float )
        {
            ChangeListConsumer* consumer = (ChangeListConsumer*) cb_data;

            if( consumer->raw( QString::fromUtf8( delta->old_file.path ),
                               QString::fromUtf8( delta->new_file.path ),
                               ChangeListConsumer::Type( delta->status ),
                               delta->similarity,
                               delta->binary ) )
            {
                return GIT_ERROR;
            }

            return GIT_OK;
        }

    }

    DiffList::DiffList()
    {
    }

    DiffList::DiffList( Internal::DiffListPrivate* _d )
        : d( _d )
    {
    }

    DiffList::DiffList( const DiffList& o )
        : d( o.d )
    {
    }

    DiffList::~DiffList()
    {
    }

    DiffList& DiffList::operator=( const DiffList& other )
    {
        d = other.d;
        return *this;
    }

    bool DiffList::isValid() const
    {
        return d;
    }

    Repository DiffList::repository( Result& result ) const
    {
        if( !result )
        {
            return Repository();
        }

        if( !d )
        {
            result.setInvalidObject();
            return Repository();
        }

        return Repository( d->repo() );
    }

    bool DiffList::mergeOnto( DiffList onto, Result& result ) const
    {
        if( !result )
        {
            return false;
        }

        if( !d || !onto.isValid() )
        {
            result.setInvalidObject();
            return false;
        }

        result = git_diff_merge( onto.d->mDiffList, d->mDiffList );
        return result;
    }

    bool DiffList::consumePatch( PatchConsumer* consumer, Result& result ) const
    {
        if( !result )
        {
            return false;
        }

        if( !d )
        {
            result.setInvalidObject();
            return false;
        }

        if( !consumer )
        {
            result.setInvalidObject();
            return false;
        }

        result = git_diff_foreach( d->mDiffList, consumer,
                                   &Internal::patchFileCallBack,
                                   &Internal::patchHunkCallBack,
                                   &Internal::patchDataCallBack );

        return result;
    }

    bool DiffList::consumeChangeList( ChangeListConsumer* consumer, Result& result ) const
    {
        if( !result )
        {
            return false;
        }

        if( !d )
        {
            result.setInvalidObject();
            return false;
        }

        if( !consumer )
        {
            result.setInvalidObject();
            return false;
        }

        result = git_diff_foreach( d->mDiffList, consumer,
                                   &Internal::changeListCallBack, NULL, NULL );

        return result;
    }

}
