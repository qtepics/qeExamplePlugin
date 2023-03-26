/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/shutter/vcl_shutter_manager.cpp $
 * $Revision: #1 $
 * $DateTime: 2020/06/08 15:51:50 $
 * Last checked in by: $Author: starritt $
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

#include "vcl_shutter_manager.h"
#include <vcl_shutter.h>

QE_IMPLEMENT_PLUGIN_MANAGER
   (VCLShutter, "AS VCL", ":/vcl/shutter/vcl_shutter_open.png", false)

// end
