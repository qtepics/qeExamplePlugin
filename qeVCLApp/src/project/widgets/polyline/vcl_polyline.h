/* $File: //ASP/tec/gui/qevcl/trunk/qeVCLApp/src/project/widgets/polyline/vcl_polyline.h $
 * $Revision: #1 $
 * $DateTime: 2023/03/10 12:25:22 $
 * Last checked in by: $Author: starritt $
 *
 * This file is part of the EPICS Qt (QE) Visual Component Libaray (VCL)
 * developed at the Australian Synchrotron.
 *
 * Copyright (c) 2022 Australian Synchrotron
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

#ifndef VCL_POLYLINE_H
#define VCL_POLYLINE_H

#include <QColor>
#include <QObject>
#include <QFrame>
#include <QList>
#include <QMouseEvent>
#include <QPointF>
#include <QWidget>
#include <QCaObject.h>
#include <QCaAlarmInfo.h>
#include <visual_component_library_global.h>

///
//
class VISUAL_COMPONENT_LIBRARY_SHARED VCLPolyline :
      public QFrame
{
   Q_OBJECT

   Q_PROPERTY (QColor penColour          READ  getPenColour   WRITE setPenColour)
   Q_PROPERTY (int penWidth              READ  getPenWidth    WRITE setPenWidth)
   Q_PROPERTY (Qt::PenStyle penStyle     READ  getPenStyle    WRITE setPenStyle)
   Q_PROPERTY (bool isPolygon            READ  getIsPolygon   WRITE setIsPolygon)
   Q_PROPERTY (QColor brushColour        READ  getBrushColour WRITE setBrushColour)
   Q_PROPERTY (Qt::BrushStyle brushStyle READ  getBrushStyle  WRITE setBrushStyle)
   Q_PROPERTY (double gridSize           READ  getGridSize    WRITE setGridSize)

   // Specifies the vertix values, encoded as an XML string.
   // This property stores the user info.
   // The "Edit Ployline..." context menu item must be used to edit this.
   //
   Q_PROPERTY (QString xmlPoints         READ  getXml         WRITE setXml )

public:
   typedef QList<QPointF> QPointList;

   explicit VCLPolyline (QWidget* parent = 0);
   ~VCLPolyline ();

   QSize sizeHint() const override;

   void setPenColour (const QColor penColour);
   QColor getPenColour () const;

   void setPenWidth (const int penWidth);
   int getPenWidth () const;

   void setPenStyle (const Qt::PenStyle penStyle);
   Qt::PenStyle getPenStyle () const;

   void setIsPolygon (const bool isPolygon);
   bool getIsPolygon () const;

   void setBrushColour (const QColor brushColour);
   QColor getBrushColour () const;

   void setBrushStyle (const  Qt::BrushStyle brushStyle);
   Qt::BrushStyle getBrushStyle () const;

   void setGridSize (const double gridSize);
   double getGridSize () const;

   void setXml (const QString& xml);
   QString getXml () const;

   void setPoints (const QPointList& points);
   QPointList getPoints () const;

protected:
   void paintEvent (QPaintEvent* event) override;
   void mousePressEvent (QMouseEvent* event) override;
   void mouseReleaseEvent (QMouseEvent* event) override;
   void mouseMoveEvent (QMouseEvent* event) override;

private:
   void setup ();
   QString encodeXml () const;   // Serialise data into XML string.
   bool decodeXml (const QString& xml, QPointList& pointList);  // De-serialise
   void snapToGrid ();           //

   QPoint toPixelCoords (const QPointF pos);
   QPointF toPercentageCoords (const QPoint pos);

   const bool isDialogMode;

   QColor penColour;
   int penWidth;
   Qt::PenStyle penStyle;
   bool isPolygon;
   QColor brushColour;
   Qt::BrushStyle brushStyle;
   double gridSize;

   QPointList points;

   // These items support the VCLPolylineSetupDialog called up in designer.
   //
   friend class VCLPolylineSetupDialog;

   explicit VCLPolyline (const bool dialog,
                         QWidget* parent);

   int selectedPoint;
   int hoverOverPoint;

signals:
   void updateTable();
};

#endif  // VCL_POLYLINE_H
