
#include "Libs/Git/Git_p.h"

#include "Libs/Git/ObjectTag.h"

namespace Git
{

	ObjectTag::ObjectTag()
	{
	}

	ObjectTag::ObjectTag( ObjectPrivate* _d )
		: Object( _d )
	{
		Q_ASSERT( type() == otTag );
	}

	ObjectTag::ObjectTag( const ObjectTag& o )
		: Object( o )
	{
	}

}
