/*
 * MacGitver
 * Copyright (C) 2012-2013 Sascha Cunz <sascha@babbelbox.org>
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

#ifndef MGV_MODULE_H
#define MGV_MODULE_H

#include <QObject>

#include "libGitWrap/Repository.hpp"

#include "libMacGitverCore/MacGitverApi.hpp"
#include "libMacGitverCore/App/MacGitver.hpp"
#include "libMacGitverCore/Config/Ui/ConfigPageProvider.hpp"

namespace BlueSky {
    class Mode;
}

class ConfigDialog;

/**
 * @brief The Module class provides an abstract implementation for application plugins.
 * Use this, when implementing new plugins.
 */
class MGV_CORE_API Module : public QObject, public ConfigPageProvider
{
    Q_OBJECT
public:
    Module();
    ~Module();

public:
    /**
     * @brief Setup a configuration dialog for a module, which is used in the settings.
     * @param dialog
     */
    virtual void setupConfigPages(ConfigDialog* dialog);

    /**
     * @brief Tells the module to initialize itself, after it was loaded and instantiated.
     */
    virtual void initialize() = 0;

    /**
     * @brief Called before a module is unloaded, telling it to clean up itself.
     */
    virtual void deinitialize() = 0;

protected:
    virtual int configPagePriority() const;

protected:
    BlueSky::ViewDescriptor* registerView(const BlueSky::ViewIdentifier& identifier,
                                          const QString& displayName,
                                          MgvViewCreator creator );

    void unregisterView(const BlueSky::ViewIdentifier& identifier);

    void registerMode(BlueSky::Mode* mode);
    void unregisterMode(BlueSky::Mode* mode);

private:
    template<typename T>
    struct ViewCreatorT {
        static BlueSky::View* create() {
            return new T;
        }
    };

protected:
    /**
     * @brief       Register a view
     *
     * @tparam      T           Class name of the View to unregister
     *
     * @param[in]   displayName A translated name that is displayed to the user when this view is
     *                          referenced.
     *
     * @return      The descriptor for the new view.
     *
     * This method just calls to the regular registerView() but gives @a T's class name (obtained
     * from Qt's static meta object) as identifier.
     *
     * A creator function tailored exactly for that view is automatically instanciated.
     *
     */
    template<typename T>
    inline BlueSky::ViewDescriptor* registerView(const QString& displayName) {
        return registerView(T::staticMetaObject.className(),
                            displayName,
                            &ViewCreatorT<T>::create);
    }

    /**
     * @brief       Unregister a view
     *
     * @tparam      T    Class name of the View to unregister
     *
     * This method just calls to the regular unregisterView() but gives @a T's class name (obtained
     * from Qt's static meta object) as identifier.
     *
     */
    template<typename T>
    inline void unregisterView() {
        unregisterView(T::staticMetaObject.className());
    }

};

Q_DECLARE_INTERFACE( Module,
                     "org.macgitver.Module/0.1" )

#endif
