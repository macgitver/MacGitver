
#include "Core/Git_p.h"

#include "Core/ObjectBlob.h"

namespace Git
{

	ObjectBlob::ObjectBlob()
	{
	}

	ObjectBlob::ObjectBlob( ObjectPrivate* _d )
		: Object( _d )
	{
		Q_ASSERT( type() == otBlob );
	}

	ObjectBlob::ObjectBlob( const ObjectBlob& o )
		: Object( o )
	{
	}

}
