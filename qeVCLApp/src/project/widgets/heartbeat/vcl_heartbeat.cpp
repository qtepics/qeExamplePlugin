/* Repo: tec_gui/qevcl
 * File: qeVCLApp/src/project/widgets/heartbeat/vcl_heartbeat.cpp
 * DateTime: Thu Sep 24 16:00:58 2026
 * Last checked in by: starritt
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2026 Australian Synchrotron
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

#include "vcl_heartbeat.h"
#include <QDebug>
#include <QSimpleShape.h>

#define DEBUG qDebug () << "vcl_heartbeat" << __LINE__ << __FUNCTION__ << "  "

static const QColor diastolicColour = QColor (0x5A7E90);  // dark blue
static const QColor systolicColour  = QColor (0x00AAFF);  // bright blue


//------------------------------------------------------------------------------
//
VCLHeartBeat::VCLHeartBeat (QWidget* parent) : 
   QESimpleShape (parent)
{
   this->setMinimumSize (22, 20);

   this->setShape (QSimpleShape::Shapes::heart);
   this->setModulus (2);
   this->setColour0Property (diastolicColour);
   this->setColour1Property (systolicColour);
   this->setDisplayAlarmStateOption (QE::DisplayAlarmStateOptions::WhenInvalid);
}

//------------------------------------------------------------------------------
//
VCLHeartBeat::~VCLHeartBeat () { }

// end
