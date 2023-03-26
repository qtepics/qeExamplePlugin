/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/filter_screen/vcl_filter_screen_manager.h $
 * $Revision: #1 $
 * $DateTime: 2021/03/23 15:36:59 $
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

#ifndef VCL_FILTER_SCREEN_MANAGER_H
#define VCL_FILTER_SCREEN_MANAGER_H

#include <QEDesignerPluginCommon.h>
#include <visual_component_library_global.h>

class VISUAL_COMPONENT_LIBRARY_SHARED VCLFilterScreenManager :
   public QObject,
   public QDesignerCustomWidgetInterface
{
   Q_OBJECT
   Q_INTERFACES (QDesignerCustomWidgetInterface)

   QE_DECLARE_PLUGIN_MANAGER (VCLFilterScreen)
};

#endif // VCL_FILTER_SCREEN_MANAGER_H
