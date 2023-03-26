/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/plugin/visual_component_library_global.h $
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

/* Description:
 *
 * Manage the declaration of library public functions as either exported functions
 * when building the library, or imported functions when using the library.
 *
 */

#ifndef VISUAL_COMPONENT_LIBRARY_GLOBAL_H
#define VISUAL_COMPONENT_LIBRARY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VISUAL_COMPONENT_LIBRARY)
#  define VISUAL_COMPONENT_LIBRARY_SHARED Q_DECL_EXPORT
#else
#  define VISUAL_COMPONENT_LIBRARY_SHARED Q_DECL_IMPORT
#endif

// See comments in QEFrameworkLibraryGlobal.h - this is similar
//
// Rules that appear to work are that meta data declarations are required
// when bulding against the example plugin library, but not when building
// the plugin itself.
//
#if !defined(VISUAL_COMPONENT_LIBRARY)
#  define VCL_DECLARE_METATYPE_IS_REQUIRED
#else
#  undef  VCL_DECLARE_METATYPE_IS_REQUIRED
#endif

// Example
// #ifdef VCL_DECLARE_METATYPE_IS_REQUIRED
// Q_DECLARE_METATYPE (QEFormStateChange::Formats)
// #endif

#endif // VISUAL_COMPONENT_LIBRARY_GLOBAL_H
