/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/vertslits/vcl_vertslits.cpp $
 * $Revision: #5 $
 * $DateTime: 2024/02/02 17:10:15 $
 * Last checked in by: $Author: starritt $
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2020-2024 Australian Synchrotron
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

#include "vcl_vertslits.h"
#include <ui_vcl_vertslits.h>
#include <QDebug>
#include <QEEnums.h>
#include <QECommon.h>
#include <vcl_dblslits.h>

#define DEBUG qDebug () << "vcl_vertslits" << __LINE__ << __FUNCTION__ << "  "

//------------------------------------------------------------------------------
//
VCLVerticalSlits::VCLVerticalSlits (QWidget* parent) :
   QEAbstractWidget (parent),
   ui (new Ui::VCLVerticalSlits ())
{
   this->ui->setupUi (this);

   this->setMinimumSize (this->ui->mainFrame->minimumSize ());
   this->setMaximumSize (this->ui->mainFrame->maximumSize ());

   // Clear style sheets - just used at design time.
   //
   this->ui->slitFrame->setStyleSheet ("");
   this->ui->widgetBottomLeft->setStyleSheet ("");
   this->ui->widgetBottomRight->setStyleSheet ("");

   this->ui->slitFrame->installEventFilter (this);

   this->mTitle = "Vertical Slits";
   this->ui->titleLabel->setLabelTextProperty (this->mTitle);
   this->mVsMotor = "";
   this->mVcMotor = "";
   this->mUpperMotor = "";
   this->mLowerMotor = "";
   this->mDefaultSubstitutions = "";

   this->mPrecision = 4;
   this->mUseDbPrecision = true;
   this->mAddUnits = true;

   // There are no real sensible defaults.
   // But at least these value are sane.
   //
   this->mNominalVerticalGap = 20.0;
   this->vSize   = 5.0;
   this->vCentre = 0.0;

   this->upperFrac = 0.25;
   this->lowerFrac = 0.25;

   // Connect to virtual motor position changes.
   //
   QObject::connect (this->ui->vsMtrPosnMon, SIGNAL (dbValueChanged   (const double&)),
                     this,                   SLOT   (vsMtrPosnChanged (const double&)));

   QObject::connect (this->ui->vcMtrPosnMon, SIGNAL (dbValueChanged   (const double&)),
                     this,                   SLOT   (vcMtrPosnChanged (const double&)));

   // No variables managed directly by this widget, PV management is left to
   // the embedded QE Widgets.
   //
   this->setNumVariables (0);

   this->setVariableAsToolTip (false);
   this->setAllowDrop (false);
   this->setDisplayAlarmStateOption (QE::Never);
}

//------------------------------------------------------------------------------
//
VCLVerticalSlits::~VCLVerticalSlits ()
{
   delete this->ui;
   this->ui = NULL;
}

//------------------------------------------------------------------------------
// This function provides behaviour somewhat similar to a vertical or horizontal
// layout.  It positions and sizes the internal widgets relative to the overall
// size of the VCLVerticalSlits widget itself.
//
void VCLVerticalSlits::setBladeLayout ()
{
   int bladeLength;
   QRect geo;

   const int sfw = this->ui->slitFrame->width();
   const int sfh = this->ui->slitFrame->height();

   // Essentially a third each.
   //
   const int frameWidth = (sfw - 9) / 2;
   const int bladeWidth = frameWidth;

   const int frameHeight = (sfh - 9) / 3;
   const int bladeHeight = frameHeight;

   geo.setTop (2);
   geo.setLeft (2 + frameWidth + 2);
   geo.setWidth (frameWidth);
   geo.setHeight (frameHeight);
   this->ui->widgetBottomLeft->setGeometry(geo);

   geo.setTop (2 + frameHeight + 2 + bladeHeight);
   geo.setLeft (2 + frameWidth + 2);
   geo.setWidth (frameWidth);
   geo.setHeight (frameHeight);
   this->ui->widgetBottomRight->setGeometry(geo);

   // Do blades.
   bladeLength = (sfh - 4)*this->upperFrac;
   geo.setTop (2);
   geo.setLeft (2);
   geo.setWidth (bladeWidth);
   geo.setHeight (bladeLength);
   this->ui->upperBlade->setGeometry (geo);

   bladeLength = (sfh - 4)*this->lowerFrac;
   geo.setTop (sfh - (bladeLength + 3));
   geo.setLeft (2);
   geo.setWidth (bladeWidth);
   geo.setHeight (bladeLength);
   this->ui->lowerBlade->setGeometry (geo);
}

