//
//  MetavoxelMessages.h
//  libraries/metavoxels/src
//
//  Created by Andrzej Kapolka on 12/31/13.
//  Copyright 2013 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_MetavoxelMessages_h
#define hifi_MetavoxelMessages_h

#include "MetavoxelData.h"

/// Requests to close the session.
class CloseSessionMessage {
    STREAMABLE
};

DECLARE_STREAMABLE_METATYPE(CloseSessionMessage)

/// Clears the mapping for a shared object.
class ClearSharedObjectMessage {
    STREAMABLE

public:
    
    STREAM int id;
};

DECLARE_STREAMABLE_METATYPE(ClearSharedObjectMessage)

/// Clears the mapping for a shared object on the main channel (as opposed to the one on which the message was sent).
class ClearMainChannelSharedObjectMessage {
    STREAMABLE

public:
    
    STREAM int id; 
};

DECLARE_STREAMABLE_METATYPE(ClearMainChannelSharedObjectMessage)

/// A message containing the state of a client.
class ClientStateMessage {
    STREAMABLE
    
public:
    
    STREAM MetavoxelLOD lod;
};

DECLARE_STREAMABLE_METATYPE(ClientStateMessage)

/// A message preceding metavoxel delta information.  The actual delta will follow it in the stream.
class MetavoxelDeltaMessage {
    STREAMABLE
};

DECLARE_STREAMABLE_METATYPE(MetavoxelDeltaMessage)

/// A message indicating that metavoxel delta information is being sent on a reliable channel.
class MetavoxelDeltaPendingMessage {
    STREAMABLE
    
public:
    
    STREAM int id;
};

DECLARE_STREAMABLE_METATYPE(MetavoxelDeltaPendingMessage)

/// A simple streamable edit.
class MetavoxelEditMessage {
    STREAMABLE

public:
    
    STREAM QVariant edit;
    
    void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(MetavoxelEditMessage)

/// Abstract base class for edits.
class MetavoxelEdit {
public:

    virtual ~MetavoxelEdit();
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const = 0;
};

/// An edit that sets the region within a box to a value.
class BoxSetEdit : public MetavoxelEdit {
    STREAMABLE

public:

    STREAM Box region;
    STREAM float granularity;
    STREAM OwnedAttributeValue value;
    
    BoxSetEdit(const Box& region = Box(), float granularity = 0.0f,
        const OwnedAttributeValue& value = OwnedAttributeValue());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(BoxSetEdit)

/// An edit that sets the entire tree to a value.
class GlobalSetEdit : public MetavoxelEdit {
    STREAMABLE

public:
    
    STREAM OwnedAttributeValue value;
    
    GlobalSetEdit(const OwnedAttributeValue& value = OwnedAttributeValue());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(GlobalSetEdit)

/// An edit that inserts a spanner into the tree.
class InsertSpannerEdit : public MetavoxelEdit {
    STREAMABLE    

public:
    
    STREAM AttributePointer attribute;
    STREAM SharedObjectPointer spanner;
    
    InsertSpannerEdit(const AttributePointer& attribute = AttributePointer(),
        const SharedObjectPointer& spanner = SharedObjectPointer());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(InsertSpannerEdit)

/// An edit that removes a spanner from the tree.
class RemoveSpannerEdit : public MetavoxelEdit {
    STREAMABLE

public:
    
    STREAM AttributePointer attribute;
    STREAM int id;
    
    RemoveSpannerEdit(const AttributePointer& attribute = AttributePointer(), int id = 0);
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(RemoveSpannerEdit)

/// An edit that clears all spanners from the tree.
class ClearSpannersEdit : public MetavoxelEdit {
    STREAMABLE

public:
    
    STREAM AttributePointer attribute;
    
    ClearSpannersEdit(const AttributePointer& attribute = AttributePointer());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(ClearSpannersEdit)

/// An edit that sets a spanner's attributes in the voxel tree.
class SetSpannerEdit : public MetavoxelEdit {
    STREAMABLE

public:
    
    STREAM SharedObjectPointer spanner;
    
