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

#include "seekstack.h"

#include "debug.h"

#include <QtCore/QTimer>
#include <QtCore/QDebug>

namespace Phonon
{
namespace VLC
{

SeekStack::SeekStack(MediaObject *mediaObject)
    : QObject(mediaObject)
{
    p_media_object = mediaObject;

    p_timer = new QTimer(this);
    connect(p_timer, SIGNAL(timeout()),
            this, SLOT(popSeek()));
    p_timer->setInterval(1000);
}

SeekStack::~SeekStack()
{
}

void SeekStack::pushSeek(qint64 milliseconds)
{
    debug() << Q_FUNC_INFO << "seek:" << milliseconds;

    disconnect(p_media_object, SIGNAL(tickInternal(qint64)),
               p_media_object, SLOT(tickInternalSlot(qint64)));

    stack.push(milliseconds);

    if (!p_timer->isActive()) {
        p_timer->start();
        popSeek();
    }
}

void SeekStack::popSeek()
{
    if (stack.isEmpty()) {
        p_timer->stop();
        reconnectTickSignal();
        return;
    }

    int i_milliseconds = stack.pop();
    stack.clear();

    debug() << Q_FUNC_INFO << "real seek:" << i_milliseconds;

    p_media_object->seekInternal(i_milliseconds);

    reconnectTickSignal();
}

void SeekStack::reconnectTickSignal()
{
    connect(p_media_object, SIGNAL(tickInternal(qint64)),
            p_media_object, SLOT(tickInternalSlot(qint64)));
}

}
} // Namespace Phonon::VLC
