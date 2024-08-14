/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/horizslits/vcl_horizslits.cpp $
 * $Revision: #7 $
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

#include "vcl_horizslits.h"
#include <ui_vcl_horizslits.h>
#include <QDebug>
#include <QEEnums.h>
#include <QECommon.h>
#include <vcl_dblslits.h>

#define DEBUG qDebug () << "vcl_horizslits" << __LINE__ << __FUNCTION__ << "  "

//------------------------------------------------------------------------------
//
VCLHorizontalSlits::VCLHorizontalSlits (QWidget* parent) :
   QEAbstractWidget (parent),
   ui (new Ui::VCLHorizontalSlits ())
{
   this->ui->setupUi (this);

   this->setMinimumSize (this->ui->mainFrame->minimumSize ());
   this->setMaximumSize (this->ui->mainFrame->maximumSize ());

   // Clear style sheets - just used at design time.
   //
   this->ui->slitFrame->setStyleSheet ("");
   this->ui->widgetTopLeft->setStyleSheet ("");
   this->ui->widgetTopRight->setStyleSheet ("");

   this->ui->slitFrame->installEventFilter (this);

   this->mTitle = "Horizontal Slits";
   this->ui->titleLabel->setLabelTextProperty (this->mTitle);
   this->mHsMotor = "";
   this->mHcMotor = "";
   this->mInnerMotor = "";
   this->mOuterMotor = "";
   this->mDefaultSubstitutions = "";

   this->mPrecision = 4;
   this->mUseDbPrecision = true;
   this->mAddUnits = true;

   // There are no real sensible defaults.
   // But at least these value are sane.
   //
   this->mNominalHorizontalGap = 20.0;
   this->hSize   = 5.0;
   this->hCentre = 0.0;

   this->innerFrac = 0.25;
   this->outerFrac = 0.25;

   // Connect to virtual motor position changes.
   //
   QObject::connect (this->ui->hsMtrPosnMon, SIGNAL (dbValueChanged   (const double&)),
                     this,                   SLOT   (hsMtrPosnChanged (const double&)));

   QObject::connect (this->ui->hcMtrPosnMon, SIGNAL (dbValueChanged   (const double&)),
                     this,                   SLOT   (hcMtrPosnChanged (const double&)));

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
VCLHorizontalSlits::~VCLHorizontalSlits ()
{
   delete this->ui;
   this->ui = NULL;
}

//------------------------------------------------------------------------------
// This function provides behaviour somewhat similar to a vertical or horizontal
// layout.  It positions and sizes the internal widgets relative to the overall
// size of the VCLHorizontalSlits widget itself.
//
void VCLHorizontalSlits::setBladeLayout ()
{
   int bladeLength;
   QRect geo;

   const int sfw = this->ui->slitFrame->width();
   const int sfh = this->ui->slitFrame->height();

   // Essentially a third each.
   //
   const int frameWidth = (sfw - 9) / 3;
   const int bladeWidth = frameWidth;

   const int frameHeight = (sfh - 9) / 2;
   const int bladeHeight = frameHeight;

   // Position the frames containing labels, line edits and tweak buttons
   //
   geo.setTop (2 + frameHeight + 2);
   geo.setLeft (2);
   geo.setWidth (frameWidth);
   geo.setHeight (frameHeight);
   this->ui->widgetTopLeft->setGeometry(geo);

   geo.setTop (2 + frameHeight + 2);
   geo.setLeft (2 + frameWidth + 2 + bladeWidth);
   geo.setWidth (frameWidth);
   geo.setHeight (frameHeight);
   this->ui->widgetTopRight->setGeometry(geo);

   // Do blades.
   //
   // Inner blade and label
   //
   bladeLength = (sfw - 4)*this->innerFrac;
   geo.setTop (2);
   geo.setLeft (2);
   geo.setWidth (bladeLength);
   geo.setHeight (bladeHeight);
   this->ui->innerBlade->setGeometry (geo);

   // Modify the blade geometry to suit the inner label.
   //
   geo.setTop (geo.top() + 2);
   geo.setLeft (geo.left() + 2);
   geo.setWidth (this->ui->innerLabel->width());
   geo.setHeight (this->ui->innerLabel->height());
   this->ui->innerLabel->setGeometry (geo);

   // Outer blade and label
   //
   bladeLength = (sfw - 4)*this->outerFrac;
   geo.setTop (2);
   geo.setLeft (sfw - (bladeLength + 3));
   geo.setWidth (bladeLength);
   geo.setHeight (bladeHeight);
   this->ui->outerBlade->setGeometry (geo);

   // Modify the blade geometry to suit the outer label.
   //
   geo.setTop (geo.top() + 2);
   geo.setLeft (geo.right() - this->ui->innerLabel->width() - 2);
   geo.setWidth (this->ui->outerLabel->width());
   geo.setHeight (this->ui->outerLabel->height());
   this->ui->outerLabel->setGeometry (geo);
}

//------------------------------------------------------------------------------
// Maybe we should split this function into separate horizontal and vertical
// functions as these are independent.
//
void VCLHorizontalSlits::calcFractionalPositions ()
{
   static const double minF = 0.05;
   static const double maxF = 0.95;

   double posn;
   double frac;

   // NOTE:
   // +x-axis is toward the right hand side, i.e. toward larger pixel x value
   // +y-axis is toward the top of widget, i.e.toward smaller pixel y value
   //
   // Calculate ppsn, the blade edge position, relative to centre. Then calc the
   // factional postion of blade edge from widget edge towards the widget centre.
   // The edge has a fractional postion of 0.0 (or 0%)
   // The centre has a fractional postion of 0.5 (or 50%)
   // The far/other edge has a fractional postion of 1.0 (or 100%)
   //
   // We want the nominal gap to span from 25% to 75%
   //
   posn = this->hCentre - (this->hSize / 2.0);
   frac = 0.5 + (0.5 * posn / this->mNominalHorizontalGap);
   this->innerFrac = LIMIT (frac, minF, maxF);

   posn = this->hCentre + (this->hSize / 2.0);
   frac = 0.5 - (0.5 * posn / this->mNominalHorizontalGap);
   this->outerFrac = LIMIT (frac, minF, maxF);

   this->setBladeLayout ();
}

//------------------------------------------------------------------------------
//
bool VCLHorizontalSlits::eventFilter (QObject *watched, QEvent *event)
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
void VCLHorizontalSlits::setTitle (const QString& title)
{
   this->mTitle = title;
   this->ui->titleLabel->setLabelTextProperty (this->mTitle);
}

//------------------------------------------------------------------------------
//
QString VCLHorizontalSlits::getTitle () const
{
   return this->mTitle;
}

//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::setNominalHorizontalGap (const double nominalHorizonatlGap)
{
   this->mNominalHorizontalGap = MAX (1.0e-6, nominalHorizonatlGap);
   this->calcFractionalPositions ();
}

//------------------------------------------------------------------------------
//
double VCLHorizontalSlits::getNominalHorizontalGap () const
{
   return this->mNominalHorizontalGap;
}

//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::setHsMotor (const QString& hsMotor)
{
   this->mHsMotor = hsMotor;
   this->ui->hsMtrPosnMon->setVariableNameProperty  (hsMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon));
   this->ui->hsMtrPosnSet->setVariableNameProperty  (hsMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnSet));
   this->ui->hsMtrTweakFwd->setVariableNameProperty (hsMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakFwd));
   this->ui->hsMtrTweakRev->setVariableNameProperty (hsMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakRev));
   this->ui->hsMtrTweakVal->setVariableNameProperty (hsMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakVal));
}