//------------------------------------------------------------------------------
// Maybe we should split this function into separate horizontal and vertical
// functions as these are independent.
//
void VCLVerticalSlits::calcFractionalPositions ()
{
   static const double minF = 0.05;
   static const double maxF = 0.95;

   double posn;
   double frac;

   // NOTE:
   // +x-axis is toward the left hand side, i.e.toward smaller pixel x value
   // +y-axis is toward the top of widget, i.e.toward smaller pixel y value
   //
   // Calculate p, the blade edge position, relative to centre. Then calc the
   // factional postion of blade edge from widget edge towards the widget centre.
   // The edge has a fractional postion of 0.0 (or 0%)
   // The centre has a fractional postion of 0.5 (or 50%)
   // The far/other edge has a fractional postion of 1.0 (or 100%)
   //
   // We want the nominal gap to span from 25% to 75%
   //

   // Ditto the vertical stuff.
   //
   posn = this->vCentre - (this->vSize / 2.0);
   frac = 0.5 + (0.5 * posn / this->mNominalVerticalGap);
   this->lowerFrac = LIMIT (frac, minF, maxF);

   posn = this->vCentre + (this->vSize / 2.0);
   frac = 0.5 - (0.5 * posn / this->mNominalVerticalGap);
   this->upperFrac = LIMIT (frac, minF, maxF);

   this->setBladeLayout ();
}

//------------------------------------------------------------------------------
//
bool VCLVerticalSlits::eventFilter (QObject *watched, QEvent *event)
{
   const QEvent::Type type = event->type ();
   bool result = false;

   switch (type) {

      case QEvent::Resize:
         if (watched == this->ui->slitFrame) {
            this->setBladeLayout ();
            result = true;
         }
         break;

      default:
         // Not handled.
         //
         result = false;
         break;
   }

   return result;
}

//------------------------------------------------------------------------------
//
void VCLVerticalSlits::setTitle (const QString& title)
{
   this->mTitle = title;
   this->ui->titleLabel->setLabelTextProperty (this->mTitle);
}

//------------------------------------------------------------------------------
//
QString VCLVerticalSlits::getTitle () const
{
   return this->mTitle;
}

//------------------------------------------------------------------------------
//
void VCLVerticalSlits::setNominalVerticalGap (const double nominalVerticalGap)
{
   this->mNominalVerticalGap = MAX (1.0e-6, nominalVerticalGap);
   this->calcFractionalPositions ();
}

//------------------------------------------------------------------------------
//
double VCLVerticalSlits::getNominalVerticalGap () const
{
   return this->mNominalVerticalGap;
}

//------------------------------------------------------------------------------
//
void VCLVerticalSlits::setVsMotor (const QString& vsMotor)
{
   this->mVsMotor = vsMotor;
   this->ui->vsMtrPosnMon->setVariableNameProperty  (vsMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon));
   this->ui->vsMtrPosnSet->setVariableNameProperty  (vsMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnSet));
   this->ui->vsMtrTweakFwd->setVariableNameProperty (vsMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakFwd));
   this->ui->vsMtrTweakRev->setVariableNameProperty (vsMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakRev));
   this->ui->vsMtrTweakVal->setVariableNameProperty (vsMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakVal));
}

//------------------------------------------------------------------------------
//
QString VCLVerticalSlits::getVsMotor () const
{
   return this->mVsMotor;
}

//------------------------------------------------------------------------------
//
void VCLVerticalSlits::setVcMotor (const QString& vcMotor)
{
   this->mVcMotor = vcMotor;
   this->ui->vcMtrPosnMon->setVariableNameProperty  (vcMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon));
   this->ui->vcMtrPosnSet->setVariableNameProperty  (vcMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnSet));
   this->ui->vcMtrTweakFwd->setVariableNameProperty (vcMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakFwd));
   this->ui->vcMtrTweakRev->setVariableNameProperty (vcMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakRev));
   this->ui->vcMtrTweakVal->setVariableNameProperty (vcMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakVal));
}

//------------------------------------------------------------------------------
//
QString VCLVerticalSlits::getVcMotor () const
{
   return this->mVcMotor;
}

