
#ifndef GIT_SIGNATURE_H
#define GIT_SIGNATURE_H

#include <QString>
#include <QDateTime>

namespace Git
{

	class Signature
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

	private:
		QString		mName;
		QString		mEMail;
		QDateTime	mWhen;
	};

}

#endif
