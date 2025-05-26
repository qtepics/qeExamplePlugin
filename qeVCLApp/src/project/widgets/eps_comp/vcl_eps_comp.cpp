/* File: qeVCLApp/src/project/widgets/eps_comp/vcl_eps_comp.cpp
 * DateTime: Mon May 26 17:17:29 2025
 * Last checked in by: starritt
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2023-2024 Australian Synchrotron
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

#include "vcl_eps_comp.h"
#include <QEEnums.h>
#include <ui_vcl_eps_comp.h>

#include <QDebug>

#define DEBUG qDebug () << "vcl_eps_comp" << __LINE__ << __FUNCTION__ << "  "

//------------------------------------------------------------------------------
//
VCLEpsComp::VCLEpsComp (QWidget* parent) :
   QEFrame (parent),
   ui (new Ui::VCLEpsComp ())
{
   this->ui->setupUi (this);

   // Set plain shape and noframe.
   //
   this->setFrameShape (QFrame::NoFrame);
   this->setFrameShadow (QFrame::Plain);

   this->setMinimumSize (this->ui->mainFrame->minimumSize ());
   this->setMaximumSize (this->ui->mainFrame->maximumSize ());

   // No variables managed directly by this widget, PV management is left to
   // the embedded QE Widgets.
   //
   this->setNumVariables (0);

   this->setVariableAsToolTip (false);
   this->setAllowDrop (false);
   this->setDisplayAlarmStateOption (QE::Never);

   this->mDeviceName = "";
   this->mSignalName = "";
   this->mIsStatus = false;
   this->mHasEpsOosStatus = true;
   this->mDefaultSubstitutions = "";

   QObject::connect(this->ui->epsOosShape, SIGNAL (dbValueChanged (const int&)),
                    this, SLOT(epsOosStatusChanged (const int&)));
}

//------------------------------------------------------------------------------
//
VCLEpsComp::~VCLEpsComp ()
{
   delete this->ui;
   this->ui = NULL;
}

//------------------------------------------------------------------------------
//
QSize VCLEpsComp::sizeHint () const
{
   return this->minimumSize();
}

//------------------------------------------------------------------------------
//
void VCLEpsComp::setPvNames ()
{
   QString mainPv;
   QString edgePv;

   if (!this->mIsStatus) {
      mainPv = QString ("%1:%2_MONITOR").arg (this->mDeviceName).arg (this->mSignalName);
      edgePv = QString ("%1:%2_STATUS") .arg (this->mDeviceName).arg (this->mSignalName);
      this->ui->valueLabel->setAlignment (Qt::AlignVCenter | Qt::AlignRight);
   } else {
      mainPv = QString ("%1:%2_STATUS")      .arg (this->mDeviceName).arg (this->mSignalName);
      edgePv = QString ("%1:%2_LATCH_STATUS").arg (this->mDeviceName).arg (this->mSignalName);
      this->ui->valueLabel->setAlignment (Qt::AlignVCenter | Qt::AlignHCenter);
   }

   this->ui->descriptionLabel->setVariableNameProperty (this->mDeviceName + ":NAME");
   this->ui->valueLabel->setVariableNameProperty (mainPv);
   this->ui->statusShape->setVariableNameProperty (mainPv);
   this->ui->statusShape->setEdgeVariableNameProperty (edgePv);

   if (this->mHasEpsOosStatus) {
      this->ui->epsOosShape->setVariableNameProperty (this->mDeviceName + ":EPS_OOS_STATUS");
      this->ui->epsOosShape->setShape (QSimpleShape::cross);  // until we know better
   } else {
      this->ui->epsOosShape->setVariableNameProperty ("");    // clear PV name
      this->ui->epsOosShape->setShape (QSimpleShape::noShape);
   }
}

//------------------------------------------------------------------------------
//
void VCLEpsComp::epsOosStatusChanged (const int& state)
{
   if (state == 0) {
      this->ui->epsOosShape->setShape (QSimpleShape::tick);
   } else if (state == 1) {
      this->ui->epsOosShape->setShape (QSimpleShape::cross);
   } else {
      // In an unknown state
      this->ui->epsOosShape->setShape (QSimpleShape::diamond);
   }
}

//------------------------------------------------------------------------------
//
void VCLEpsComp::setDeviceName (const QString& deviceName)
{
   this->mDeviceName = deviceName;
   this->setPvNames();
}

//------------------------------------------------------------------------------
//
QString VCLEpsComp::getDeviceName () const
{
   return this->mDeviceName;
}

//------------------------------------------------------------------------------
//
void VCLEpsComp::setSignalName (const QString& signalName)
{
   this->mSignalName = signalName;
   this->setPvNames();
}

//------------------------------------------------------------------------------
//
QString VCLEpsComp::getSignalName () const
{
   return this->mSignalName;
}

//------------------------------------------------------------------------------
//
void VCLEpsComp::setIsStatus (const bool isStatus)
{
   this->mIsStatus = isStatus;
   this->setPvNames();
}

//------------------------------------------------------------------------------
//
bool VCLEpsComp::getIsStatus () const
{
   return this->mIsStatus;
}

//------------------------------------------------------------------------------
//
void VCLEpsComp::setHasEpsOosStatus (const bool hasEpsOosStatus)
{
   this->mHasEpsOosStatus = hasEpsOosStatus;
   this->setPvNames();
}

//------------------------------------------------------------------------------
//
bool VCLEpsComp::getHasEpsOosStatus () const
{
   return this->mHasEpsOosStatus;
}

//------------------------------------------------------------------------------
//
void VCLEpsComp::setDefaultSubstitutions (const QString& defSubs)
{
   this->mDefaultSubstitutions = defSubs;

   // Apply the same default substitutions to all the embedded widgets.
   //
   this->ui->descriptionLabel->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->valueLabel->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->statusShape->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->epsOosShape->setVariableNameSubstitutionsProperty (defSubs);
}

//------------------------------------------------------------------------------
//
QString VCLEpsComp::getDefaultSubstitutions () const
{
   return this->mDefaultSubstitutions;
}

// end
