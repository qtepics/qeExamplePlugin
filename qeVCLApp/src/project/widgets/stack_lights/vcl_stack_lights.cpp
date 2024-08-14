/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/stack_lights/vcl_stack_lights.cpp $
 * $Revision: #4 $
 * $DateTime: 2024/02/02 17:10:15 $
 * Last checked in by: $Author: starritt $
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2021-2024 Australian Synchrotron
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

#include "vcl_stack_lights.h"
#include <QBrush>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QEEnums.h>
#include <QECommon.h>
#include <QEInteger.h>

#define DEBUG qDebug () << "vcl_stack_lights" << __LINE__ << __FUNCTION__ << "  "

#define MAIN_PV_INDEX   0

//------------------------------------------------------------------------------
//
VCLStackLights::VCLStackLights (QWidget* parent) :
   QEAbstractWidget (parent),
   QESingleVariableMethods (this, MAIN_PV_INDEX)

{
   this->onColour[spTop]  = QColor (255, 100, 100);  // red
   this->onColour[spMid]  = QColor (255, 200, 100);  // amber/yellow
   this->onColour[spBot]  = QColor (100, 220, 100);  // green

   this->offColour[spTop] = QColor (100,   0,   0);  // dark red
   this->offColour[spMid] = QColor (100,  50,   0);  // dark amber/yellow
   this->offColour[spBot] = QColor (  0, 100,   0);  // dark green

   this->isConnected = false;
   this->isValid = false;
   this->currentValue = 0;

   this->setMinimumSize (12, 24);

   this->setNumVariables (1);
   this->setVariableAsToolTip (true);
   this->setAllowDrop (false);
   this->setDisplayAlarmStateOption (QE::WhenInvalid);

   // Use default context menu.
   //
   this->setupContextMenu ();

   // Set up a connection to recieve variable name property changes
   // The variable name property manager class only delivers an updated
   // variable name after the user has stopped typing.
   //
   this->connectNewVariableNameProperty
         (SLOT (useNewPvName (QString, QString, unsigned int)));
}

//------------------------------------------------------------------------------
//
VCLStackLights::~VCLStackLights () { }

//------------------------------------------------------------------------------
//
QSize VCLStackLights::sizeHint () const
{
   return QSize (24, 48);
}

//------------------------------------------------------------------------------
//
void VCLStackLights::paintEvent (QPaintEvent*)
{
   QPainter painter (this);
   QPen pen;
   QBrush brush;

   const int w = this->width() - 2;
   const int h = this->height() / 3;
   const int h3 = this->height() - (2*h) - 1;

   QRect r1 = QRect (1,  1,  w, h);
   QRect r2 = QRect (1,  h,  w, h);
   QRect r3 = QRect (1, 2*h, w, h3);

   QColor penColour;
   QColor color [NUMBER_OF_LIGHTS];

   // Determine each colour.
   //
   for (int j = 0, m = 1; j < NUMBER_OF_LIGHTS; j++, m <<= 1) {
      if (this->isConnected) {
         penColour = QColor (0, 0, 0);  // black
         if (this->isValid) {
            if ((this->currentValue & m) == m)  {
               // On - use base colour.
               color [j] = this->onColour [j];
            }  else {
               // Off - double dark
               color [j] = this->offColour [j];
            }
         } else {
            // Invalid - use the standard alarm colour for rectangle.
            //
            penColour = QColor (255, 128, 0);  // orange
            color [j] = this->alarmColour;
         }
      } else {
         // Disconnected - grayed out.
         //
         penColour = QColor (128, 128, 128);  // gray
         color [j] = QEUtilities::blandColour (this->onColour [j]);
      }
   }

   pen.setColor (penColour);
   pen.setWidth (2);
   pen.setStyle (Qt::SolidLine);
   painter.setPen (pen);

   brush.setStyle (Qt::SolidPattern);

   brush.setColor (color [spTop]);
   painter.setBrush (brush);
   painter.drawRect (r1);

   brush.setColor (color [spMid]);
   painter.setBrush (brush);
   painter.drawRect (r2);

   brush.setColor (color [spBot]);
   painter.setBrush (brush);
   painter.drawRect (r3);
}

