#pragma once


namespace ForgeDelta {

	class RenderCommandQueue
	{
	public:
		using RenderCommand = std::function<unsigned int(void*)>;
		typedef unsigned int(*RenderCommandFn)(void*);

		RenderCommandQueue();
		~RenderCommandQueue();

		void Submit(const RenderCommand& command);
		void SubmitCommand(RenderCommandFn fn, void* params, unsigned int size);
		void* Allocate(RenderCommandFn fn, unsigned int size);
		void Execute();
	private:
		unsigned char* m_CommandBuffer;
		unsigned char* m_CommandBufferPtr;
		unsigned int m_RenderCommandCount = 0;
		unsigned int m_CommandCount = 0;
	};



}