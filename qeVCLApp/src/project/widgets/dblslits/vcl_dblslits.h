/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/dblslits/vcl_dblslits.h $
 * $Revision: #5 $
 * $DateTime: 2023/02/16 11:27:26 $
 * Last checked in by: $Author: starritt $
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2020-2023 Australian Synchrotron
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

#ifndef VCL_DOUBLE_SLITS_H
#define VCL_DOUBLE_SLITS_H

#include <QObject>
#include <QWidget>
#include <QEAbstractWidget.h>
#include <visual_component_library_global.h>

namespace Ui {
   class VCLDoubleSlits;  // differed
}

class VISUAL_COMPONENT_LIBRARY_SHARED VCLDoubleSlits :
   public QEAbstractWidget
{
   Q_OBJECT

   /// These are hard coded for the name and nominal gaps
   /// May replace/augment with PVs.
   ///
   Q_PROPERTY (QString  title                 READ getTitle                 WRITE setTitle)
   Q_PROPERTY (double   nominalHorizontalGap  READ getNominalHorizontalGap  WRITE setNominalHorizontalGap)
   Q_PROPERTY (double   nominalVerticalGap    READ getNominalVerticalGap    WRITE setNominalVerticalGap)

   /// Coordinated motor names - we use .VAL, .RBV, .TWV, .TWR, and .TWF fields
   /// Horizonatal size
   ///
   Q_PROPERTY (QString  hsMotor     READ getHsMotor    WRITE setHsMotor)

   /// Horizonatal centre
   ///
   Q_PROPERTY (QString  hcMotor     READ getHcMotor    WRITE setHcMotor)

   /// Vertical size
   ///
   Q_PROPERTY (QString  vsMotor     READ getVsMotor    WRITE setVsMotor)

   /// Vertical centre
   ///
   Q_PROPERTY (QString  vcMotor     READ getVcMotor    WRITE setVcMotor)

   /// Physical motor names - we use .RBV field only.
   /// Inner
   ///
   Q_PROPERTY (QString  innerMotor  READ getInnerMotor WRITE setInnerMotor)

   /// Outer
   ///
   Q_PROPERTY (QString  outerMotor  READ getOuterMotor WRITE setOuterMotor)

   /// Upper
   Q_PROPERTY (QString  upperMotor  READ getUpperMotor WRITE setUpperMotor)

   /// Lower
   ///
   Q_PROPERTY (QString  lowerMotor  READ getLowerMotor WRITE setLowerMotor)

   /// Default macro substitutions. The default is no substitutions.
   /// The format is NAME1=VALUE1[,] NAME2=VALUE2...
   /// Values may be quoted strings. For example, 'PUMP=PMP3, NAME = "My Pump"'
   /// These substitutions are applied to variable names for all QE widgets.
   /// In some widgets are are also used for other purposes.
   ///
   Q_PROPERTY (QString  defaultSubstitutions
               READ  getDefaultSubstitutions
               WRITE setDefaultSubstitutions)

   /// Precision used when formatting floating point numbers. The default is 4.
   /// This is only used if useDbPrecision is false.
   ///
   Q_PROPERTY (int  precision      READ getPrecision      WRITE setPrecision)

   /// If true (default), format floating point numbers using the precision supplied with the data.
   /// If false, the precision property is used.
   ///
   Q_PROPERTY (bool useDbPrecision READ getUseDbPrecision WRITE setUseDbPrecision)

   /// If true (default), add engineering units supplied with the data.
   ///
   Q_PROPERTY (bool addUnits       READ getAddUnits       WRITE setAddUnits)

public:
   friend class VCLHorizontalSlits;
   friend class VCLVerticalSlits;

   // Used internally by this widget and the VCLHorizontalSlits
   // and VCLVerticalSlits widgets.
   //
   enum SuffixKinds {
      PosnMon = 0,
      PosnSet,
      TweakFwd,
      TweakRev,
      TweakVal,
      NUMBER_OF_SUFFICIES
   };

   explicit VCLDoubleSlits (QWidget* parent = 0);
   ~VCLDoubleSlits ();

   // Property access functions.
   //
   void setTitle (const QString& title);
   QString getTitle () const;

   void setNominalHorizontalGap (const double nominalHorizonatlGap);
   double getNominalHorizontalGap () const;

   void setNominalVerticalGap (const double nominalVerticalGap);
   double getNominalVerticalGap () const;

   // These are essentially substitutioon property values.
   //
   void setHsMotor (const QString& hsMotor);
   QString getHsMotor () const;

   void setHcMotor (const QString& hcMotor);
   QString getHcMotor () const;

   void setVsMotor (const QString& vsMotor);
   QString getVsMotor () const;

   void setVcMotor (const QString& vcMotor);
   QString getVcMotor () const;

   void setInnerMotor (const QString& innerMotor);
   QString getInnerMotor () const;

   void setOuterMotor (const QString& outerMotor);
   QString getOuterMotor () const;

   void setUpperMotor (const QString& upperMotor);
   QString getUpperMotor () const;

   void setLowerMotor (const QString& lowerMotor);
   QString getLowerMotor () const;

   void setDefaultSubstitutions (const QString& defSubs);
   QString getDefaultSubstitutions () const;

   void setPrecision (const int precision);
   int getPrecision () const;

   void setUseDbPrecision (const bool useDbPrecision);
   bool getUseDbPrecision () const;

   void setAddUnits (const bool addUnits);
   bool getAddUnits () const;

protected:
   bool eventFilter (QObject* watched, QEvent* event);

private:
   void setBladeLayout ();
   void calcFractionalPositions ();

   // Common utilitty function.
   //
   static QString suffix (const SuffixKinds kind);

   Ui::VCLDoubleSlits* ui;

   // Property values
   //
   QString mTitle;
   double  mNominalHorizontalGap;
   double  mNominalVerticalGap;
   QString mHsMotor;
   QString mHcMotor;
   QString mVsMotor;
   QString mVcMotor;
   QString mInnerMotor;
   QString mOuterMotor;
   QString mUpperMotor;
   QString mLowerMotor;
   QString mDefaultSubstitutions;
   int  mPrecision;
   bool mUseDbPrecision;
   bool mAddUnits;

   // State values related to the blade positions.
   // These values are cached directly from the (virtual motor) PVs.
   //
   double hSize;
   double hCentre;
   double vSize;
   double vCentre;

   // These are the fraction positions to the blade edges towards the centre. A
   // fractional postion of 0.0 means the blade is drawn as if total extracted.
   // A fractional postion of 0.5 means the inner blade edge is drawn at the
   // centre of the widget.  Fractional postions are constrained to be in the
   // range 0.05 .. 0.95 for widget drawing/painting purposes.
   //
   double innerFrac;
   double outerFrac;
   double upperFrac;
   double lowerFrac;

private slots:
   void hsMtrPosnChanged (const double& size);
   void hcMtrPosnChanged (const double& centre);
   void vsMtrPosnChanged (const double& size);
   void vcMtrPosnChanged (const double& centre);
};

#endif  // VCL_DOUBLE_SLITS_H
