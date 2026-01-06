export module SampleCommon:AppFramework;

import Aether;
import <string>;

constexpr uint32_t DEFAULT_WND_WIDTH = 1280;
constexpr uint32_t DEFAULT_WND_HEIGHT = 720;

using namespace Aether;

export namespace SampleCommon
{
    class AppFramework
    {
    public:
        AppFramework(std::string const& name);
        virtual ~AppFramework() = default;

        AResult Run();

        virtual AResult         OnCreate() = 0;
        virtual AResult         OnUpdate() = 0;
        virtual AResult         OnDestroy() { return A_Success; }
        virtual AResult         RenderFrame();

        virtual AResult         InitEngine(void* device = nullptr, void* native_wnd = nullptr);

        virtual void            IMGUI_Begin();
        virtual void            IMGUI_Rendering();

        static std::string      FullPath(std::string relativePath);
        static std::string      Basename(std::string filepath) { return filepath.substr(filepath.rfind("/") + 1); }


    protected:
        std::string     m_szName;
        AetherEnginePtr m_pEngine = nullptr;
        bool            m_bInit = false;

    };

    int APP_RUN(AppFramework* app)
    {
        AResult ret = app->Run();
        return ret;
    }
};