//------------------------------------------------------------------------------
//
QString VCLHorizontalSlits::getHsMotor () const
{
   return this->mHsMotor;
}

//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::setHcMotor (const QString& hcMotor)
{
   this->mHcMotor = hcMotor;
   this->ui->hcMtrPosnMon->setVariableNameProperty  (hcMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon));
   this->ui->hcMtrPosnSet->setVariableNameProperty  (hcMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnSet));
   this->ui->hcMtrTweakFwd->setVariableNameProperty (hcMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakFwd));
   this->ui->hcMtrTweakRev->setVariableNameProperty (hcMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakRev));
   this->ui->hcMtrTweakVal->setVariableNameProperty (hcMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::TweakVal));
}

//------------------------------------------------------------------------------
//
QString VCLHorizontalSlits::getHcMotor () const
{
   return this->mHcMotor;
}

//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::setInnerMotor (const QString& innerMotor)
{
   this->mInnerMotor = innerMotor;
   const QString pvName = innerMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon);
   this->ui->innerBlade->setVariableNameProperty (pvName);
   this->ui->innerBlade->setEdgeVariableNameProperty (pvName);
}

//------------------------------------------------------------------------------
//
QString VCLHorizontalSlits::getInnerMotor () const
{
   return this->mInnerMotor;
}