//------------------------------------------------------------------------------
//
void VCLStackLights::establishConnection (unsigned int variableIndex)
{
   if (variableIndex != MAIN_PV_INDEX) return;  // sanity check

   // Create a connection.
   // If successfull, the QCaObject object that will supply data update signals will be returned
   // Note createConnection creates the connection and returns reference to existing QCaObject.
   //
   qcaobject::QCaObject* qca = this->createConnection (variableIndex);

   // If a QCaObject object is now available to supply data update signals,
   // connect it to the appropriate slots.
   //
   if (qca) {
      QObject::connect (qca,  SIGNAL (connectionChanged (QCaConnectionInfo&, const unsigned int &)),
                        this, SLOT   (connectionChanged (QCaConnectionInfo&, const unsigned int &)));

      QObject::connect (qca,  SIGNAL (integerChanged (const long&, QCaAlarmInfo&, QCaDateTime&, const unsigned int&)),
                        this, SLOT   (valueChanged   (const long&, QCaAlarmInfo&, QCaDateTime&, const unsigned int&)));
   }
}

//------------------------------------------------------------------------------
//
qcaobject::QCaObject* VCLStackLights::createQcaItem (unsigned int variableIndex)
{
   if (variableIndex != MAIN_PV_INDEX) return NULL;  // sanity check

   qcaobject::QCaObject* result;
   result = new QEInteger (this->getSubstitutedVariableName (variableIndex),
                           this, &this->integerFormatting, variableIndex);

   // Apply currently defined array index/elements request values.
   //
   this->setSingleVariableQCaProperties (result);

   return result;
}

//------------------------------------------------------------------------------
//
QString VCLStackLights::copyVariable ()
{
   QString result;
   result = this->getSubstitutedVariableName (MAIN_PV_INDEX);
   return result;
}

//------------------------------------------------------------------------------
//
QVariant VCLStackLights::copyData ()
{
   QVariant result;
   if (this->isConnected) {
      result = QVariant (this->currentValue);
   }
   return result;
}

//------------------------------------------------------------------------------
//
void VCLStackLights::paste (QVariant s)
{
   // s.toSring is a bit limiting when s is a StringList or a List of Strings, so
   // use common variantToStringList function which handles these options.
   //
   QStringList pvNameList = QEUtilities::variantToStringList (s);
   if (pvNameList.count() >= 1) {
      this->setVariableName (pvNameList.value (0, ""), MAIN_PV_INDEX);
      this->establishConnection (MAIN_PV_INDEX);
   }
}

//------------------------------------------------------------------------------
// slots
//------------------------------------------------------------------------------
//
void VCLStackLights::connectionChanged (QCaConnectionInfo& connectionInfo,
                                        const unsigned int &variableIndex)
{
   if (variableIndex != MAIN_PV_INDEX) return;  // sanity check

   // Note the connected state
   //
   this->isConnected = connectionInfo.isChannelConnected();

   if (!this->isConnected) {
      this->update ();
   }

   // Display the connected state
   //
   this->updateToolTipConnection (this->isConnected, variableIndex);

   this->emitDbConnectionChanged (variableIndex);
}

//------------------------------------------------------------------------------
//
void VCLStackLights::valueChanged (const long& value, QCaAlarmInfo& alarmInfo,
                                   QCaDateTime&, const unsigned int& variableIndex)
{
   if (variableIndex != MAIN_PV_INDEX) return;  // sanity check

   this->isValid = !alarmInfo.isInvalid();
   this->currentValue = value;
   this->alarmColour = QColor (alarmInfo.getColorName());
   this->update ();

   // Invoke tool tip handling directly. We don't want to interfere with the style
   // as widget draws it's own stuff with own, possibly clear, colours.
   //
   this->processAlarmInfo (alarmInfo, variableIndex);

   // Signal a database value change to any Link (or other) widgets using one
   // of the dbValueChanged (for main variable only).
   //
   this->emitDbValueChanged (variableIndex);
}

//------------------------------------------------------------------------------
//
void VCLStackLights::useNewPvName (QString variableNameIn,
                                   QString substitutionsIn,
                                   unsigned int variableIndex)
{
   if (variableIndex != MAIN_PV_INDEX) return;  // sanity check
   this->setVariableNameAndSubstitutions (variableNameIn, substitutionsIn, variableIndex);
}

// end
