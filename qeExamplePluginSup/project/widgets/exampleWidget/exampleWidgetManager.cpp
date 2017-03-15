/*
 *  This file is part of the EPICS QT Framework, initially developed at the Australian Synchrotron.
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
 *  Copyright (c) 2015 Australian Synchrotron
 *
 *  Author:
 *    Andrew Rhyder
 *  Contact details:
 *    andrew.rhyder@synchrotron.org.au
 */

#include <exampleWidgetManager.h>
#include <exampleWidget.h>
#include <QtPlugin>

/*
    ???
*/
exampleWidgetManager::exampleWidgetManager( QObject *parent ) : QObject( parent ) {
    initialized = false;
}

/*
    ???
*/
void exampleWidgetManager::initialize( QDesignerFormEditorInterface * ) {
    if( initialized ) {
        return;
    }
    initialized = true;
}

/*
    ???
*/
bool exampleWidgetManager::isInitialized() const {
    return initialized;
}

/*
    Widget factory. Creates a exampleWidget widget.
*/
QWidget *exampleWidgetManager::createWidget ( QWidget *parent ) {
    return new exampleWidget(parent);
}

/*
    Name for widget. Used by Qt Designer in widget list.
*/
QString exampleWidgetManager::name() const {
    return "exampleWidget";
}

/*
    Name of group Qt Designer will add widget to.
*/
QString exampleWidgetManager::group() const {
    return "EPICSQt Example Plugin";
}

/*
    Icon for widget. Used by Qt Designer in widget list.
*/
QIcon exampleWidgetManager::icon() const {
    return QIcon(":/customPlugin/exampleWidget/exampleWidget.png");
}

/*
    Tool tip for widget. Used by Qt Designer in widget list.
*/
QString exampleWidgetManager::toolTip() const {
    return "Example widget based on QE framework";
}

/*
    ???
*/
QString exampleWidgetManager::whatsThis() const {
    return "Example widget (EPICS diference label)";
}

/*
    ???
*/
bool exampleWidgetManager::isContainer() const {
    return false;
}

/*
    ???
*/
/*QString exampleWidgetManager::domXml() const {
    return "<widget class=\"exampleWidget\" name=\"exampleWidget\">\n"
           " <property name=\"geometry\">\n"
           "  <rect>\n"
           "   <x>0</x>\n"
           "   <y>0</y>\n"
           "   <width>100</width>\n"
           "   <height>100</height>\n"
           "  </rect>\n"
           " </property>\n"
           " <property name=\"toolTip\" >\n"
           "  <string></string>\n"
           " </property>\n"
           " <property name=\"whatsThis\" >\n"
           "  <string> "
           ".</string>\n"
           " </property>\n"
           "</widget>\n";
}*/

/*
    ???
*/
QString exampleWidgetManager::includeFile() const {
    return "exampleWidget.h";
}
