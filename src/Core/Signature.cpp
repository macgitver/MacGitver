
#include "Core/Git_p.h"
#include "Core/Signature.h"

namespace Git
{
	
	Signature git2Signature( const git_signature* gitsig )
	{
		QDateTime dt = QDateTime::fromTime_t( gitsig->when.time );
		dt.setUtcOffset( gitsig->when.offset * 60 );

		return Signature(
			QString::fromLatin1( gitsig->name ),
			QString::fromLatin1( gitsig->email ),
			dt );
	}

	git_signature* signature2git( const Signature& sig )
	{
		git_signature* gitsig = 0;

		int rc = git_signature_new(
			&gitsig, qPrintable( sig.name() ), qPrintable( sig.email() ),
			sig.when().toTime_t(), sig.when().utcOffset() / 60 );

		if( rc < GIT_SUCCESS )
		{
			return NULL;
		}

		return gitsig;
	}

}

