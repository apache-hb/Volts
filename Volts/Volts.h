#pragma once

#include "Volts/Core/Emulator.h"

#include <filesystem>
#include <fstream>

#include "Volts/Utils/SFO.h"
#include "Volts/Utils/UNSELF.h"

#define CXXOPTS_NO_EXCEPTIONS
#define CXXOPTS_NO_RTTI

#include "cxxopts/cxxopts.h"

#include <argo.hpp>

#if OS_WINDOWS
#   include <io.h>
#   include <fcntl.h>
#endif

namespace Volts::Args
{
    using namespace Cthulhu;

    using namespace Utils;

    using namespace std;

    namespace fs = std::filesystem;

    struct CLI
    {
        cxxopts::Options Opts;

        CLI()
            : Opts("volt", "Command line emulator interface")
        {
#if OS_WINDOWS
            if(AttachConsole(ATTACH_PARENT_PROCESS))
            {
                freopen("CONOUT$", "w", stdout);
                freopen("CONOUT$", "w", stderr);
            }
#endif
        }

        static CLI* Get()
        {
            static CLI* Singleton = new CLI();
            return Singleton;
        }

        CLI* Build()
        {
            Opts.add_options()
                ("H,help", "Display help message then exit")
                ("P,play", "Play the game at the desired location", cxxopts::value<std::string>())
                ("S,sfo", "Parse a SFO and dump to json", cxxopts::value<std::string>())
                ("U,unself", "Decrypt a SELF and write to a file", cxxopts::value<std::string>())
                ("O,output", "A location to output data to", cxxopts::value<std::string>());
            return this;
        }

        void Run(int Argc, char** Argv)
        {
            auto Res = Opts.parse(Argc, Argv);

            if(Res.count("output"))
            {
                auto Path = Res["output"].as<std::string>();

                static auto Pipe = std::ofstream();
                Pipe.open(Path, std::ofstream::out);
                Emulator::Get()->OutStream = &Pipe;
            }

            if(Res.count("help"))
            {
                // print help then exit
                VINFO(Opts.help());
            }

            if(Res.count("unself"))
            {
                auto Path = Res["unself"].as<std::string>();
                if(!fs::exists(Path))
                {
                    VFATAL("SELF as {} was not found", Path);
                    exit(1);
                }

                auto SELF = Utils::LoadSELF({Path.c_str()});

                if(SELF.Len() == 0)
                {
                    VFATAL("Failed to decrypt SELF");
                    exit(1);
                }

                std::ofstream Output("out.elf", ios::out | ios::binary);

                Output.write((const char*)SELF.GetData(), SELF.Len());

                Output.close();

                VINFO("Done {}", SELF.Len());
            }

            if(Res.count("sfo"))
            {
                auto Path = Res["sfo"].as<std::string>();
                if(!fs::exists(Path))
                {
                    VFATAL("SFO File at {} was not found", Path);
                    exit(1);
                }

                auto SFO = Utils::LoadSFO({Path.c_str()});

                argo::json Output{argo::json::object_e};

                for(auto& [Key, Val] : SFO)
                {
                    switch(Val.Type)
                    {
                        case Format::String:
                            Output[Key] = (char*)Val.Data.data();
                            break;
                        case Format::Array:
                        {
                            argo::json V{argo::json::array_e};
                            for(auto& B : Val.Data)
                                V.append(B);

                            Output[Key] = V;
                            break;
                        }
                        case Format::Integer:
                            Output[Key] = *(I32*)Val.Data.data();
                            break;
                        default:
                            Output[Key] = nullptr;
                            break;
                    }
                }

                argo::unparser::unparse(*Emulator::Get()->OutStream, Output, "", "\n", "    ", 1);
            }
        }
    };
}