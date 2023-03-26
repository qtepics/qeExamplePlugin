/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/push_buttons/vcl_push_buttons_manager.cpp $
 * $Revision: #1 $
 * $DateTime: 2021/05/01 20:37:04 $
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

#include "vcl_push_buttons_manager.h"
#include "vcl_push_buttons.h"

QE_IMPLEMENT_PLUGIN_MANAGER
   (VCLApplyPushButton,    "AS VCL", ":/vcl/push_buttons/vcl_apply_push_button.png", false)

QE_IMPLEMENT_PLUGIN_MANAGER
   (VCLNavigatePushButton, "AS VCL", ":/vcl/push_buttons/vcl_navigate_push_button.png", false)

QE_IMPLEMENT_PLUGIN_MANAGER
   (VCLResetPushButton,    "AS VCL", ":/vcl/push_buttons/vcl_reset_push_button.png", false)

QE_IMPLEMENT_PLUGIN_MANAGER
   (VCLStartPushButton,    "AS VCL", ":/vcl/push_buttons/vcl_start_push_button.png", false)

QE_IMPLEMENT_PLUGIN_MANAGER
   (VCLStopPushButton,     "AS VCL", ":/vcl/push_buttons/vcl_stop_push_button.png", false)

QE_IMPLEMENT_PLUGIN_MANAGER
   (VCLValidatePushButton, "AS VCL", ":/vcl/push_buttons/vcl_validate_push_button.png", false)

// end