    SetSpannerEdit(const SharedObjectPointer& spanner = SharedObjectPointer());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(SetSpannerEdit)

/// An edit that directly sets part of the metavoxel data.
class SetDataEdit : public MetavoxelEdit {
    STREAMABLE
    
public:
    
    STREAM glm::vec3 minimum;
    STREAM MetavoxelData data;
    STREAM bool blend;
    
    SetDataEdit(const glm::vec3& minimum = glm::vec3(), const MetavoxelData& data = MetavoxelData(), bool blend = false);
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(SetDataEdit)

/// An edit that sets a region of a heightfield height.
class PaintHeightfieldHeightEdit : public MetavoxelEdit {
    STREAMABLE

public:
    
    STREAM glm::vec3 position;
    STREAM float radius;
    STREAM float height;
    
    PaintHeightfieldHeightEdit(const glm::vec3& position = glm::vec3(), float radius = 0.0f, float height = 0.0f);
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(PaintHeightfieldHeightEdit)

/// An edit that sets a region of a heightfield color.
class PaintHeightfieldColorEdit : public MetavoxelEdit {
    STREAMABLE

public:
    
    STREAM glm::vec3 position;
    STREAM float radius;
    STREAM QColor color;
    
    PaintHeightfieldColorEdit(const glm::vec3& position = glm::vec3(), float radius = 0.0f, const QColor& color = QColor());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(PaintHeightfieldColorEdit)

/// An edit that sets a region of a heightfield material.
class PaintHeightfieldMaterialEdit : public MetavoxelEdit {
    STREAMABLE

public:
    
    STREAM glm::vec3 position;
    STREAM float radius;
    STREAM SharedObjectPointer material;
    STREAM QColor averageColor;
    
    PaintHeightfieldMaterialEdit(const glm::vec3& position = glm::vec3(), float radius = 0.0f,
        const SharedObjectPointer& material = SharedObjectPointer(), const QColor& averageColor = QColor());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(PaintHeightfieldMaterialEdit)

/// An edit that sets the color of voxels within a box to a value.
class VoxelColorBoxEdit : public MetavoxelEdit {
    STREAMABLE

public:

    STREAM Box region;
    STREAM float granularity;
    STREAM QColor color;
    
    VoxelColorBoxEdit(const Box& region = Box(), float granularity = 0.0f, const QColor& color = QColor());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(VoxelColorBoxEdit)

/// An edit that sets the materials of voxels within a box to a value.
class VoxelMaterialBoxEdit : public MetavoxelEdit {
    STREAMABLE

public:

    STREAM Box region;
    STREAM float granularity;
    STREAM SharedObjectPointer material;
    STREAM QColor averageColor;
    
    VoxelMaterialBoxEdit(const Box& region = Box(), float granularity = 0.0f,
        const SharedObjectPointer& material = SharedObjectPointer(), const QColor& averageColor = QColor());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(VoxelMaterialBoxEdit)

/// An edit that sets the color of voxels within a sphere to a value.
class VoxelColorSphereEdit : public MetavoxelEdit {
    STREAMABLE

public:

    STREAM glm::vec3 center;
    STREAM float radius;
    STREAM float granularity;
    STREAM QColor color;
    
    VoxelColorSphereEdit(const glm::vec3& center = glm::vec3(), float radius = 0.0f,
        float granularity = 0.0f, const QColor& color = QColor());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(VoxelColorSphereEdit)

/// An edit that sets the materials of voxels within a sphere to a value.
class VoxelMaterialSphereEdit : public MetavoxelEdit {
    STREAMABLE

public:

    STREAM glm::vec3 center;
    STREAM float radius;
    STREAM float granularity;
    STREAM SharedObjectPointer material;
    STREAM QColor averageColor;
    
    VoxelMaterialSphereEdit(const glm::vec3& center = glm::vec3(), float radius = 0.0f, float granularity = 0.0f,
        const SharedObjectPointer& material = SharedObjectPointer(), const QColor& averageColor = QColor());
    
    virtual void apply(MetavoxelData& data, const WeakSharedObjectHash& objects) const;
};

DECLARE_STREAMABLE_METATYPE(VoxelMaterialSphereEdit)

#endif // hifi_MetavoxelMessages_h
