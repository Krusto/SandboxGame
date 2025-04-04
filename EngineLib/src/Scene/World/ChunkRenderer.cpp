#include "ChunkRenderer.hpp"
#include "ChunkConstraints.hpp"
#include <Core/Allocator.hpp>

namespace Engine
{
    void ChunkRenderer::Init()
    {
        size_t numIndices = CHUNK_SIZE_CUBIC * 6;
        m_VertexArray.Create(numIndices);
        m_StorageBuffer.Init(nullptr, BUFFER_SIZE, Engine::StorageBufferType::DynamicStorage);
        std::vector<uint32_t> indices(numIndices);

        for (size_t i = 0; i < numIndices; i += 6)
        {
            indices.push_back((i << 2) | 2u);
            indices.push_back((i << 2) | 0u);
            indices.push_back((i << 2) | 1u);
            indices.push_back((i << 2) | 1u);
            indices.push_back((i << 2) | 3u);
            indices.push_back((i << 2) | 2u);
        }
        m_VertexArray.AddIndexBuffer(indices.data(), numIndices);
        m_IndirectBuffer.Create(nullptr, sizeof(DrawElementsIndirectCommand) * MAX_DRAW_COMMANDS,
                                Engine::StorageBufferType::DynamicStorage);
    }

    DrawElementsIndirectCommand* ChunkRenderer::GetDrawCommand(uint32_t quadCount, uint32_t baseInstance)
    {
        uint32_t requestedSize = quadCount * QUAD_SIZE;

        if ((BUFFER_SIZE - m_AllocationEnd) > requestedSize)
        {
            auto slot = Allocator::Allocate<BufferSlot>();
            slot->startByte = m_AllocationEnd;
            slot->sizeBytes = requestedSize;
            m_UsedSlots.push_back(slot);
            m_AllocationEnd += requestedSize;
            return CreateCommand(*slot, baseInstance);
        }

        auto prev = m_UsedSlots.begin();
        BufferFit bestFit;
        bool foundFit = false;
        uint32_t spaceInFront = m_UsedSlots.front()->startByte;
        if (spaceInFront >= requestedSize)
        {
            foundFit = true;
            bestFit = {0, spaceInFront, m_UsedSlots.begin()};
        }
        if (m_UsedSlots.size() == 1)
        {
            auto slot = Allocator::Allocate<BufferSlot>();
            slot->startByte = m_AllocationEnd;
            slot->sizeBytes = requestedSize;
            m_UsedSlots.insert(m_UsedSlots.end(), slot);
            return CreateCommand(*slot, baseInstance);
        }

        for (auto it = m_UsedSlots.begin() + 1; it != m_UsedSlots.end(); it++)
        {
            uint32_t position = (*prev)->startByte + (*prev)->sizeBytes;
            uint32_t spaceBetween = (*it)->startByte - position;
            if (spaceBetween >= requestedSize && (!foundFit || spaceBetween < bestFit.space))
            {
                foundFit = true;
                bestFit = {position, spaceBetween, it};
            }
            prev = it;
        }
        auto slot = Allocator::Allocate<BufferSlot>();
        slot->startByte = m_UsedSlots.back()->startByte + m_UsedSlots.back()->sizeBytes;
        slot->sizeBytes = requestedSize;
        m_UsedSlots.insert(m_UsedSlots.end(), slot);
        return CreateCommand(*slot, baseInstance);
    }

    DrawElementsIndirectCommand* ChunkRenderer::CreateCommand(BufferSlot& slot, uint32_t baseInstance)
    {
        auto command = Allocator::Allocate<DrawElementsIndirectCommand>();
        command->indexCount = (slot.sizeBytes / QUAD_SIZE) * 6;
        command->instanceCount = 1;
        command->firstIndex = 0;
        command->baseQuad = (slot.startByte / QUAD_SIZE) << 2;
        command->baseInstance = baseInstance;
        return command;
    }

    void ChunkRenderer::RemoveDrawCommand(const DrawElementsIndirectCommand* command)
    {
        for (auto it = m_UsedSlots.begin(); it != m_UsedSlots.end(); it++)
        {
            if ((*it)->startByte == (command->baseQuad >> 2) * QUAD_SIZE)
            {
                m_UsedSlots.erase(it);
                break;
            }
        }
    }

    void ChunkRenderer::UploadData(const DrawElementsIndirectCommand& command, void* data)
    {
        m_StorageBuffer.Upload((uint8_t*) data, (command.indexCount / 6) * QUAD_SIZE,
                               (command.baseQuad >> 2) * QUAD_SIZE);
    }

    void ChunkRenderer::AddDrawCommand(const DrawElementsIndirectCommand& command)
    {
        m_DrawCommands.push_back(command);
    }

    void ChunkRenderer::Render()
    {
        size_t numCommands = m_DrawCommands.size();
        if (numCommands == 0) return;

        m_IndirectBuffer.Upload((uint8_t*) m_DrawCommands.data(), sizeof(DrawElementsIndirectCommand) * numCommands, 0);

        m_VertexArray.Bind();
        m_StorageBuffer.Bind(0);
        m_IndirectBuffer.Bind();
        m_IndirectBuffer.Draw(numCommands, 0, 0);

        m_DrawCommands.clear();
    }
}// namespace Engine