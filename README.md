Aether Rendering Engine
---

```mermaid
┌─────────────────────────────────────────┐
│           应用层 (Application)           │
├─────────────────────────────────────────┤
│         渲染框架层 (RenderFramework)     │
├─────────────────────────────────────────┤
│   Aether核心层 (Aether Core)             │
│  ┌─────────────┬─────────────────────┐  │
│  │ 渲染图      │  资源管理             │ │
│  │ (RenderGraph) │ (Resource Manager) │ │
│  └─────────────┴─────────────────────┘  │
├─────────────────────────────────────────┤
│     硬件抽象层 (RHI - Render Hardware)   │
│  ┌─────────────┬─────────────────────┐  │
│  │ Vulkan      │   DirectX 12        │  │
│  └─────────────┴─────────────────────┘  │
└─────────────────────────────────────────┘
```


## List to do：
> 1. Support c++20 modules  
> 2. Support c++20 Coroutine，asynchronous Resource loading
> 3. CommandList based RHI, support D3D12/Vulkan
> 4. GPU Driven Pipeline: 
- a. GPU Culling
- b. Indirect Rendering
- c. GPU Data Management 管理场景数据（将物体位置、材质、LOD级别保存到GPU缓存中）
- d. Dynamic LOD
> 5. Hlsl based shading language, support ray tracing shader & Mesh Shader



## 已实现的功能：（2026.02.20）
> 1. Support c++20 modules  所有模块都使用C++ Module
> 2. 已定义CommandList based RHI, 正在实现D3D12


