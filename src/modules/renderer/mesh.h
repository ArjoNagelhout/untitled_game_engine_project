//
// Created by Arjo Nagelhout on 15/11/2023.
//

#ifndef SHAPEREALITY_MESH_H
#define SHAPEREALITY_MESH_H

#include <graphics/device.h>
#include <graphics/buffer.h>

#include "math/vector.h"

#include <vector>

namespace renderer
{
    enum VertexAttribute_
    {
        VertexAttribute_None = 0,
        VertexAttribute_Position = 1 << 0,
        VertexAttribute_Normal = 1 << 1,
        VertexAttribute_Tangent = 1 << 2,
        VertexAttribute_TextureCoordinate = 1 << 3, // also known as UV
        VertexAttribute_Color = 1 << 4,
        VertexAttribute_Joints = 1 << 5,
        VertexAttribute_Weights = 1 << 6,
        VertexAttribute_All = (1 << 7) - 1
    };

    enum class ElementType
    {
        Scalar = 0,
        Vector2,
        Vector3,
        Vector4,
        Matrix2x2,
        Matrix3x3,
        Matrix4x4,
    };

    [[nodiscard]] size_t componentCount(ElementType elementType);

    /**
     * Data type used for components or the index buffer
     */
    enum class ComponentType
    {
        SignedByte = 0, // 8 bits, 1 byte
        UnsignedByte, // 8 bits, 1 byte
        SignedShort, // 16 bits, 2 bytes
        UnsignedShort, // 16 bits, 2 bytes
        UnsignedInt, // 32 bits, 4 bytes
        Float // 32 bits, 4 bytes
    };

    // get the stride for a given component type (in bytes)
    [[nodiscard]] size_t stride(ComponentType componentType);

    struct VertexAttributeDescriptor
    {
        size_t index = 0;
        VertexAttribute_ type = VertexAttribute_Position;
        ElementType elementType = ElementType::Vector3;
        ComponentType componentType = ComponentType::Float;
    };

    [[nodiscard]] size_t elementSize(VertexAttributeDescriptor const& descriptor);

    struct MeshDescriptor
    {
        // mesh type
        graphics::PrimitiveType primitiveType;

        // vertices
        std::vector<VertexAttributeDescriptor> attributes;
        size_t vertexCount = 0;

        // indices
        bool hasIndexBuffer = false; // if this is set to true, indexData should not be nullptr and indexCount should be set.
        size_t indexCount = 0;
        ComponentType indexType = ComponentType::UnsignedInt; // UnsignedInt = 32 bits, 2^16 only supports 65.536 indices

        bool writable = false; // if this is set to true, we keep a copy of the mesh on the CPU that can be written to.

        [[nodiscard]] bool valid() const;
    };

    struct Mesh;

    // this iterator enables us to iterate over the vertex attributes in the Mesh abstraction
    struct VertexAttributesIterator final
    {
        struct Data
        {
            size_t index = 0;
            size_t offset = 0;
            VertexAttributeDescriptor const* descriptor = nullptr;
        };

        using iterator_category = std::forward_iterator_tag;
        using difference_type = size_t;
        using value_type = Data;
        using const_reference = value_type const&;

        explicit VertexAttributesIterator(Mesh const& mesh, size_t index);

        [[nodiscard]] const_reference operator*() const;

        // prefix increment
        [[nodiscard]] VertexAttributesIterator& operator++();

        // equality
        friend  bool operator==(VertexAttributesIterator const& lhs, VertexAttributesIterator const& rhs);

        friend bool operator!=(VertexAttributesIterator const& lhs, VertexAttributesIterator const& rhs);

    private:
        Mesh const& mesh;
        value_type current;

        void updateCurrent();
    };

    /**
     * We use non-interleaved vertex attributes.
     * All vertex attributes are stored in the same buffer, at defined offsets.
     *
     * todo: element alignment and buffer alignment depending on supported feature set of GPU
     *       should be queried with graphics API.
     *
     * late night notes:
     * alignment needs to be checked... so we need to manually import the gltf buffers
     * and convert them to the optimal layout for the specific operating system.
     *
     * **Add alignment** for elements (e.g. Vector<3, float> takes up 4 * sizeof(float)).
     * and alignment for buffer offset. These requirements are specified in the feature set table.
     *
     * maybe write a method on a Mesh to create a new Mesh with the desired alignment that reads from another Mesh.
     * and then there's just a simple method that queries the desired MeshDescriptor, that returns the right values depending on the specification
     * (Apple GPU feature set table).
     *
     * The structs inside the Metal shaders should then also be changed depending on what is required on the target platform.
     */
    class Mesh
    {
    public:
        // construct mesh without vertex or index data provided yet, can be supplied later using the setXXX() methods
        explicit Mesh(graphics::IDevice* device, MeshDescriptor descriptor);

        // construct mesh from memory that already contains the different attributes sequentially
        explicit Mesh(graphics::IDevice* device, MeshDescriptor descriptor, void* vertexData,
                      void* indexData = nullptr);

        // construct mesh from individual pieces of memory that contain the different attributes separately
        explicit Mesh(graphics::IDevice* device, MeshDescriptor descriptor, std::vector<void*> const& attributesData,
                      void* indexData = nullptr);

        ~Mesh();

        // clears the buffers and resets the descriptor to represent an empty mesh
        // does not reset any of the other MeshDescriptor parameters like its PrimitiveType or whether it's writable
        void clear();

        /**
         * set data of the mesh for a specific vertex attribute
         * no validation is done for whether data is large enough
         *
         * @param attribute which attribute type to use
         * @param data
         * @param index e.g. whether to use UV_0 or UV_1
         */
        void setAttributeData(VertexAttribute_ attribute, void* data, size_t index = 0);

        // set the different vertex attributes from different memory locations individually
        void setAttributesData(std::vector<void*> const& attributesData);

        // set the entire vertex buffer at once, vertexData should not be nullptr
        void setVertexData(void* vertexData);

        // set the index buffer data, indexData should not be nullptr
        void setIndexData(void* indexData);

        [[nodiscard]] MeshDescriptor const& descriptor() const;

        // get vertex buffer
        [[nodiscard]] graphics::Buffer* vertexBuffer();

        // get index buffer
        [[nodiscard]] graphics::Buffer* indexBuffer();

        // vertex attributes iterator begin (for iterating over the vertex attributes)
        [[nodiscard]] VertexAttributesIterator begin() const;

        // vertex attributes iterator end
        [[nodiscard]] VertexAttributesIterator end() const;

    private:
        graphics::IDevice* device;
        MeshDescriptor descriptor_; // description of what is inside the buffers, e.g. vertex count and the primitive type
        std::unique_ptr<graphics::Buffer> vertexBuffer_; // vertex buffer with non-interleaved (sequential) contiguous data for all vertex attributes
        std::unique_ptr<graphics::Buffer> indexBuffer_;
        std::vector<size_t> offsets;

        // reallocates the vertex buffer if its size is not equal to the desired size
        void createVertexBuffer();

        //
        void createIndexBuffer();

        //
        [[nodiscard]] size_t desiredVertexBufferSize();

        //
        [[nodiscard]] graphics::BufferUsage_ bufferUsage() const;

        // returns the index of the attribute descriptor inside descriptor_.attributes()
        // this index enables getting the offset of a given attribute
        // asserts that attribute type exists with provided index
        [[nodiscard]] size_t getAttributeIndex(VertexAttribute_ attribute, size_t index) const;

        // updates the private member `offsets` for the VertexAttributesIterator
        void updateOffsets();

        friend class VertexAttributesIterator;
    };
}

#endif //SHAPEREALITY_MESH_H
