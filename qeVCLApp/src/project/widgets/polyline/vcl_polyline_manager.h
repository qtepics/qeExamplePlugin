/* File: qeVCLApp/src/project/widgets/polyline/vcl_polyline_manager.h
 * DateTime: Mon May 26 17:17:29 2025
 * Last checked in by: starritt
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2021 Australian Synchrotron
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

#ifndef VCL_POLYLINE_MANAGER_H
#define VCL_POLYLINE_MANAGER_H

#include <QtGlobal>
#include <QtUiPlugin/QDesignerCustomWidgetInterface>
#include <QDesignerTaskMenuExtension>
#include <QExtensionFactory>
#include <QEDesignerPluginCommon.h>

#include <vcl_polyline.h>
#include <visual_component_library_global.h>

// Differed class declarations
//
QT_BEGIN_NAMESPACE
class QAction;
class QExtensionManager;
QT_END_NAMESPACE

class VISUAL_COMPONENT_LIBRARY_SHARED VCLPolylineManager :
   public QObject,
   public QDesignerCustomWidgetInterface
{
   Q_OBJECT
   Q_INTERFACES (QDesignerCustomWidgetInterface)
   QE_DECLARE_PLUGIN_MANAGER (VCLPolyline)
};

//------------------------------------------------------------------------------
// The VCLPolyline provides design time extensions
//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLPolylineTaskMenu :
      public QObject,
      public QDesignerTaskMenuExtension
{
    Q_OBJECT
    Q_INTERFACES (QDesignerTaskMenuExtension)

public:
    VCLPolylineTaskMenu (VCLPolyline* menuButton, QObject* parent);

    QAction* preferredEditAction() const;
    QList<QAction *> taskActions() const;

private slots:
    void editUserInfo ();

private:
    QAction* editUserInfoAction;
    VCLPolyline* polyline;
};

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLPolylineTaskMenuFactory :
      public QExtensionFactory
{
    Q_OBJECT

public:
    VCLPolylineTaskMenuFactory (QExtensionManager* parent = 0);

protected:
    QObject* createExtension (QObject* object,
                              const QString& iid,
                              QObject* parent) const;
};


#endif // VCL_POLYLINE_MANAGER_H
