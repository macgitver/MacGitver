
#ifndef GIT_REFERENCE_H
#define GIT_REFERENCE_H

#include "Core/Git.h"

namespace Git
{

	class ReferencePrivate;

	class ObjectId;

	class Reference
	{
	public:
		enum Type
		{
			Direct, Symbolic
		};

	public:
		Reference();
		Reference( ReferencePrivate* p );

	public:
		bool isValid() const;
		void destroy();
		QByteArray name() const;

		Type type() const;
		ObjectId objectId() const;
		QByteArray target() const;

	private:
		ReferencePrivate* d;
	};

}

#endif
