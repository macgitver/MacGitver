
#ifndef HIC_H
#define HIC_H

#include <QCoreApplication>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QList>

enum ObjectTypes
{
	HACO_Ui,
	HACO_Action,
	HACO_Menu,
	HACO_MenuBar,
	HACO_ToolBar,
	HACO_Separator,
	HACO_Container,
	HACO_MergePlace
};

enum HIDTokenId
{
	Token_EOF		= 0,

	Token_Ui,
	Token_Action,
	Token_Menu,
	Token_MenuBar,
	Token_ToolBar,
	Token_Separator,
	Token_Container,
	Token_MergePlace,
	Token_Content,

	Token_string,
	Token_translateString,
	Token_integer,
	Token_true,
	Token_false,

	Token_OpenCurly,
	Token_CloseCurly,
	Token_OpenSquare,
	Token_CloseSquare,
	Token_Comma,
	Token_Semicolon
};

enum HICPropertyTypes
{
	HICP_Any = -1,
	HICP_NULL,
	HICP_String,
	HICP_TRString,
	HICP_Boolean,
	HICP_Integer
};

struct HIDToken
{
	HIDTokenId		id;
	int				line;
	QString			value;
};

class HICProperty
{
public:
	HICProperty( QVariant v, HICPropertyTypes type )
		: mValue( v ), mType( type )
	{
	}

	HICProperty()
		: mType( HICP_NULL )
	{
	}

	QVariant value() const{ return mValue; }
	HICPropertyTypes type() const{ return mType; }

private:
	QVariant mValue;
	HICPropertyTypes mType;
};

class HICObject;

class HICObjects : public QList< HICObject* >
{
public:
	HICObjects() {}

public:
	HICObjects byType( ObjectTypes type ) const;
};

class HICObject
{
public:
	HICObject( ObjectTypes type )
		: mType( type )
	{
	}

public:
	ObjectTypes type(){ return mType; }

	void setName( const QString& name )
	{
		mName = name;
	}

	QString name() const
	{
		return mName;
	}

	bool hasProperty( const QString& name, HICPropertyTypes type = HICP_Any ) const
	{
		if( type == HICP_Any )
		{
			return mProperties.contains( name );
		}

		if( mProperties.contains( name ) )
		{
			const HICProperty& p = mProperties[ name ];
			return p.type() == type;
		}

		return false;
	}

	HICProperty getProperty( const QString& name ) const
	{
		return mProperties.value( name, HICProperty() );
	}

	QStringList propertyNames() const
	{
		return mProperties.keys();
	}

	void addProperty( QString name, HICProperty p )
	{
		Q_ASSERT( !mProperties.contains( name ) );
		mProperties.insert( name, p );
	}

	void addContent( HICObject* content )
	{
		mContent.append( content );
	}

	HICObjects content() const
	{
		return mContent;
	}

	HICObjects content( ObjectTypes type ) const
	{
		return mContent.byType( type );
	}

	bool hasReferenceTo( HICObject* obj )
	{
		return mContent.contains( obj );
	}

private:
	ObjectTypes mType;
	QString mName;
	QHash< QString, HICProperty > mProperties;
	HICObjects mContent;
};

class HeavenInterfaceCompiler : public QCoreApplication
{
public:
	HeavenInterfaceCompiler( int argc, char** argv );

public:
	int run();

private:
	HIDTokenId cur(){ return ( tokenPos < tokens.count() ) ? tokens[ tokenPos ].id : Token_EOF; }
	bool parse();
	bool tokenize( const QString& text );
	void flush( int line );
	bool parseNewObject();
	bool parseProperty();
	bool parseObjectContent();

	void error( const char* pszText, int line );
	void error( const char* pszText );

	bool spitHeader( QTextStream& tsOut );
	bool spitSource( QTextStream& tsOut, const QString& basename );
	void spitSetProperties(QTextStream& tsOut, HICObject* obj,
						   const char* whitespace, const char* prefix );
	HICObjects allObjects( ObjectTypes byType ) const;

private:
	int	tokenPos;
	QList< HIDToken > tokens;
	QString currentText;
	static QHash< QString, HIDTokenId > sTokens;

	HICObject* currentObject;
	HICObject* lastCreatedObject;
	HICObject* currentUiObject;
	QHash< QString, HICObject* > objects;
};

#endif