//------------------------------------------------------------------------------
//
void VCLVerticalSlits::setUpperMotor (const QString& upperMotor)
{
   this->mUpperMotor = upperMotor;
   const QString pvName = upperMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon);
   this->ui->upperBlade->setVariableNameProperty (pvName);
   this->ui->upperBlade->setEdgeVariableNameProperty (pvName);
}

//------------------------------------------------------------------------------
//
QString VCLVerticalSlits::getUpperMotor () const
{
   return this->mUpperMotor;
}

//------------------------------------------------------------------------------
//
void VCLVerticalSlits::setLowerMotor (const QString& lowerMotor)
{
   this->mLowerMotor = lowerMotor;
   const QString pvName = lowerMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon);
   this->ui->lowerBlade->setVariableNameProperty (pvName);
   this->ui->lowerBlade->setEdgeVariableNameProperty (pvName);

}

//------------------------------------------------------------------------------
//
QString VCLVerticalSlits::getLowerMotor () const
{
   return this->mLowerMotor;
}

//------------------------------------------------------------------------------
//
void VCLVerticalSlits::setDefaultSubstitutions (const QString& defSubs)
{
   this->mDefaultSubstitutions = defSubs;

   this->ui->titleLabel->setSubstitutionsProperty (defSubs);

   this->ui->vsMtrPosnMon->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->vsMtrPosnSet->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->vsMtrTweakFwd->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->vsMtrTweakRev->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->vsMtrTweakVal->setVariableNameSubstitutionsProperty (defSubs);

   this->ui->vcMtrPosnMon->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->vcMtrPosnSet->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->vcMtrTweakFwd->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->vcMtrTweakRev->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->vcMtrTweakVal->setVariableNameSubstitutionsProperty (defSubs);

   this->ui->upperBlade->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->lowerBlade->setVariableNameSubstitutionsProperty (defSubs);
}

//------------------------------------------------------------------------------
//
QString VCLVerticalSlits::getDefaultSubstitutions () const
{
    return this->mDefaultSubstitutions;
}

//------------------------------------------------------------------------------
//
void VCLVerticalSlits::setPrecision (const int precision)
{
   this->mPrecision = precision;
   this->ui->vsMtrPosnMon->setPrecision (precision);
   this->ui->vcMtrPosnMon->setPrecision (precision);
   this->ui->upperBlade->setPrecision (precision);
   this->ui->lowerBlade->setPrecision (precision);
}

//------------------------------------------------------------------------------
//
int VCLVerticalSlits::getPrecision () const
{
   return this->mPrecision;
}


//------------------------------------------------------------------------------
//
void VCLVerticalSlits::setUseDbPrecision (const bool useDbPrecision)
{
   this->mUseDbPrecision = useDbPrecision;
   this->ui->vsMtrPosnMon->setUseDbPrecision (useDbPrecision);
   this->ui->vcMtrPosnMon->setUseDbPrecision (useDbPrecision);
   this->ui->upperBlade->setUseDbPrecision (useDbPrecision);
   this->ui->lowerBlade->setUseDbPrecision (useDbPrecision);
}

//------------------------------------------------------------------------------
//
bool VCLVerticalSlits::getUseDbPrecision () const
{
   return this->mUseDbPrecision;
}

//------------------------------------------------------------------------------
//
void VCLVerticalSlits::setAddUnits (const bool addUnits)
{
   this->mAddUnits = addUnits;
   this->ui->vsMtrPosnMon->setAddUnits (addUnits);
   this->ui->vcMtrPosnMon->setAddUnits (addUnits);
   this->ui->upperBlade->setAddUnits (addUnits);
   this->ui->lowerBlade->setAddUnits (addUnits);
}

//------------------------------------------------------------------------------
//
bool VCLVerticalSlits::getAddUnits () const
{
   return this->mAddUnits;
}

//------------------------------------------------------------------------------
// slots
//------------------------------------------------------------------------------
//
void VCLVerticalSlits::vsMtrPosnChanged (const double& size)
{
   this->vSize = size;
   this->calcFractionalPositions ();
}

//------------------------------------------------------------------------------
//
void VCLVerticalSlits::vcMtrPosnChanged (const double& centre)
{
   this->vCentre = centre;
   this->calcFractionalPositions ();
}

// end
