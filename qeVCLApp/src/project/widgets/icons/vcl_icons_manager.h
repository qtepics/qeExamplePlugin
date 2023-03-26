/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/icons/vcl_icons_manager.h $
 * $Revision: #1 $
 * $DateTime: 2021/08/07 12:27:57 $
 * Last checked in by: $Author: starritt $
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

#ifndef VCL_ICONS_MANAGER_H
#define VCL_ICONS_MANAGER_H

#include <QEDesignerPluginCommon.h>
#include <visual_component_library_global.h>

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLIonPumpIconManager :
   public QObject,
   public QDesignerCustomWidgetInterface
{
   Q_OBJECT
   Q_INTERFACES (QDesignerCustomWidgetInterface)

   QE_DECLARE_PLUGIN_MANAGER (VCLIonPumpIcon)
};

//------------------------------------------------------------------------------
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLCameraIconManager :
   public QObject,
   public QDesignerCustomWidgetInterface
{
   Q_OBJECT
   Q_INTERFACES (QDesignerCustomWidgetInterface)

   QE_DECLARE_PLUGIN_MANAGER (VCLCameraIcon)
};

#endif // VCL_ICONS_MANAGER_H
