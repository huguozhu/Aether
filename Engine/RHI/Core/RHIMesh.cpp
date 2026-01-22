module Aether:RHIMesh;
import :RHIMesh;
import :RHIResource;
import :RHIStruct;
import :ResourceManager;
import :Engine;
import :EngineDefinition;
import :RHIContext;

namespace Aether
{
    bool RHIMesh::IsUseIndices() const
    {
        return this->GetNumIndices() != 0;
    }

    uint32_t RHIMesh::GetNumIndices() const
    {
        uint32_t n = 0;
        if (m_pIndexBuffer)
        {
            uint32_t m = m_eIndexBufferType == EIndexBufferType::UInt16 ? 2 : 4;
            n = m_pIndexBuffer->GetSize() / m;
        }
        else if (m_indicesRes)
        {
            n = m_indicesRes->_indexCount;
        }
        return n;
    }

    RHIBufferPtr const& RHIMesh::GetIndexBuffer()
    {
        if (!m_pIndexBuffer)
        {
            RHIContext& rc = m_pEngine->RHIContextInstance();
            m_pIndexBuffer = rc.CreateIndexBuffer((uint32_t)m_indicesRes->_size, m_indicesRes->_data);
        }
        return m_pIndexBuffer;
    }

    void RHIMesh::SetIndexBuffer(RHIBufferPtr buffer, EIndexBufferType type)
    {
        m_pIndexBuffer = buffer;
        m_eIndexBufferType = type;
        m_bDataDirty = true;
    }

    void RHIMesh::SetIndexBufferResource(std::shared_ptr<VertexIndicesResource>& indicesRes)
    {
        m_indicesRes = indicesRes;
        m_eIndexBufferType = indicesRes->_indexBufferType;
        m_pIndexBuffer.reset();
    }

    uint32_t RHIMesh::GetNumVertex() const
    {
        uint32_t n = 0;
        if (!m_vVertexStreams.empty())
        {
            n = m_vVertexStreams[0].render_buffer->GetSize() / m_vVertexStreams[0].stride;
        }
        return n;
    }

    uint32_t RHIMesh::NumVertexStream()
    {
        return (uint32_t)GetVertexStreams().size();
    }

    void RHIMesh::AddVertexStream(RHIBufferPtr render_buffer, uint32_t buffer_offset,
        uint32_t stride, VertexFormat format, EVertexElementUsage usage, uint32_t usage_index)
    {
        VertexStream* stream = nullptr;

        for (size_t i = 0; i < m_vVertexStreams.size(); i++)
        {
            if (render_buffer == m_vVertexStreams[i].render_buffer)
            {
                stream = &m_vVertexStreams[i];
                break;
            }
        }

        if (!stream)
        {
            m_vVertexStreams.push_back(VertexStream());
            stream = &m_vVertexStreams[m_vVertexStreams.size() - 1];
            stream->render_buffer = render_buffer;
            stream->stride = stride;
        }

        VertexStreamLayout layout;
        layout.buffer_offset = buffer_offset;
        layout.format = format;
        layout.usage = usage;
        layout.usage_index = usage_index;
        stream->layouts.push_back(layout);

        m_bDataDirty = true;
    }
    void RHIMesh::AddVertexStream(VertexStream& vs)
    {
        m_vVertexStreams.push_back(vs);
    }

    std::vector<VertexStream>& RHIMesh::GetVertexStreams()
    {
        if (m_vVertexStreams.empty())
        {
            RHIContext& rc = m_pEngine->RHIContextInstance();
            for (size_t i = 0; i != m_vertexAttributeRes._vertexStreams.size(); i++)
            {
                VertexStream& _vs = m_vertexAttributeRes._vertexStreams[i];
                std::shared_ptr<BufferResource>& _vsBuf = m_vertexAttributeRes._vertexBuffers[i];

                m_vVertexStreams.emplace_back(_vs);
                VertexStream& vs = m_vVertexStreams.back();
                vs.render_buffer = rc.CreateVertexBuffer((uint32_t)_vsBuf->_size, _vsBuf->_data);
            }
        }

        return m_vVertexStreams;
    }

    VertexStream& RHIMesh::GetVertexStreamByIndex(uint32_t i)
    {
        return GetVertexStreams()[i];
    }

    void RHIMesh::SetVertexAttributeResource(VertexAttributeResource& res)
    {
        m_vertexAttributeRes = res;
    }

    VertexAttributeResource& RHIMesh::GetVertexAttributeResource()
    {
        return m_vertexAttributeRes;
    };

    VertexIndicesResource& RHIMesh::GetVertexIndicesResource()
    {
        return *m_indicesRes;
    };
    MaterialPtr& RHIMesh::GetMaterial()
    {
        if (!m_pMaterial)
            m_pMaterial = MakeSharedPtr<Material>();
        return m_pMaterial;
    }

};
