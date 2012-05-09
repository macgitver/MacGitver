
#ifndef GIT_SIGNATURE_H
#define GIT_SIGNATURE_H

#include <QDebug>

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

		QString fullName() const
		{
			return QString( "%1<%2>" )
					.arg( mName )
					.arg( mEMail );
		}

		QString toNaturalString() const
		{
			return QString( "%1 <%2> %3" )
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
