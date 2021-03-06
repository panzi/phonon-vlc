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

#ifndef Phonon_VLC_BACKEND_H
#define Phonon_VLC_BACKEND_H

#include "audiooutput.h"
#include "debug.h"

#include <phonon/objectdescription.h>
#include <phonon/backendinterface.h>

#ifndef PHONON_VLC_NO_EXPERIMENTAL
#include "phonon/experimental/backendinterface.h"
#endif // PHONON_VLC_NO_EXPERIMENTAL

#include <QtCore/QList>
#include <QtCore/QPointer>
#include <QtCore/QStringList>

class LibVLC;

namespace Phonon
{
namespace VLC
{
class AudioOutput;
class DeviceManager;
class EffectManager;

/** \brief Backend class for Phonon-VLC.
 *
 * This class provides the special objects created by the backend and information about
 * various things that the backend supports. An object of this class is the root for
 * the backend plugin.
 *
 * Phonon will request the backend to create objects of various classes, like MediaObject,
 * AudioOutput, VideoWidget, Effect. There are also methods to handle the connections between
 * these objects.
 *
 * This class also provides information about the devices and effects that the backend supports.
 * These are audio output devices, audio capture devices, video capture devices, effects.
 */

class Backend : public QObject, public BackendInterface
{
    Q_OBJECT
    Q_INTERFACES(Phonon::BackendInterface)

public:
    /**
     * Instance. Since there is no backend instance without actual Backend object
     * this class behaves likes a singleton.
     */
    static Backend *self;

    /**
     * Constructs the backend. Sets the backend properties, fetches the debug level from the
     * environment, initializes libVLC, constructs the device and effect managers, initializes
     * PulseAudio support.
     *
     * \param parent A parent object for the backend (passed to the QObject constructor)
     */
    explicit Backend(QObject *parent = 0, const QVariantList & = QVariantList());
    virtual ~Backend();

    /**
     * \return The device manager that is associated with this backend object
     */
    DeviceManager *deviceManager() const;

    /**
     * \return The effect manager that is associated with this backend object.
     */
    EffectManager *effectManager() const;

    /**
     * Creates a backend object of the desired class and with the desired parent. Extra arguments can be provided.
     *
     * \param c The class of object that is to be created
     * \param parent The object that will be the parent of the new object
     * \param args Optional arguments for the object creation
     * \return The desired object or NULL if the class is not implemented.
     */
    QObject *createObject(BackendInterface::Class, QObject *parent, const QList<QVariant> &args);

    bool supportsVideo() const;
    bool supportsOSD() const;
    bool supportsSubtitles() const;
    QStringList availableMimeTypes() const;

    /**
     * Returns a list of indexes for the desired object types. It specifies a list of objects
     * of a particular category that the backend knows about. These indexes can be used with
     * objectDescriptionProperties() to get the properties of a particular object.
     *
     * \param type The type of objects for the list
     */
    QList<int> objectDescriptionIndexes(ObjectDescriptionType type) const;

    /**
     * Returns a list of properties for a particular object of the desired category.
     *
     * \param type The type of object for the index
     * \param index The index for the object of the desired type
     * \return The property list. If the object is inexistent, an empty list is returned.
     */
    QHash<QByteArray, QVariant> objectDescriptionProperties(ObjectDescriptionType type, int index) const;

    /**
     * Called when a connection between nodes is about to be changed
     *
     * \param objects A set of objects that will be involved in the change
     */
    bool startConnectionChange(QSet<QObject *>);

    /**
     * Connects two media nodes. The sink is informed that it should connect itself to the source.
     *
     * \param source The source media node for the connection
     * \param sink The sink media node for the connection
     * \return True if the connection was successful
     */
    bool connectNodes(QObject *, QObject *);

    /**
     * Disconnects two previously connected media nodes. It disconnects the sink node from the source node.
     *
     * \param source The source node for the disconnection
     * \param sink The sink node for the disconnection
     * \return True if the disconnection was successful
     */
    bool disconnectNodes(QObject *, QObject *);

    /**
     * Called after a connection between nodes has been changed
     *
     * \param objects Nodes involved in the disconnection
     */
    bool endConnectionChange(QSet<QObject *>);

    /**
     * Return the current debug level.
     *
     * The debug level can be set by the PHONON_VLC_DEBUG environment variable.
     * \li 0 - Only fatal errors are shown
     * \li 1 - Fatal and non-fatal errors
     * \li 2 - All errors and warnings
     * \li 3 - Everything
     *
     * \return The debug level.
     */
    Debug::DebugLevel debugLevel() const;

Q_SIGNALS:
    void objectDescriptionChanged(ObjectDescriptionType);

private:
    mutable QStringList m_supportedMimeTypes;
    QList<QPointer<AudioOutput> > m_audioOutputs;

    DeviceManager *m_deviceManager;
    EffectManager *m_effectManager;
};

}
} // namespace Phonon::VLC

#endif // Phonon_VLC_BACKEND_H
