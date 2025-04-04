#pragma once
#include <Renderer/Renderer.hpp>

namespace Engine
{
    static const int BUFFER_SIZE = 5e8;// 500MB
    static const uint32_t QUAD_SIZE = 8;
    static const int MAX_DRAW_COMMANDS = 1000000;

    struct BufferSlot {
        uint32_t startByte;
        uint32_t sizeBytes;
    };

    struct DrawElementsIndirectCommand {
        uint32_t indexCount;   // Quad count * 6
        uint32_t instanceCount;// 1
        uint32_t firstIndex;   // 0
        uint32_t baseQuad;     // (baseVertex) starting index
        uint32_t baseInstance; // Chunk position,face index
    };

    struct BufferFit {
        uint32_t pos;
        uint32_t space;
        std::vector<BufferSlot*>::iterator iter;
    };

    class ChunkRenderer
    {
    public:
        void Init();
        DrawElementsIndirectCommand* GetDrawCommand(uint32_t quadCount, uint32_t baseInstance);
        DrawElementsIndirectCommand* CreateCommand(BufferSlot& slot, uint32_t baseInstance);
        void RemoveDrawCommand(const DrawElementsIndirectCommand* command);
        void UploadData(const DrawElementsIndirectCommand& command, void* data);
        void AddDrawCommand(const DrawElementsIndirectCommand& command);
        void Render();

    private:
        VertexArray m_VertexArray;
        StorageBuffer m_StorageBuffer;
        DrawIndirectBuffer m_IndirectBuffer;
        uint32_t m_AllocationEnd{};
        std::vector<BufferSlot*> m_UsedSlots;
        std::vector<DrawElementsIndirectCommand> m_DrawCommands;
    };
}// namespace Engine