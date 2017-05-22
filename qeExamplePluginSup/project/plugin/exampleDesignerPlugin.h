/*  exampleDesignerPlugin.h
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

#ifndef EXAMPLE_DESIGNER_PLUGIN_H
#define EXAMPLE_DESIGNER_PLUGIN_H

#include <QtPlugin>
#include <QtGlobal>
#if QT_VERSION >= 0x050500
    #include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>
#else
    #include <QDesignerCustomWidgetCollectionInterface>
#endif

// The plugin interface class.
//
class exampleWidgets: public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    Q_PLUGIN_METADATA(IID "au.org.synchrotron.example" FILE "exampleplugin.json")
#endif
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
    exampleWidgets(QObject *parent = 0);
    virtual ~exampleWidgets(){}

    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
    QList<QDesignerCustomWidgetInterface*> widgets;
};

#endif // EXAMPLE_DESIGNER_PLUGIN_H
