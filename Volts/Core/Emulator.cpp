#include "Emulator.h"

#include <iostream>
#include <filesystem>
#include <chrono>

#include "imgui.h"
#include "imgui/examples/imgui_impl_glfw.h"
#include "imgui/examples/imgui_impl_win32.h"
#include "imfilebrowser.h"

#include "Utils/SFO.h"
#include "Utils/UNSELF.h"

namespace Volts
{
    using namespace Utils;
    bool LogScrollToBottom = false;

    void Trace(const char* M)
    {
        auto* Emu = Emulator::Get();
        if(Emu->Level > LogLevel::Trace)
            return;

        Emu->LogBuffer.append(fmt::format("[trace] {}\n", M).c_str());
        LogScrollToBottom = true;
    }

    void Info(const char* M)
    {
        auto* Emu = Emulator::Get();
        if(Emu->Level > LogLevel::Info)
            return;

        Emu->LogBuffer.append(fmt::format("[info] {}\n", M).c_str());
        LogScrollToBottom = true;
    }

    void Warn(const char* M)
    {
        auto* Emu = Emulator::Get();
        if(Emu->Level > LogLevel::Warn)
            return;

        Emu->LogBuffer.append(fmt::format("[info] {}\n", M).c_str());
        LogScrollToBottom = true;
    }

    void Error(const char* M)
    {
        auto* Emu = Emulator::Get();
        if(Emu->Level > LogLevel::Error)
            return;

        Emu->LogBuffer.append(fmt::format("[error] {}\n", M).c_str());
        LogScrollToBottom = true;
    }

    void Fatal(const char* M)
    {
        Emulator::Get()->LogBuffer.append(fmt::format("[fatal] {}\n", M).c_str());
        LogScrollToBottom = true;
    }

    using namespace std;

    Emulator* Emulator::Get()
    {
        static Emulator* Singleton = new Emulator();
        return Singleton;
    }

    enum class DialogType : U8
    {
        None,
        PARAM,
        UNSELF,
    };

    using TimePoint = decltype(std::chrono::high_resolution_clock::now());
    using TimeDiff = std::chrono::duration<double, std::milli>;

    DialogType CurrentDialog;
    ImGui::FileBrowser FileDialog;
    TimePoint LastFrame = std::chrono::high_resolution_clock::now();

