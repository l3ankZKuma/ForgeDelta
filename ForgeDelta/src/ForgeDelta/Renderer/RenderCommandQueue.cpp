#include "fdpch.h"
#include"RenderCommandQueue.h"
#include"ForgeDelta/Core/Log.h"

#define FD_RENDER_TRACE(...) FD_CORE_TRACE(__VA_ARGS__)

namespace ForgeDelta {

	RenderCommandQueue::RenderCommandQueue()
	{
		m_CommandBuffer = new unsigned char[10 * 1024 * 1024]; // 10mb buffer
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
	}

	RenderCommandQueue::~RenderCommandQueue()
	{
		delete[] m_CommandBuffer;
	}

	void RenderCommandQueue::Submit(const RenderCommand& command)
	{
		auto ptr = m_CommandBuffer;

		memcpy(m_CommandBuffer, &command, sizeof(RenderCommand));
		m_CommandBufferPtr += sizeof(RenderCommand);
		m_RenderCommandCount++;
	}
	
	void *RenderCommandQueue::Allocate(RenderCommandFn fn, unsigned int size)
  {
    unsigned int totalSize = sizeof(RenderCommandFn) + size + (16 - ((sizeof(RenderCommandFn) + size) % 16)); // Include padding

    if (m_CommandBufferPtr + totalSize > m_CommandBuffer + (10 * 1024 * 1024)) // Check if we're about to exceed buffer
    {
      // Handle buffer overflow - perhaps by executing current commands and resetting, or by expanding the buffer
      FD_CORE_ERROR("Render command buffer overflow!");
      return nullptr;
    }

    byte* buffer = m_CommandBufferPtr;
    memcpy(buffer, &fn, sizeof(RenderCommandFn));
    buffer += sizeof(RenderCommandFn);

    m_CommandBufferPtr += totalSize;
    m_RenderCommandCount++;

    return buffer;
  }

	void RenderCommandQueue::SubmitCommand(RenderCommandFn fn, void* params, unsigned int size)
	{
		unsigned int totalSize = sizeof(RenderCommandFn) + size + (16 - ((sizeof(RenderCommandFn) + size) % 16)); // Include padding

		if (m_CommandBufferPtr + totalSize > m_CommandBuffer + (10 * 1024 * 1024)) // Check if we're about to exceed buffer
		{
			// Handle buffer overflow - perhaps by executing current commands and resetting, or by expanding the buffer
			FD_CORE_ERROR("Render command buffer overflow!");
			return;
		}

		byte*& buffer = m_CommandBufferPtr;
		memcpy(buffer, &fn, sizeof(RenderCommandFn));
		buffer += sizeof(RenderCommandFn);
		memcpy(buffer, params, size);
		buffer += size;

		auto padding = totalSize - (sizeof(RenderCommandFn) + size);
		buffer += padding;

		m_RenderCommandCount++;
	}

	void RenderCommandQueue::Execute()
	{
		byte* buffer = m_CommandBuffer;

		for (unsigned int i = 0; i < m_RenderCommandCount; i++)
		{
			RenderCommandFn fn = *(RenderCommandFn*)buffer;
			buffer += sizeof(RenderCommandFn);
			buffer += (*fn)(buffer);

			// Ensure proper alignment
			uintptr_t address = reinterpret_cast<uintptr_t>(buffer);
			buffer = reinterpret_cast<byte*>((address + 15) & ~15);
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_RenderCommandCount = 0;
	}

}