//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::setOuterMotor (const QString& outerMotor)
{
   this->mOuterMotor = outerMotor;
   const QString pvName = outerMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon);
   this->ui->outerBlade->setVariableNameProperty (pvName);
   this->ui->outerBlade->setEdgeVariableNameProperty (pvName);
}

//------------------------------------------------------------------------------
//
QString VCLHorizontalSlits::getOuterMotor () const
{
   return this->mOuterMotor;
}

//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::setDefaultSubstitutions (const QString& defSubs)
{
   this->mDefaultSubstitutions = defSubs;

   this->ui->titleLabel->setSubstitutionsProperty (defSubs);

   this->ui->hsMtrPosnMon->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->hsMtrPosnSet->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->hsMtrTweakFwd->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->hsMtrTweakRev->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->hsMtrTweakVal->setVariableNameSubstitutionsProperty (defSubs);

   this->ui->hcMtrPosnMon->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->hcMtrPosnSet->setVariableNameSubstitutionsProperty  (defSubs);
   this->ui->hcMtrTweakFwd->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->hcMtrTweakRev->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->hcMtrTweakVal->setVariableNameSubstitutionsProperty (defSubs);

   this->ui->innerBlade->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->outerBlade->setVariableNameSubstitutionsProperty (defSubs);
}

//------------------------------------------------------------------------------
//
QString VCLHorizontalSlits::getDefaultSubstitutions () const
{
    return this->mDefaultSubstitutions;
}

//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::setPrecision (const int precision)
{
   this->mPrecision = precision;
   this->ui->hsMtrPosnMon->setPrecision (precision);
   this->ui->hcMtrPosnMon->setPrecision (precision);
   this->ui->innerBlade->setPrecision (precision);
   this->ui->outerBlade->setPrecision (precision);
}

//------------------------------------------------------------------------------
//
int VCLHorizontalSlits::getPrecision () const
{
   return this->mPrecision;
}


//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::setUseDbPrecision (const bool useDbPrecision)
{
   this->mUseDbPrecision = useDbPrecision;
   this->ui->hsMtrPosnMon->setUseDbPrecision (useDbPrecision);
   this->ui->hcMtrPosnMon->setUseDbPrecision (useDbPrecision);
//   this->ui->vsMtrPosnMon->setUseDbPrecision (useDbPrecision);
//   this->ui->vcMtrPosnMon->setUseDbPrecision (useDbPrecision);
   this->ui->innerBlade->setUseDbPrecision (useDbPrecision);
   this->ui->outerBlade->setUseDbPrecision (useDbPrecision);
//   this->ui->upperBlade->setUseDbPrecision (useDbPrecision);
//   this->ui->lowerBlade->setUseDbPrecision (useDbPrecision);
}

//------------------------------------------------------------------------------
//
bool VCLHorizontalSlits::getUseDbPrecision () const
{
   return this->mUseDbPrecision;
}

//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::setAddUnits (const bool addUnits)
{
   this->mAddUnits = addUnits;
   this->ui->hsMtrPosnMon->setAddUnits (addUnits);
   this->ui->hcMtrPosnMon->setAddUnits (addUnits);
   this->ui->innerBlade->setAddUnits (addUnits);
   this->ui->outerBlade->setAddUnits (addUnits);
}

//------------------------------------------------------------------------------
//
bool VCLHorizontalSlits::getAddUnits () const
{
   return this->mAddUnits;
}

//------------------------------------------------------------------------------
// slots
//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::hsMtrPosnChanged (const double& size)
{
   this->hSize = size;
   this->calcFractionalPositions ();
}

//------------------------------------------------------------------------------
//
void VCLHorizontalSlits::hcMtrPosnChanged (const double& centre)
{
   this->hCentre = centre;
   this->calcFractionalPositions ();
}

// end
