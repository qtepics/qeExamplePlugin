/*  exampleDesignerPlugin.cpp
 *
 *  This file is part of the EPICS QT Framework, initially developed at the
 *  Australian Synchrotron.
 *
 *  The EPICS QT Framework is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The EPICS QT Framework is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with the EPICS QT Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright (c) 2015,2017 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

#include <exampleDesignerPlugin.h>

#include <exampleWidgetManager.h>
// ADD NEXT WIDGET HERE...


//-------------------------------------------------------------------------------
// Add all plugins that will be provided in QT Designer.
//
exampleWidgets::exampleWidgets(QObject *parent) : QObject(parent)
{
    widgets.append (new exampleWidgetManager (this));
    // ADD NEXT WIDGET HERE...
}

//-------------------------------------------------------------------------------
// Multiplugin interface.
//
QList<QDesignerCustomWidgetInterface*> exampleWidgets::customWidgets() const
{
    return widgets;
}

//-------------------------------------------------------------------------------
// Publish plugins through to QT designer.
//
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
// Publish plugins through to QT designer.
Q_EXPORT_PLUGIN2(exampleWidgetsPlugin, exampleWidgets)
#endif

// end
