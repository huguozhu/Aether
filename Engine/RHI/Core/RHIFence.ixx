module;
import <string>;
export module Aether:RHIFence;

export namespace Aether
{
	class AetherEngine;
	class RHIFence
	{
	public:
		RHIFence(AetherEngine* engine)
			:m_pEngine(engine)
		{
		}
		virtual ~RHIFence() {}

		virtual uint64_t Signal() = 0;
		virtual void Wait(uint64_t value) = 0;
		virtual bool IsCompleted(uint64_t value) = 0;

	protected:
		AetherEngine* m_pEngine = nullptr;
	};

};