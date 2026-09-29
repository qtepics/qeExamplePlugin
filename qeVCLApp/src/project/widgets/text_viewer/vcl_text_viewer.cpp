/* Repo: tec_gui/qevcl
 * File: qeVCLApp/src/project/widgets/text_viewer/vcl_text_viewer.cpp
 * DateTime: Tue Sep 29 09:41:53 2026
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

#include "vcl_text_viewer.h"
#include <QEEnums.h>
#include <ui_vcl_text_viewer.h>

#include <QDebug>

#define DEBUG qDebug () << "vcl_text_viewer" << __LINE__ << __FUNCTION__ << "  "

#define MAIN_PV_INDEX   0

//------------------------------------------------------------------------------
//
VCLTextViewer::VCLTextViewer (QWidget* parent) :
   QEAbstractWidget (parent),
   QESingleVariableMethods (this, MAIN_PV_INDEX),
   ui (new Ui::VCLTextViewer ())
{
   this->ui->setupUi (this);

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
   this->connectPvNameProperties
         (SLOT (usePvNameProperties (const QEPvNameProperties&)));
}

//------------------------------------------------------------------------------
//
VCLTextViewer::~VCLTextViewer ()
{
   delete this->ui;
   this->ui = NULL;
}

//------------------------------------------------------------------------------
//
void VCLTextViewer::establishConnection (unsigned int variableIndex)
{
   if (variableIndex != MAIN_PV_INDEX) return;  // sanity check

   // Create a connection.
   // If successfull, the QEChannel object that will supply data update signals will be returned
   // Note createConnection creates the connection and returns reference to existing QEChannel.
   //
   QEChannel* qca = this->createConnection (variableIndex);

   // If a QEChannel object is now available to supply data update signals,
   // connect it to the appropriate slots.
   //
   if (qca) {
      QObject::connect (qca,  SIGNAL (connectionUpdated (const QEConnectionUpdate&)),
                        this, SLOT   (connectionUpdated (const QEConnectionUpdate&)));

      QObject::connect (qca,  SIGNAL (valueUpdated (const QEStringValueUpdate&)),
                        this, SLOT   (valueUpdated (const QEStringValueUpdate&)));
   }

}

//------------------------------------------------------------------------------
//
QEChannel* VCLTextViewer::createQcaItem (unsigned int variableIndex)
{
   if (variableIndex != MAIN_PV_INDEX) return NULL;  // sanity check

   const QString pvName = this->getSubstitutedVariableName (variableIndex);
   QEChannel* result = new QEString (pvName, this, &this->stringFormatting, variableIndex);

   // Apply currently defined array index/elements request values.
   //
   this->setSingleVariableQCaProperties (result);

   return result;
}

//------------------------------------------------------------------------------
//
void VCLTextViewer::usePvNameProperties (const QEPvNameProperties& pvNameProperties)
{
   if (pvNameProperties.index != MAIN_PV_INDEX) return;  // sanity check

   this->setVariableNameAndSubstitutions (pvNameProperties.pvName,
                                          pvNameProperties.substitutions,
                                          pvNameProperties.index);
}

//------------------------------------------------------------------------------
//
void VCLTextViewer::connectionUpdated (const QEConnectionUpdate& update)
{
   const unsigned int vi = update.variableIndex;

   if (vi != MAIN_PV_INDEX) return;  // sanity check

   // Note the connected state
   //
   const bool isConnected = update.connectionInfo.isChannelConnected();

   // Display the connected state
   //
   this->updateToolTipConnection (isConnected, vi);

   this->emitDbConnectionChanged (vi);
}

//------------------------------------------------------------------------------
//
void VCLTextViewer::valueUpdated (const QEStringValueUpdate& update)
{
   const unsigned int vi = update.variableIndex;

   if (vi != MAIN_PV_INDEX) return;  // sanity check

   /// const bool isValid = !update.alarmInfo.isInvalid();

   // Set text and scroll to the bottom.
   //
   this->ui->plainTextEdit->setPlainText (update.value);
   this->ui->plainTextEdit->moveCursor(QTextCursor::End);
   this->ui->plainTextEdit->ensureCursorVisible();

   // Invoke tool tip handling directly. We don't want to interfere with the style
   // as widget draws it's own stuff with own, possibly clear, colours.
   //
   this->processAlarmInfo (update.alarmInfo, vi);

   // Signal a database value change to any Link (or other) widgets using one
   // of the dbValueChanged (for main variable only).
   //
   this->emitDbValueChanged (vi);
}

// end
