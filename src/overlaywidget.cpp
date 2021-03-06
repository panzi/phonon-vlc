/*****************************************************************************
 * libVLC backend for the Phonon library                                     *
 *                                                                           *
 * Copyright (C) 2007-2008 Tanguy Krotoff <tkrotoff@gmail.com>               *
 * Copyright (C) 2008 Lukas Durfina <lukas.durfina@gmail.com>                *
 * Copyright (C) 2009 Fathi Boudra <fabo@kde.org>                            *
 * Copyright (C) 2009-2010 vlc-phonon AUTHORS                                *
 *                                                                           *
 * This program is free software; you can redistribute it and/or             *
 * modify it under the terms of the GNU Lesser General Public                *
 * License as published by the Free Software Foundation; either              *
 * version 2.1 of the License, or (at your option) any later version.        *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU         *
 * Lesser General Public License for more details.                           *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public          *
 * License along with this package; if not, write to the Free Software       *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA *
 *****************************************************************************/

#include "overlaywidget.h"

#include <QtGui/QPainter>

namespace Phonon
{
namespace VLC
{
OverlayWidget::OverlayWidget(QWidget *parent) :
    BaseWidget(parent)
{
    // When resizing fill with black (backgroundRole color) the rest is done by paintEvent
    setAttribute(Qt::WA_OpaquePaintEvent);

    // Disable Qt composition management as MPlayer draws onto the widget directly
    setAttribute(Qt::WA_PaintOnScreen);

    // Indicates that the widget has no background,
    // i.e. when the widget receives paint events, the background is not automatically repainted.
    setAttribute(Qt::WA_NoSystemBackground);

    // Required for dvdnav
#ifdef __GNUC__
#warning dragonplayer munches on our mouse events, so clicking in a DVD menu does not work - vlc 1.2 where are thu?
#endif // __GNUC__
    setMouseTracking(true);
}

void OverlayWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // FIXME this makes the video flicker
    // Make everything backgroundRole color
    QPainter painter(this);
    painter.eraseRect(rect());
}

void OverlayWidget::setBackgroundColor(const QColor &color)
{
    QPalette p = palette();
    p.setColor(backgroundRole(), color);
    setPalette(p);
}

}
} // Namespace Phonon::VLC
