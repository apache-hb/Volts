#pragma once

#include "Support.h"

#include <vector>

namespace Volts::RSX
{
    using DX12Support::Ptr;

    struct DX12 : Render
    {
        DX12();
        virtual ~DX12() override {}
        virtual void Attach(GUI::Frame* Handle) override;
        virtual void Detach() override;
        virtual const char* Name() const override { return "DX12"; }
        virtual const char* Description() const override { return "DirectX12"; }

        virtual Device* Devices(U32* Count) override;
        virtual void SetDevice(RSX::Device* Device) override;

        virtual void Options() override;

        virtual void Resize(GUI::Size NewSize) override;

        virtual void UpdateVSync(bool NewMode) override { VSyncMode = NewMode ? 1 : 0; }

        virtual void BeginRender() override;
        virtual void PresentRender() override;

        virtual void Windowed() override;
        virtual void Borderless() override;
        virtual void Fullscreen() override;

    private:

        struct Vertex
        {
            F32 Pos[3];
            F32 Colour[4];
        };

        void LoadPipeline();
        void LoadData();
        void ReleaseObjects();
        void PopulateCommandList();
        void WaitForGPU();
        void AdvanceFrame();

        static constexpr U32 FrameCount = 2;

        void CreateChildWindow();
        void CleanupChildWindow();
        HWND Child;

        // creation objects
        Ptr<IDXGIFactory4> Factory;
        std::vector<DX12Support::DX12Device> DeviceList;
        U32 CurrentDeviceIndex = 0;

        // Pipeline state objects
        D3D12_VIEWPORT Viewport;
        D3D12_RECT Scissor;

        Ptr<IDXGISwapChain3> Swap;
        Ptr<ID3D12Device> Device;
        Ptr<ID3D12CommandQueue> CommandQueue;

        Ptr<ID3D12Resource> RenderTargets[FrameCount];
        Ptr<ID3D12CommandAllocator> CommandAllocators[FrameCount];

        Ptr<ID3D12RootSignature> RootSignature;
        Ptr<ID3D12PipelineState> PipelineState;
        Ptr<ID3D12GraphicsCommandList> CommandList;
        Ptr<ID3D12DescriptorHeap> RTVHeap;
        U32 RTVDescriptorSize;

        Ptr<ID3D12DescriptorHeap> SRVHeap;

        Ptr<ID3D12Resource> VertexBuffer;
        D3D12_VERTEX_BUFFER_VIEW VertexBufferView;

#if VDXDEBUG
        Ptr<ID3D12InfoQueue> DebugQueue;
#endif

        // Synchronization data
        U32 FrameIndex;
        HANDLE FenceEvent;
        Ptr<ID3D12Fence> Fence;
        U64 FenceValues[FrameCount];

        // extra data
        bool Tear;

        U32 VSyncMode = 0;
        UINT ShaderOptimizationLevel = 0;

        // External data
        GUI::Frame* Frame;
    };
}