    void Emulator::GUI()
    {
        if(!ImGui::Begin("Volts"))
        {
            ImGui::End();
        }
        else
        {
            if(ImGui::CollapsingHeader("Metrics"))
            {
                auto Now = std::chrono::high_resolution_clock::now();

                TimeDiff Count = (Now - LastFrame);

                ImGui::Text("%s", fmt::format("FrameTime: {:.2f}ms", Count.count()).c_str());
                ImGui::Text("%s", fmt::format("FPS: {:.2f}", 1000 / Count.count()).c_str());

                LastFrame = Now;
            }

            if(ImGui::CollapsingHeader("Settings"))
            {
                ImGui::Combo("Renders", &Render.Index, Render.Names, Render.Count);

                static bool VSyncEnabled = Cfg.GetVSync();
                if(ImGui::Checkbox("VSync", &VSyncEnabled))
                {
                    Render.Current()->UpdateVSync(VSyncEnabled);
                    Cfg.UpdateVSync(VSyncEnabled);
                }

                if(ImGui::Combo("Device", &CurrentDevice, DeviceNames, DeviceCount))
                {
                    Render.Current()->SetDevice(CurrentDevice);
                }

                static int CurrentAspectRatio = Cfg.GetAspectRatio();
                static const char* AspectRatioOptions[] = { "4:3", "16:9" };
                if(ImGui::Combo("Aspect ratio", &CurrentAspectRatio, AspectRatioOptions, 2))
                {
                    Cfg.SetAspectRatio(AspectRatioOptions[CurrentAspectRatio]);
                }

                Render.Current()->GUI();

                ImGui::Separator();

                ImGui::Combo("Audio", &Audio.Index, Audio.Names, Audio.Count);
                ImGui::Combo("Input", &Input.Index, Input.Names, Input.Count);

                ImGui::Separator();

                static float Scale = Cfg.GetGuiScale();
                if(ImGui::SliderFloat("GUI scale", &Scale, 0.1f, 5.0f, "scale %.3f"))
                {
                    ImGui::GetStyle().ScaleAllSizes(Scale);
                    ImFontConfig FontCfg;
                    FontCfg.SizePixels = 13.f * Scale;
                    ImGui::GetIO().Fonts->AddFontDefault(&FontCfg)->DisplayOffset.y = Scale;

                    Cfg.UpdateGuiScale(Scale);
                }
            }

            if(ImGui::CollapsingHeader("Logs"))
            {
                const char* LevelOptions[] = { "Trace", "Info", "Warn", "Error", "Fatal" };
                if(ImGui::Combo("Filter", (I32*)&Level, LevelOptions, IM_ARRAYSIZE(LevelOptions)))
                {
                    std::string Opt = LevelOptions[(I32)Level];
                    std::for_each(Opt.begin(), Opt.end(), [](char& C){
                        C = std::tolower(C);
                    });

                    Cfg.Data["log_level"] = Opt;
                    Cfg.Save();
                }

                ImGui::SameLine();
                if(ImGui::Button("Clear"))
                    LogBuffer.clear();

                ImGui::Separator();
                ImGui::BeginChild("LogBox");

                ImGui::PushTextWrapPos();
                ImGui::TextUnformatted(LogBuffer.c_str());
                ImGui::PopTextWrapPos();

                if(LogScrollToBottom)
                    ImGui::SetScrollHere(1.f);
                LogScrollToBottom = false;

                ImGui::EndChild();
            }

            if(ImGui::CollapsingHeader("Files"))
            {
                if(ImGui::Button("Parse PARAM.SFO"))
                {
                    CurrentDialog = DialogType::PARAM;
                    FileDialog.SetTitle("SFO");
                    FileDialog.Open();
                }

                if(ImGui::Button("Decrypt EBOOT"))
                {
                    CurrentDialog = DialogType::UNSELF;
                    FileDialog.SetTitle("UNSELF");
                    FileDialog.Open();
                }

                FileDialog.Display();

                if(FileDialog.HasSelected())
                {
                    // TODO: all this is debug code
                    Info(fmt::format("Selected {}", FileDialog.GetSelected().string()).c_str());

                    if(CurrentDialog == DialogType::PARAM)
                    {
                        auto Obj = LoadSFO({FileDialog.GetSelected().string().c_str()});

                        for(auto& [Key, Val] : Obj)
                        {
                            std::string ValStr;
                            switch(Val.Type)
                            {
                                case Format::String:
                                    ValStr = (char*)Val.Data.data();
                                    break;
                                case Format::Array:
                                    for(auto E : Val.Data)
                                        ValStr += std::to_string(E) + " ";
                                    break;
                                case Format::Integer:
                                    ValStr = std::to_string(*(U32*)Val.Data.data());
                                    break;
                            }
                            VINFO("{}: {}", Key, ValStr);
                        }
                    }
                    else if(CurrentDialog == DialogType::UNSELF)
                    {
                        auto Obj = LoadSELF({FileDialog.GetSelected().string().c_str()});

                        auto* F = fopen("VOLTS.BIN", "wb");
                        fwrite(Obj.GetData(), 1, Obj.Len(), F);
                        fclose(F);
                    }
                    FileDialog.ClearSelected();
                }
            }
            ImGui::End();
        }
    }

    void Emulator::UpdateDeviceNames()
    {
        auto* Devices = Render.Current()->Devices(&DeviceCount);
        if(!Devices)
            return;

        DeviceNames = new char*[DeviceCount];
        for(U32 I = 0; I < DeviceCount; I++)
            DeviceNames[I] = strdup(Devices[I].Name());
    }

    void Emulator::Run()
    {
        Cfg.Init();
        auto& IO = ImGui::GetIO();
        IO.IniFilename = "Config/imgui.ini";

        // todo: the cursor dissapears when enabling scaling
        // i dont know why
        double Scale = Cfg.GetGuiScale();
        ImGui::GetStyle().ScaleAllSizes(Scale);
        ImFontConfig FontCfg;
        FontCfg.SizePixels = 13.f * Scale;
        ImGui::GetIO().Fonts->AddFontDefault(&FontCfg)->DisplayOffset.y = Scale;

        Render.Finalize();
        Input.Finalize();
        Audio.Finalize();

        Frame.Open();

        ImGui_ImplGlfw_InitForOpenGL(Frame, true);

        Render.Current()->Attach();
        Render.Current()->UpdateVSync(Cfg.GetVSync());

        while(!glfwWindowShouldClose(Frame))
        {
            auto* RenderBackend = Render.Current();
            RenderBackend->Begin();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame();
            GUI();
            ImGui::Render();

            RenderBackend->End();
            glfwPollEvents();
        }

        Render.Current()->Detach();
        ImGui_ImplGlfw_Shutdown();

        Frame.Close();

        Cfg.Save();
    }
}