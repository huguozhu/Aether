module Aether:RHIFrameBuffer;
import :RHIFrameBuffer;
import :RHIViewport;
import :RHIRenderView;
import :Engine;
import :EngineDefinition;
import :Vector;
import :Log;

namespace Aether
{
    void RHIFrameBuffer::AttachTargetView(Attachment att, RHIRenderTargetViewPtr const& view)
    {
        uint8_t index = to_underlying(att);
        if (index >= MAX_COLOR_ATTACHMENTS)
            return;

        m_vRenderTargets[att] = view;
        m_bDirty = true;
        if (Attachment::Color0 == att)
        {
            this->SetViewport({ 0, 0, view->Width(), view->Height() });
        }
    }
    void RHIFrameBuffer::DetachTargetView(Attachment att)
    {
        if (att < m_vRenderTargets.size() && m_vRenderTargets[att])
        {
            m_vRenderTargets[att].reset();
            m_bDirty = true;
        }
        else
        {
            LOG_ERROR("DetachTargetView() error, invalid attachment %d", att);
        }
        if (Attachment::Color0 == att)
        {
            this->SetViewport({ 0, 0, 0, 0 });
        }
    }
    void RHIFrameBuffer::DetachAllTargetView()
    {
        for (size_t i = 0; i < m_vRenderTargets.size(); i++)
        {
            if (m_vRenderTargets[i])
            {
                m_vRenderTargets[i].reset();
                m_bDirty = true;
            }
        }
        this->SetViewport({ 0, 0, 0, 0 });
    }
    void RHIFrameBuffer::AttachDepthStencilView(RHIDepthStencilViewPtr const& view)
    {
        m_pDepthStencilView = view; 
        m_bDirty = true;
    }
    void RHIFrameBuffer::DetachDepthStencilView() 
    {
        m_pDepthStencilView = nullptr;
        m_bDirty = true;
    }
};
