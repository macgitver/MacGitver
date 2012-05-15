
#ifndef MGV_CONFIG_PAGE_H
#define MGV_CONFIG_PAGE_H

#include <QWidget>

class ConfigPageGroup
{
public:
	ConfigPageGroup( ConfigPageGroup* parent, const QString& name, const QByteArray& id );

public:
	QByteArray id() const;
	QString name() const;
	ConfigPageGroup* parent();

private:
	ConfigPageGroup*	mParent;
	QString				mName;
	QByteArray			mId;
};

class ConfigWidget;

class ConfigPage : public QWidget
{
	Q_OBJECT
public:
	ConfigPage( ConfigWidget* widget );

protected:
	ConfigPageGroup* getOrMakeGroup( const QString& name, const QByteArray& id );
	void setModified();

public:
	virtual void apply() = 0;
	virtual void init() = 0;

	virtual ConfigPageGroup* group() const = 0;
	virtual QString name() const = 0;

	ConfigWidget* configWidget();

private:
	ConfigWidget* mWidget;
};

#endif
