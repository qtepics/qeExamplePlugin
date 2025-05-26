/* File: qeVCLApp/src/project/plugin/visual_component_library_plugin.h
 * DateTime: Mon May 26 17:17:29 2025
 * Last checked in by: starritt
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2019 Australian Synchrotron
 *
 * The QE VCL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The QE VCL Framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the QE VCL Framework.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Original author: Andrew Starritt
 * Maintained by:   Andrew Starritt
 * Contact details: andrews@ansto.gov.au
 */

#ifndef VISUAL_COMPONENT_LIBRARY_PLUGIN_H
#define VISUAL_COMPONENT_LIBRARY_PLUGIN_H

#include <QtPlugin>
#include <QtGlobal>
#include <QtUiPlugin/QDesignerCustomWidgetCollectionInterface>

// The plugin interface class.
//
class VclWidgets: public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT

    Q_PLUGIN_METADATA (IID "au.org.synchrotron.vcl" FILE "visual_component_library_plugin.json")
    Q_INTERFACES (QDesignerCustomWidgetCollectionInterface)

public:
    VclWidgets (QObject* parent = 0);
    virtual ~VclWidgets();

    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
    QList<QDesignerCustomWidgetInterface*> widgets;
};

#endif // VISUAL_COMPONENT_LIBRARY_PLUGIN_H
