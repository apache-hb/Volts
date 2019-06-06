﻿
#include "Volts.h"

#include "PS3/Util/Decrypt/UNSELF.h"
#include "PS3/VM/Interpreter/PPUInterpreter.h"

using namespace Volts;
using namespace Volts::PS3;
using namespace Cthulhu;

// entry point, nothing here yet as right now we're just testing very basic parts
int main(int argc, const char** argv)
{
	LogLevel = Level::Trace;
	FileSystem::BufferedFile F(argv[1]);

	PPUInterpreter Interp;
	Binary B;

	Byte* Buf = new Byte[F.Size()];
	F.ReadN(Buf, F.Size());

	B.WriteN(Buf, F.Size());
	Interp.Run(B);
}
