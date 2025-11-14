/* File: qeVCLApp/src/project/plugin/visual_component_library_plugin.cpp
 * DateTime: Fri Nov 14 17:13:22 2025
 * Last checked in by: starritt
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2019-2023 Australian Synchrotron
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

#include "visual_component_library_plugin.h"
#include <vcl_shutter_manager.h>
#include <vcl_valve_manager.h>
#include <vcl_dblslits_manager.h>
#include <vcl_vertslits_manager.h>
#include <vcl_horizslits_manager.h>
#include <vcl_filter_screen_manager.h>
#include <vcl_stack_lights_manager.h>
#include <vcl_push_buttons_manager.h>
#include <vcl_icons_manager.h>
#include <vcl_eps_comp_manager.h>

// ADD NEXT WIDGET MANAGER HEADER FILE HERE...
// #include <vcl_polyline_manager.h>


//------------------------------------------------------------------------------
// Add all plugins that will be provided in QT Designer.
//
VclWidgets::VclWidgets(QObject *parent) : QObject(parent)
{
   this->widgets.append (new VCLDoubleSlitsManager (this));
   this->widgets.append (new VCLVerticalSlitsManager (this));
   this->widgets.append (new VCLHorizontalSlitsManager (this));
   this->widgets.append (new VCLFilterScreenManager (this));
   this->widgets.append (new VCLApplyPushButtonManager (this));
   this->widgets.append (new VCLNavigatePushButtonManager (this));
   this->widgets.append (new VCLResetPushButtonManager (this));
   this->widgets.append (new VCLStartPushButtonManager (this));
   this->widgets.append (new VCLStopPushButtonManager (this));
   this->widgets.append (new VCLValidatePushButtonManager (this));
   this->widgets.append (new VCLShutterManager (this));
   this->widgets.append (new VCLStackLightsManager (this));
   this->widgets.append (new VCLValveManager (this));
   this->widgets.append (new VCLIonPumpIconManager (this));
   this->widgets.append (new VCLCameraIconManager (this));
   this->widgets.append (new VCLEpsCompManager (this));

   // ADD NEXT WIDGET HERE...
   // this->widgets.append (new VCLPolylineManager (this));
}

//------------------------------------------------------------------------------
//
VclWidgets::~VclWidgets() { }

//------------------------------------------------------------------------------
// Multiplugin interface.
//
QList<QDesignerCustomWidgetInterface*> VclWidgets::customWidgets() const
{
    return this->widgets;
}

// end
