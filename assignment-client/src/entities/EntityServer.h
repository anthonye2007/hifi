//
//  EntityServer.h
//  assignment-client/src/entities
//
//  Created by Brad Hefta-Gaub on 4/29/14
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_EntityServer_h
#define hifi_EntityServer_h

#include "../octree/OctreeServer.h"

#include "EntityItem.h"
#include "EntityServerConsts.h"
#include "EntityTree.h"

/// Handles assignments of type EntityServer - sending entities to various clients.
class EntityServer : public OctreeServer, public NewlyCreatedEntityHook {
    Q_OBJECT
public:
    EntityServer(const QByteArray& packet);
    ~EntityServer();

    // Subclasses must implement these methods
    virtual OctreeQueryNode* createOctreeQueryNode();
    virtual Octree* createTree();
    virtual char getMyNodeType() const { return NodeType::EntityServer; }
    virtual PacketType getMyQueryMessageType() const { return PacketTypeEntityQuery; }
    virtual const char* getMyServerName() const { return MODEL_SERVER_NAME; }
    virtual const char* getMyLoggingServerTargetName() const { return MODEL_SERVER_LOGGING_TARGET_NAME; }
    virtual const char* getMyDefaultPersistFilename() const { return LOCAL_MODELS_PERSIST_FILE; }
    virtual PacketType getMyEditNackType() const { return PacketTypeEntityEditNack; }

    // subclass may implement these method
    virtual void beforeRun();
    virtual bool hasSpecialPacketToSend(const SharedNodePointer& node);
    virtual int sendSpecialPacket(const SharedNodePointer& node, OctreeQueryNode* queryNode, int& packetsSent);

    virtual void entityCreated(const EntityItem& newEntity, const SharedNodePointer& senderNode);

public slots:
    void pruneDeletedEntities();

private:
};

#endif // hifi_EntityServer_h
