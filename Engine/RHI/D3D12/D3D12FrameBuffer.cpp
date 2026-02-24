module Aether:D3D12FrameBuffer;
import :D3D12FrameBuffer;
import :Engine;
import :EngineDefinition;
import :D3D12Definition;
import :D3D12Context;
import :D3D12RenderView;
import :D3D12Translate;
import :D3D12CommandList;
import :Error;

namespace Aether
{
    D3D12FrameBuffer::D3D12FrameBuffer(AetherEngine* engine)
        :RHIFrameBuffer(), m_pEngine(engine)
    {
        m_D3dViewport.MinDepth = 0.0f;
        m_D3dViewport.MaxDepth = 1.0;
    }

    void D3D12FrameBuffer::BindBarrier(ID3D12GraphicsCommandList* cmd_list)
    {
        if (m_bDirty)
        {
            this->Update();
            m_bDirty = false;
        }

        for (uint32_t i = 0; i < m_vD3dRtvResources.size(); ++i)
        {
            D3D12Resource* pRes = std::get<0>(m_vD3dRtvResources[i]);
            uint32_t first_subres = std::get<1>(m_vD3dRtvResources[i]);
            uint32_t num_subres = std::get<2>(m_vD3dRtvResources[i]);
            for (uint32_t j = 0; j < num_subres; ++j)
                pRes->UpdateResourceBarrier(cmd_list, first_subres + j, D3D12_RESOURCE_STATE_RENDER_TARGET);
        }
        if (std::get<0>(m_DsvResource))
        {
            D3D12Resource* pRes = std::get<0>(m_DsvResource);
            uint32_t first_subres = std::get<1>(m_DsvResource);
            uint32_t num_subres = std::get<2>(m_DsvResource);
            for (uint32_t j = 0; j < num_subres; ++j)
                pRes->UpdateResourceBarrier(cmd_list, first_subres + j, D3D12_RESOURCE_STATE_DEPTH_WRITE);
        }
    }

    void D3D12FrameBuffer::Update()
    {
        m_vD3dRtvResources.clear();
        m_vD3dRtvCpuHandles.clear();

        m_iNumRtvs = 0;
        for (uint32_t i = 0; i < m_vRenderTargets.size(); ++i)
        {
            D3D12RenderTargetView* pRtv = (D3D12RenderTargetView*)(m_vRenderTargets[i].get());
            if (pRtv)
            {
                m_vD3dRtvResources.push_back(std::make_tuple<D3D12Resource*, uint32_t, uint32_t>(
                    pRtv->GetResource().get(),                 
                    pRtv->GetFirstSubRes(), 
                    pRtv->GetNumSubRes()
                ));
                m_vD3dRtvCpuHandles.push_back(pRtv->GetD3DRtv()->Handle());
                m_iNumRtvs++;
                m_vRtvFormats[i] = D3D12Translate::TranslateToPlatformFormat(pRtv->Format());
            }            
        }

        if (m_pDepthStencilView)
        {
            D3D12DepthStencilView* pDsv = (D3D12DepthStencilView*)(m_pDepthStencilView.get());
            m_DsvResource = std::make_tuple<D3D12Resource*, uint32_t, uint32_t>(
                pDsv->GetResource().get(),
                pDsv->GetFirstSubRes(),
                pDsv->GetNumSubRes()
            );
            m_D3dSdvHandle = pDsv->GetD3DDsv()->Handle();
            m_D3dSdvHandlePtr = &m_D3dSdvHandle;
            m_eDsvFormat = D3D12Translate::TranslateToPlatformFormat(pDsv->Format());
        }

        m_D3dViewport.TopLeftX = m_Viewport.Left();
        m_D3dViewport.TopLeftY = m_Viewport.Top();
        m_D3dViewport.Width = m_Viewport.Width();
        m_D3dViewport.Height = m_Viewport.Height();
        m_iNumSamples = 1;
    }

    AResult D3D12FrameBuffer::Active(D3D12CommandList* cmd_list)
    {
        if (!cmd_list)
            return ERR_INVALID_ARG;

        ID3D12GraphicsCommandList* pD3dCmdList = cmd_list->GetD3dCommandList();
        pD3dCmdList->OMSetRenderTargets(m_iNumRtvs, m_vD3dRtvCpuHandles.data(), false, &m_D3dSdvHandle);

        for (uint32_t i = 0; i < m_vRenderTargets.size(); ++i)
        {
            if ( m_colorLoadOptions[i].loadAction == LoadAction::Clear)
            {
                if (i >= m_vD3dRtvCpuHandles.size())
                    continue;
                pD3dCmdList->ClearRenderTargetView(m_vD3dRtvCpuHandles[i], m_colorLoadOptions[i].clearColor.data(), 0, nullptr);
            }
        }
        return A_Success;
    }
    AResult D3D12FrameBuffer::Active(ID3D12GraphicsCommandList* cmd_list)
    {
        if (!cmd_list)
            return ERR_INVALID_ARG;
        
        this->BindBarrier(cmd_list);
        cmd_list->OMSetRenderTargets(m_iNumRtvs, m_vD3dRtvCpuHandles.data(), false, &m_D3dSdvHandle);

        for (uint32_t i = 0; i < m_vRenderTargets.size(); ++i)
        {
            if (m_colorLoadOptions[i].loadAction == LoadAction::Clear)
            {
                if (i >= m_vD3dRtvCpuHandles.size())
                    continue;
                cmd_list->ClearRenderTargetView(m_vD3dRtvCpuHandles[i], m_colorLoadOptions[i].clearColor.data(), 0, nullptr);
            }
        }
        cmd_list->RSSetViewports(1, &m_D3dViewport);
        return A_Success;
    }
};