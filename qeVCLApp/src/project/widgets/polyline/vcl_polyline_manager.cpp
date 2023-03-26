/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/polyline/vcl_polyline_manager.cpp $
 * $Revision: #1 $
 * $DateTime: 2023/03/10 12:25:22 $
 * Last checked in by: $Author: starritt $
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2021 Australian Synchrotron
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

#include "vcl_polyline_manager.h"
#include <QtDesigner>
#include <QtPlugin>
#include "vcl_polyline.h"
#include "vcl_polyline_setup_dialog.h"

//QE_IMPLEMENT_PLUGIN_MANAGER
//   (VCLPolyline, "AS VCL", ":/vcl/polyline/vcl_polyline.png", false)


//==============================================================================
// VCLPolylineManager
//==============================================================================
//
VCLPolylineManager::VCLPolylineManager (QObject * parent) : QObject (parent)
{
   initialized = false;
}

//------------------------------------------------------------------------------
//
void VCLPolylineManager::initialize (QDesignerFormEditorInterface* formEditor)
{
   if (initialized) {
      return;
   }

   QExtensionManager* manager = formEditor->extensionManager ();
   Q_ASSERT (manager != 0);

   manager->registerExtensions (new VCLPolylineTaskMenuFactory (manager),
                                Q_TYPEID (QDesignerTaskMenuExtension));

   initialized = true;
}

//------------------------------------------------------------------------------
//
bool VCLPolylineManager::isInitialized () const
{
   return initialized;
}

//------------------------------------------------------------------------------
// Widget factory. Creates a VCLPolyline widget.
//
QWidget *VCLPolylineManager::createWidget (QWidget * parent)
{
   return new VCLPolyline (parent);
}

//------------------------------------------------------------------------------
// Name for widget. Used by Qt Designer in widget list.
//
QString VCLPolylineManager::name () const
{
   return "VCLPolyline";
}

//------------------------------------------------------------------------------
// Name of group Qt Designer will add widget to.
//
QString VCLPolylineManager::group () const
{
   return "AS VCL";
}

//------------------------------------------------------------------------------
// Icon for widget. Used by Qt Designer in widget list.
//
QIcon VCLPolylineManager::icon () const
{
   return QIcon (":/vcl/polyline/vcl_polyline.png");
}

//------------------------------------------------------------------------------
// Tool tip for widget. Used by Qt Designer in widget list.
//
QString VCLPolylineManager::toolTip () const
{
   return "VCL Polyline";
}

//------------------------------------------------------------------------------
//
QString VCLPolylineManager::whatsThis () const
{
   return "VCL Polyline";
}

//------------------------------------------------------------------------------
//
bool VCLPolylineManager::isContainer () const
{
   return false;
}

//------------------------------------------------------------------------------
//
QString VCLPolylineManager::includeFile () const
{
   return "VCLPolyline.h";
}


//==============================================================================
// VCLPolylineTaskMenu
//==============================================================================
//
VCLPolylineTaskMenu::VCLPolylineTaskMenu (VCLPolyline* polylineIn, QObject* parent)
   : QObject(parent)
{
   this->polyline = polylineIn;

   this->editUserInfoAction = new QAction (tr ("Edit Polyline..."), this);

   connect (this->editUserInfoAction, SIGNAL (triggered ()),
            this,                     SLOT   (editUserInfo ()));
}

//------------------------------------------------------------------------------
//
void VCLPolylineTaskMenu::editUserInfo ()
{
   // Note: the use of the polyline widget's parent as the dialogs parent.
   //
   QWidget* wp = qobject_cast <QWidget*> (this->polyline->parent ());
   VCLPolylineSetupDialog dialog (this->polyline, wp);
   dialog.exec ();
}

//------------------------------------------------------------------------------
//
QAction *VCLPolylineTaskMenu::preferredEditAction () const
{
   return this->editUserInfoAction;
}

//------------------------------------------------------------------------------
//
QList<QAction *> VCLPolylineTaskMenu::taskActions () const
{
   QList<QAction *> list;

   list.append (this->editUserInfoAction);
   return list;
}


//==============================================================================
// VCLPolylineTaskMenuFactory
//==============================================================================
//
VCLPolylineTaskMenuFactory::VCLPolylineTaskMenuFactory (QExtensionManager *parent)
   : QExtensionFactory (parent)
{
}

//------------------------------------------------------------------------------
//
QObject* VCLPolylineTaskMenuFactory::createExtension (QObject* object,
                                                      const QString& iid,
                                                      QObject* parent) const
{
   if (iid != Q_TYPEID (QDesignerTaskMenuExtension)) {
      return 0;
   }

   if (VCLPolyline *pp = qobject_cast<VCLPolyline*>(object)) {
      return new VCLPolylineTaskMenu (pp, parent);
   }

   return 0;
}

// end
