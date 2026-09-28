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
#include <ui_vcl_heartbeat.h>
#include <QDebug>
#include <QECommon.h>

#define DEBUG qDebug () << "vcl_heartbeat" << __LINE__ << __FUNCTION__ << "  "

//------------------------------------------------------------------------------
//
VCLHeartBeat::VCLHeartBeat (QWidget* parent) : 
   QEAbstractWidget (parent),
   ui (new Ui::VCLHeartBeat ())
{
   this->ui->setupUi (this);

   // No variables managed directly by this widget, PV management is left to
   // the embedded QE Widgets.
   //
   this->setNumVariables (0);

   this->setMinimumSize (22, 20);

   this->setVariableAsToolTip (false);
   this->setAllowDrop (false);
   this->setDisplayAlarmStateOption (QE::Never);

   this->mIocName = "";
   this->mEdgeWidth = 0;
   this->mDefaultSubstitutions = "";

   this->ui->heartBeatShape->setEdgeWidth (0);
}

//------------------------------------------------------------------------------
//
VCLHeartBeat::~VCLHeartBeat ()
{
   delete this->ui;
   this->ui = NULL;
}

//------------------------------------------------------------------------------
//
void VCLHeartBeat::setIocName (const QString& iocName)
{
   this->mIocName = iocName;
   this->ui->heartBeatShape->setVariableNameProperty (iocName + ":IOC_UP_TIME_MONITOR");
}

//------------------------------------------------------------------------------
//
QString VCLHeartBeat::getIocName () const
{
   return this->mIocName;
}

//------------------------------------------------------------------------------
//
void VCLHeartBeat::setEdgeWidth (const int edgeWidth)
{
   this->ui->heartBeatShape->setEdgeWidth (edgeWidth);
}

//------------------------------------------------------------------------------
//
int VCLHeartBeat::getEdgeWidth () const
{
   return this->ui->heartBeatShape->getEdgeWidth ();
}

//------------------------------------------------------------------------------
//
void VCLHeartBeat::setDefaultSubstitutions (const QString& defSubs)
{
   this->mDefaultSubstitutions = defSubs;

   this->ui->heartBeatShape->setVariableNameSubstitutionsProperty (defSubs);
}

//------------------------------------------------------------------------------
//
QString VCLHeartBeat::getDefaultSubstitutions () const
{
   return this->mDefaultSubstitutions;
}

// end
