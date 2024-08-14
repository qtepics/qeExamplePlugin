/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/dblslits/vcl_dblslits.cpp $
 * $Revision: #8 $
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

#include "vcl_dblslits.h"
#include <ui_vcl_dblslits.h>
#include <QDebug>
#include <QEEnums.h>
#include <QECommon.h>

#define DEBUG qDebug () << "vcl_dblslits" << __LINE__ << __FUNCTION__ << "  "

// Static shared with horizonatl and vertical slits widgets
//
// Actual sufficies for motor record fields
//
static const QString sufficies [VCLDoubleSlits::NUMBER_OF_SUFFICIES] = {
   ".RBV", ".VAL", ".TWF", ".TWR", ".TWV"
};

// Test sufficies for non-motor record testing
//
static const QString testSufficies [VCLDoubleSlits::NUMBER_OF_SUFFICIES] = {
   "_MON", "_SP", "_TWF", "_TWR", "_TWV"
};

//------------------------------------------------------------------------------
// static
QString VCLDoubleSlits::suffix (const SuffixKinds kind)
{
   // return testSufficies [kind];
   return sufficies [kind];
}

//------------------------------------------------------------------------------
//
VCLDoubleSlits::VCLDoubleSlits (QWidget* parent) :
   QEAbstractWidget (parent),
   ui (new Ui::VCLDoubleSlits ())
{
   this->ui->setupUi (this);

   this->setMinimumSize (this->ui->mainFrame->minimumSize ());
   this->setMaximumSize (this->ui->mainFrame->maximumSize ());

   // Clear style sheets - just used at design time.
   //
   this->ui->slitFrame->setStyleSheet ("");
   this->ui->widgetTopLeft->setStyleSheet ("");
   this->ui->widgetTopRight->setStyleSheet ("");
   this->ui->widgetBottomLeft->setStyleSheet ("");
   this->ui->widgetBottomRight->setStyleSheet ("");

   this->ui->slitFrame->installEventFilter (this);

   this->mTitle = "Double Slits";
   this->ui->titleLabel->setLabelTextProperty (this->mTitle);
   this->mHsMotor = "";
   this->mHcMotor = "";
   this->mVsMotor = "";
   this->mVcMotor = "";
   this->mInnerMotor = "";
   this->mOuterMotor = "";
   this->mUpperMotor = "";
   this->mLowerMotor = "";
   this->mDefaultSubstitutions = "";

   this->mPrecision = 4;
   this->mUseDbPrecision = true;
   this->mAddUnits = true;

   // There are no real sensible defaults.
   // But at least these value are sane.
   //
   this->mNominalHorizontalGap = 20.0;
   this->mNominalVerticalGap = 20.0;
   this->hSize   = 5.0;
   this->hCentre = 0.0;
   this->vSize   = 5.0;
   this->vCentre = 0.0;

   this->innerFrac = 0.25;
   this->outerFrac = 0.25;
   this->upperFrac = 0.25;
   this->lowerFrac = 0.25;

   // Connect to virtual motor position changes.
   //
   QObject::connect (this->ui->hsMtrPosnMon, SIGNAL (dbValueChanged   (const double&)),
                     this,                   SLOT   (hsMtrPosnChanged (const double&)));

   QObject::connect (this->ui->hcMtrPosnMon, SIGNAL (dbValueChanged   (const double&)),
                     this,                   SLOT   (hcMtrPosnChanged (const double&)));

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
VCLDoubleSlits::~VCLDoubleSlits ()
{
   delete this->ui;
   this->ui = NULL;
}

//------------------------------------------------------------------------------
// This function provides behaviour somewhat similar to a vertical or horizontal
// layout.  It positions and sizes the internal widgets relative to the overall
// size of the VCLDoubleSlits widget itself.
//
void VCLDoubleSlits::setBladeLayout ()
{
   int bladeLength;
   QRect geo;

   const int sfw = this->ui->slitFrame->width();
   const int sfh = this->ui->slitFrame->height();

   // Essentially a third each.
   //
   const int frameWidth = (sfw - 9) / 3;
   const int bladeWidth = frameWidth;

   const int frameHeight = (sfh - 9) / 3;
   const int bladeHeight = frameHeight;

   // Position the frames containing labels, line edits and tweak buttons
   //
   geo.setTop (2);
   geo.setLeft (2);
   geo.setWidth (frameWidth);
   geo.setHeight (frameHeight);
   this->ui->widgetTopLeft->setGeometry(geo);

   geo.setTop (2);
   geo.setLeft (2 + frameWidth + 2 + bladeWidth);
   geo.setWidth (frameWidth);
   geo.setHeight (frameHeight);
   this->ui->widgetTopRight->setGeometry(geo);

   geo.setTop (2 + frameHeight + 2 + bladeHeight);
   geo.setLeft (2);
   geo.setWidth (frameWidth);
   geo.setHeight (frameHeight);
   this->ui->widgetBottomLeft->setGeometry(geo);

   geo.setTop (2 + frameHeight + 2 + bladeHeight);
   geo.setLeft (2 + frameWidth + 2 + bladeWidth);
   geo.setWidth (frameWidth);
   geo.setHeight (frameHeight);
   this->ui->widgetBottomRight->setGeometry(geo);

   // Do blades.
   //
   // Inner blade and label
   //
   bladeLength = (sfw - 4)*this->innerFrac;
   geo.setTop (2 + frameHeight + 2);
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
   geo.setTop (2 + frameHeight + 2);
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

   // Upper blade
   //
   bladeLength = (sfh - 4)*this->upperFrac;
   geo.setTop (2);
   geo.setLeft (2 + frameWidth + 2);
   geo.setWidth (bladeWidth);
   geo.setHeight (bladeLength);
   this->ui->upperBlade->setGeometry (geo);

   // Lower blade
   //
   bladeLength = (sfh - 4)*this->lowerFrac;
   geo.setTop (sfh - (bladeLength + 3));
   geo.setLeft (2 + frameWidth + 2);
   geo.setWidth (bladeWidth);
   geo.setHeight (bladeLength);
   this->ui->lowerBlade->setGeometry (geo);
}

//------------------------------------------------------------------------------
// Maybe we should split this function into separate horizontal and vertical
// functions as these are independent.
//
void VCLDoubleSlits::calcFractionalPositions ()
{
   static const double minF = 0.05;
   static const double maxF = 0.95;

   double posn;
   double frac;

   // NOTE:
   // +x-axis is toward the right hand side, i.e. toward larger pixel x value
   // +y-axis is toward the top of widget, i.e.toward smaller pixel y value
   //
   // Calculate posn, the blade edge position, relative to centre. Then calc the
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
bool VCLDoubleSlits::eventFilter (QObject *watched, QEvent *event)
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
void VCLDoubleSlits::setTitle (const QString& title)
{
   this->mTitle = title;
   this->ui->titleLabel->setLabelTextProperty (this->mTitle);
}

//------------------------------------------------------------------------------
//
QString VCLDoubleSlits::getTitle () const
{
   return this->mTitle;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setNominalHorizontalGap (const double nominalHorizonatlGap)
{
   this->mNominalHorizontalGap = MAX (1.0e-6, nominalHorizonatlGap);
   this->calcFractionalPositions ();
}

//------------------------------------------------------------------------------
//
double VCLDoubleSlits::getNominalHorizontalGap () const
{
   return this->mNominalHorizontalGap;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setNominalVerticalGap (const double nominalVerticalGap)
{
   this->mNominalVerticalGap = MAX (1.0e-6, nominalVerticalGap);
   this->calcFractionalPositions ();
}

//------------------------------------------------------------------------------
//
double VCLDoubleSlits::getNominalVerticalGap () const
{
   return this->mNominalVerticalGap;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setHsMotor (const QString& hsMotor)
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
QString VCLDoubleSlits::getHsMotor () const
{
   return this->mHsMotor;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setHcMotor (const QString& hcMotor)
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
QString VCLDoubleSlits::getHcMotor () const
{
   return this->mHcMotor;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setVsMotor (const QString& vsMotor)
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
QString VCLDoubleSlits::getVsMotor () const
{
   return this->mVsMotor;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setVcMotor (const QString& vcMotor)
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
QString VCLDoubleSlits::getVcMotor () const
{
   return this->mVcMotor;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setInnerMotor (const QString& innerMotor)
{
   this->mInnerMotor = innerMotor;
   const QString pvName = innerMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon);
   this->ui->innerBlade->setVariableNameProperty (pvName);
   this->ui->innerBlade->setEdgeVariableNameProperty (pvName);
}

//------------------------------------------------------------------------------
//
QString VCLDoubleSlits::getInnerMotor () const
{
   return this->mInnerMotor;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setOuterMotor (const QString& outerMotor)
{
   this->mOuterMotor = outerMotor;
   const QString pvName = outerMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon);
   this->ui->outerBlade->setVariableNameProperty (pvName);
   this->ui->outerBlade->setEdgeVariableNameProperty (pvName);
}

//------------------------------------------------------------------------------
//
QString VCLDoubleSlits::getOuterMotor () const
{
   return this->mOuterMotor;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setUpperMotor (const QString& upperMotor)
{
   this->mUpperMotor = upperMotor;
   const QString pvName = upperMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon);
   this->ui->upperBlade->setVariableNameProperty (pvName);
   this->ui->upperBlade->setEdgeVariableNameProperty (pvName);
}

//------------------------------------------------------------------------------
//
QString VCLDoubleSlits::getUpperMotor () const
{
   return this->mUpperMotor;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setLowerMotor (const QString& lowerMotor)
{
   this->mLowerMotor = lowerMotor;
   const QString pvName = lowerMotor + VCLDoubleSlits::suffix (VCLDoubleSlits::PosnMon);
   this->ui->lowerBlade->setVariableNameProperty (pvName);
   this->ui->lowerBlade->setEdgeVariableNameProperty (pvName);
}

//------------------------------------------------------------------------------
//
QString VCLDoubleSlits::getLowerMotor () const
{
   return this->mLowerMotor;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setDefaultSubstitutions (const QString& defSubs)
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

   this->ui->innerBlade->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->outerBlade->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->upperBlade->setVariableNameSubstitutionsProperty (defSubs);
   this->ui->lowerBlade->setVariableNameSubstitutionsProperty (defSubs);
}

//------------------------------------------------------------------------------
//
QString VCLDoubleSlits::getDefaultSubstitutions () const
{
    return this->mDefaultSubstitutions;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setPrecision (const int precision)
{
   this->mPrecision = precision;
   this->ui->hsMtrPosnMon->setPrecision (precision);
   this->ui->hcMtrPosnMon->setPrecision (precision);
   this->ui->vsMtrPosnMon->setPrecision (precision);
   this->ui->vcMtrPosnMon->setPrecision (precision);
   this->ui->innerBlade->setPrecision (precision);
   this->ui->outerBlade->setPrecision (precision);
   this->ui->upperBlade->setPrecision (precision);
   this->ui->lowerBlade->setPrecision (precision);
}

//------------------------------------------------------------------------------
//
int VCLDoubleSlits::getPrecision () const
{
   return this->mPrecision;
}


//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setUseDbPrecision (const bool useDbPrecision)
{
   this->mUseDbPrecision = useDbPrecision;
   this->ui->hsMtrPosnMon->setUseDbPrecision (useDbPrecision);
   this->ui->hcMtrPosnMon->setUseDbPrecision (useDbPrecision);
   this->ui->vsMtrPosnMon->setUseDbPrecision (useDbPrecision);
   this->ui->vcMtrPosnMon->setUseDbPrecision (useDbPrecision);
   this->ui->innerBlade->setUseDbPrecision (useDbPrecision);
   this->ui->outerBlade->setUseDbPrecision (useDbPrecision);
   this->ui->upperBlade->setUseDbPrecision (useDbPrecision);
   this->ui->lowerBlade->setUseDbPrecision (useDbPrecision);
}

//------------------------------------------------------------------------------
//
bool VCLDoubleSlits::getUseDbPrecision () const
{
   return this->mUseDbPrecision;
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::setAddUnits (const bool addUnits)
{
   this->mAddUnits = addUnits;
   this->ui->hsMtrPosnMon->setAddUnits (addUnits);
   this->ui->hcMtrPosnMon->setAddUnits (addUnits);
   this->ui->vsMtrPosnMon->setAddUnits (addUnits);
   this->ui->vcMtrPosnMon->setAddUnits (addUnits);
   this->ui->innerBlade->setAddUnits (addUnits);
   this->ui->outerBlade->setAddUnits (addUnits);
   this->ui->upperBlade->setAddUnits (addUnits);
   this->ui->lowerBlade->setAddUnits (addUnits);
}

//------------------------------------------------------------------------------
//
bool VCLDoubleSlits::getAddUnits () const
{
   return this->mAddUnits;
}

//------------------------------------------------------------------------------
// slots
//------------------------------------------------------------------------------
//
void VCLDoubleSlits::hsMtrPosnChanged (const double& size)
{
   this->hSize = size;
   this->calcFractionalPositions ();
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::hcMtrPosnChanged (const double& centre)
{
   this->hCentre = centre;
   this->calcFractionalPositions ();
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::vsMtrPosnChanged (const double& size)
{
   this->vSize = size;
   this->calcFractionalPositions ();
}

//------------------------------------------------------------------------------
//
void VCLDoubleSlits::vcMtrPosnChanged (const double& centre)
{
   this->vCentre = centre;
   this->calcFractionalPositions ();
}

// end
