/* File: qeVCLApp/src/project/widgets/polyline/vcl_polyline.cpp
 * DateTime: Mon May 26 17:17:29 2025
 * Last checked in by: starritt
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

#include "vcl_polyline.h"
#include <qdom.h>
#include <QBrush>
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QECommon.h>

#define DEBUG qDebug () << "vcl_polyline" << __LINE__ << __FUNCTION__ << "  "

// XML tag/attribute names etc.
//
static const QString mainTagName      = "Polyline";
static const QString versionAttrName  = "Version";
static const QString pointTagName     = "point";

const static int maximumPoints = 200;

//------------------------------------------------------------------------------
//
VCLPolyline::VCLPolyline (QWidget* parent) :
   QFrame (parent),
   isDialogMode (false)
{
   this->setup ();
}

//------------------------------------------------------------------------------
//
VCLPolyline::VCLPolyline (const bool dialog,
                          QWidget* parent) :
   QFrame (parent),
   isDialogMode (dialog)

{
   this->setup ();
}

//------------------------------------------------------------------------------
//
void VCLPolyline::setup ()
{
   this->setMinimumSize (12, 12);
   this->isPolygon = false;

   this->points.clear();

   this->penColour = QColor (0, 0, 0, 255);
   this->penWidth = 1;
   this->penStyle = Qt::SolidLine;
   this->isPolygon = false;
   this->brushColour = QColor (255, 255, 255, 255);
   this->brushStyle = Qt::SolidPattern;
   this->gridSize = 1.0;

   this->selectedPoint = -1;
   this->hoverOverPoint = -1;

   this->setMouseTracking (this->isDialogMode);
}

//------------------------------------------------------------------------------
//
VCLPolyline::~VCLPolyline () { }

//------------------------------------------------------------------------------
//
QSize VCLPolyline::sizeHint() const
{
   return QSize (40, 40);
}

//------------------------------------------------------------------------------
//
void VCLPolyline::setPoints (const QPointList& pointsIn)
{
   this->points = pointsIn;
   this->snapToGrid ();
   this->update ();
   emit updateTable();
}

//------------------------------------------------------------------------------
//
VCLPolyline::QPointList VCLPolyline::getPoints () const
{
   return this->points;
}

//------------------------------------------------------------------------------
//
void VCLPolyline::paintEvent (QPaintEvent* event)
{
   QFrame::paintEvent (event);  // call parent function first.

   QPainter painter (this);
   QPen pen;
   QBrush brush;

   pen.setColor (this->penColour);
   pen.setWidth (this->penWidth);
   pen.setStyle (this->penStyle);
   painter.setPen (pen);

   brush.setColor (this->brushColour);
   brush.setStyle (this->brushStyle);
   painter.setBrush (brush);

   QPointF line [maximumPoints];
   const int n = this->points.count();

   for (int j = 0; j < n; j++) {
      QPointF p = this->points.value (j);
      line [j] = this->toPixelCoords (p);
   }

   if (this->isPolygon) {
      painter.drawPolygon (line, n);
   } else {
      painter.drawPolyline(line, n);
   }

   // Only applicable in designer using VCLPolylineSetupDialog.
   //
   if (this->isDialogMode) {
      pen.setColor (QColor (128, 128, 128));
      pen.setWidth (1);
      pen.setStyle (Qt::SolidLine);
      painter.setPen (pen);

      brush.setStyle (Qt::NoBrush);

      for (int j = 0; j < n; j++) {
         static const double size = 9.0;
         QPointF p = line [j];

         if (j == this->selectedPoint) {
            brush.setColor (QColor (255, 0, 0));
            brush.setStyle (Qt::SolidPattern);

         } else if (j == this->hoverOverPoint) {
            brush.setColor (QColor (255, 255, 0));
            brush.setStyle (Qt::SolidPattern);

         } else {
            brush.setStyle (Qt::NoBrush);
         }
         painter.setBrush (brush);

         painter.drawRect (p.x() - size/2, p.y() - size/2, size, size);
      }
   }
}

//------------------------------------------------------------------------------
//
void VCLPolyline::mousePressEvent (QMouseEvent*)
{
   if (!this->isDialogMode) return;

   this->selectedPoint = this->hoverOverPoint;
   this->hoverOverPoint = -1;
   this->update ();
}

//------------------------------------------------------------------------------
//
void VCLPolyline::mouseReleaseEvent (QMouseEvent*)
{
   if (!this->isDialogMode) return;

   this->hoverOverPoint = this->selectedPoint;
   this->selectedPoint = -1;
   this->update ();
}

//------------------------------------------------------------------------------
//
void VCLPolyline::mouseMoveEvent (QMouseEvent* event)
{
   if (!this->isDialogMode) return;

   if (this->selectedPoint >= 0) {
      // a point is selected
      //
      QPoint  p = event->pos();
      QPointF v = this->toPercentageCoords (p);
      this->points.replace (this->selectedPoint, v);
      this->update ();
      emit updateTable();

   } else {
      int closestPoint = -1;
      double minimumDistance2 = 1.0e+9;
      const int n = this->points.count();

      for (int j = 0; j < n; j++) {
         QPointF p = this->points.value (j);
         QPointF d = event->pos() - this->toPixelCoords (p);
         double dist2 = d.x()*d.x() + d.y()*d.y();
         if (dist2 < 36) {
            // Close enough.
            //
            if (dist2 < minimumDistance2) {
               // Better
               minimumDistance2 = dist2;
               closestPoint = j;
            }
         }
      }

      if (this->hoverOverPoint != closestPoint) {
         this->hoverOverPoint = closestPoint;
         this->update();
      }
   }
}

//------------------------------------------------------------------------------
// Serialise vertex points list into XML string.
//
QString VCLPolyline::encodeXml () const
{
   QDomDocument doc;
   QDomElement docElem;

   doc.clear ();
   docElem = doc.createElement (mainTagName);
   docElem.setAttribute (versionAttrName, 1);
   doc.appendChild (docElem);

   const int n = this->points.count();
   for (int j = 0; j < n; j++) {
      QPointF point = this->points.value(j);
      QDomElement pointElement;
      pointElement = doc.createElement (pointTagName);
      pointElement.setAttribute ("x", point.x());
      pointElement.setAttribute ("y", point.y());
      docElem.appendChild (pointElement);
   }

   // Remove newlines - designer's property manager does not handle properties
   // with many many lines very well.
   //
   return doc.toString ().replace ("\n", "");
}

//------------------------------------------------------------------------------
//
bool VCLPolyline::decodeXml (const QString& xml, QPointList& pl)
{
   QDomDocument doc;
   QString errorText;
   int errorLine;
   int errorCol;

   pl.clear();

   if (!doc.setContent (xml, false, &errorText, &errorLine, &errorCol)) {
      DEBUG << QString ("%1:%2").arg (errorLine).arg (errorCol)
            << " set content failed " << errorText;
      return false;
   }

   QDomElement docElem = doc.documentElement ();

   // Examine top level tag name - is this the tag we expect?
   //
   if (docElem.tagName () != mainTagName) {
      DEBUG  << "unexpected tag name " << docElem.tagName ();
      return false;
   }

   // Verify version.
   //
   QString versionImage = docElem.attribute (versionAttrName).trimmed ();
   bool versionOkay;
   int version = versionImage.toInt (&versionOkay);

   if (!versionOkay || (version != 1)) {
      DEBUG << "invalid/unexpected version specified: " << versionImage;
      return false;
   }

   QDomElement childElement = docElem.firstChildElement (pointTagName);
   while (!childElement.isNull ()) {
      QString attr;
      bool okay;

      attr = childElement.attribute ("x", "<>");
      double x = attr.toDouble (&okay);
      if (!okay) return false;

      attr = childElement.attribute ("y", "<>");
      double y = attr.toDouble (&okay);
      if (!okay) return false;

      pl.append (QPointF (x, y));

      if (pl.count() >= maximumPoints) {
         break;
      }

      childElement = childElement.nextSiblingElement (pointTagName);
   }

   return true;
}

//------------------------------------------------------------------------------
//
void VCLPolyline::snapToGrid ()
{
   if (this->gridSize > 0) {

      const int n = this->points.count();
      for (int j = 0; j < n; j++) {
         QPointF point = this->points.value(j);

         double x = point.x();
         double y = point.y();

         x = this->gridSize * qRound (x / this->gridSize);
         y = this->gridSize * qRound (y / this->gridSize);

         point = QPointF (x, y);
         this->points.replace(j, point);
      }

      this->update ();
      emit updateTable();
   }
}

//------------------------------------------------------------------------------
//
QPoint VCLPolyline::toPixelCoords (const QPointF pos)
{
   const int ew = this->width()  - this->penWidth;
   const int eh = this->height() - this->penWidth;
   const int offset = this->penWidth / 2;

   const double x = offset + ew * pos.x() / 100.0;  // % value
   const double y = offset + eh * pos.y() / 100.0;  // % value

   QPoint result = QPoint (qRound (x), qRound(y));

   return result;
}

//------------------------------------------------------------------------------
//
QPointF VCLPolyline::toPercentageCoords (const QPoint pos)
{
   const int ew = this->width()  - this->penWidth;
   const int eh = this->height() - this->penWidth;
   const int offset = this->penWidth / 2;

   double x = 100.0 * (pos.x() - offset) / ew;
   double y = 100.0 * (pos.y() - offset) / eh;

   if (this->gridSize > 0) {
      x = this->gridSize * qRound (x / this->gridSize);
      y = this->gridSize * qRound (y / this->gridSize);
   }

   QPointF result = QPointF (x, y);

   return result;
}

//------------------------------------------------------------------------------
// properties setters and getters
//------------------------------------------------------------------------------
//
void VCLPolyline::setPenColour (const QColor penColourIn)
{
   this->penColour = penColourIn;
   this->update();
}

//------------------------------------------------------------------------------
//
QColor VCLPolyline::getPenColour () const
{
   return this->penColour;
}

//------------------------------------------------------------------------------
//
void VCLPolyline::setPenWidth (const int penWidthIn)
{
   this->penWidth = LIMIT (penWidthIn, 0, 100);
   this->update();
}

//------------------------------------------------------------------------------
//
int VCLPolyline::getPenWidth () const
{
   return this->penWidth;
}

//------------------------------------------------------------------------------
//
void VCLPolyline::setPenStyle (const Qt::PenStyle penStyleIn)
{
   this->penStyle = penStyleIn;
   this->update();
}

//------------------------------------------------------------------------------
//
Qt::PenStyle VCLPolyline::getPenStyle () const
{
   return this->penStyle;
}

//------------------------------------------------------------------------------
//
void VCLPolyline::setIsPolygon (const bool isPolygonIn)
{
   this->isPolygon = isPolygonIn;
   this->update();
}

//------------------------------------------------------------------------------
//
bool VCLPolyline::getIsPolygon () const
{
   return this->isPolygon;
}

//------------------------------------------------------------------------------
//
void VCLPolyline::setBrushColour (const QColor brushColourIn)
{
   this->brushColour = brushColourIn;
   this->update();
}

//------------------------------------------------------------------------------
//
QColor VCLPolyline::getBrushColour () const
{
   return this->brushColour;
}

//------------------------------------------------------------------------------
//
void VCLPolyline::setBrushStyle (const  Qt::BrushStyle brushStyleIn)
{
   this->brushStyle = brushStyleIn;
   this->update();
}

//------------------------------------------------------------------------------
//
Qt::BrushStyle VCLPolyline::getBrushStyle () const
{
   return this->brushStyle;
}

//------------------------------------------------------------------------------
//
void VCLPolyline::setGridSize (const double gridSizeIn)
{
   // Zero means no grid size restriction.
   // Non-zero must be >= 0.01
   //
   const double tempGS = LIMIT (gridSizeIn, 0, 5);
   if (this->gridSize != tempGS) {
      this->gridSize = tempGS;

      if (this->gridSize > 0) {
         this->gridSize = MAX (this->gridSize, 0.01);
      }

      this->snapToGrid ();  // snap tp the new grid size
   }
}

//------------------------------------------------------------------------------
//
double VCLPolyline::getGridSize () const
{
   return this->gridSize;
}

//------------------------------------------------------------------------------
//
void VCLPolyline::setXml (const QString& xmlStringIn)
{
   const QString trimmedXmlString = xmlStringIn.trimmed();

   if (trimmedXmlString.isEmpty()) {
      // Special treatment for empty string
      //
      this->points.clear();
   } else {

      QPointList tempPoints;
      bool okay = this->decodeXml (trimmedXmlString, tempPoints);
      if (okay) {
         this->points = tempPoints;
         this->update();
         emit updateTable ();
      } else {
         DEBUG << "Invalid xml string";
      }
   }
}

//------------------------------------------------------------------------------
//
QString VCLPolyline::getXml () const
{
   return this->encodeXml ();
}

// end
