/*********************************************************\
 * Copyright (c) 2012-2022 The Unrimp Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "ExampleProjectCompiler/Private/CommandLineArguments.h"

#include <RendererToolkit/Public/RendererToolkitInstance.h>

#include <Renderer/Public/Core/File/FileSystemHelper.h>
#include <Renderer/Public/Core/File/DefaultFileManager.h>

#include <Rhi/Public/DefaultLog.h>
#include <Rhi/Public/DefaultAssert.h>
#ifdef EXAMPLES_MIMALLOC
	#include <Rhi/Public/MimallocAllocator.h>
#else
	#include <Rhi/Public/DefaultAllocator.h>
#endif


//[-------------------------------------------------------]
//[ Platform independent program entry point              ]
//[-------------------------------------------------------]
[[nodiscard]] int programEntryPoint(const CommandLineArguments& commandLineArguments)
{
	Rhi::DefaultLog log;
	Rhi::DefaultAssert assert;
	#ifdef EXAMPLES_MIMALLOC
		Rhi::MimallocAllocator allocator;
	#else
		Rhi::DefaultAllocator allocator;
	#endif
	Renderer::DefaultFileManager defaultFileManager(log, assert, allocator, std_filesystem::canonical(std_filesystem::current_path() / "..").generic_string());
	RendererToolkit::Context rendererToolkitContext(log, assert, allocator, defaultFileManager);
	RendererToolkit::RendererToolkitInstance rendererToolkitInstance(rendererToolkitContext);
	RendererToolkit::IRendererToolkit* rendererToolkit = rendererToolkitInstance.getRendererToolkit();
	if (nullptr != rendererToolkit)
	{
		RendererToolkit::IProject* project = rendererToolkit->createProject();
		try
		{
			// Load project: Shippable executable binaries are inside e.g. "unrimp/Binary/Windows_x64_Shared" while development data source is located
			// at "unrimp/Example/DataSource/Example" and the resulting compiled/baked data ends up inside e.g. "unrimp/Binary/DataPc/Example"
			project->load("../../Example/DataSource/Example");

			if (commandLineArguments.getArguments().empty())
			{
				//	project->compileAllAssets("Direct3D9_30");
					project->compileAllAssets("Direct3D11_50");
				//	project->compileAllAssets("Direct3D12_50");
				//	project->compileAllAssets("OpenGLES3_300");
				//	project->compileAllAssets("OpenGL_440");
			}
			else
			{
				// For now all given arguments are interpreted as render target
				for (const std::string_view& renderTarget : commandLineArguments.getArguments())
				{
					const std::string renderTargetString(renderTarget);
					RHI_LOG(rendererToolkitContext, INFORMATION, "Compiling for target: \"%s\"", renderTargetString.c_str())
					project->compileAllAssets(renderTargetString.c_str());
					RHI_LOG(rendererToolkitContext, INFORMATION, "Compilation done")
				}
			}
		}
		catch (const std::exception& e)
		{
			RHI_LOG(rendererToolkitContext, CRITICAL, "Project compilation failed: %s", e.what())
			RHI_LOG(rendererToolkitContext, INFORMATION, "Press any key to continue")
			getchar();
		}
		delete project;
	}

	// No error
	return 0;
}


//[-------------------------------------------------------]
//[ Platform dependent program entry point                ]
//[-------------------------------------------------------]
// Windows implementation
#ifdef _WIN32
	#include <Renderer/Public/Core/Platform/WindowsHeader.h>

	#ifdef _CONSOLE
		#ifdef UNICODE
			int wmain(int, wchar_t**)
		#else
			int main(int, char**)
		#endif
			{
				// For memory leak detection
				#ifdef RHI_DEBUG
					// "_CrtDumpMemoryLeaks()" reports false positive memory leak with static variables, so use a memory difference instead
					_CrtMemState crtMemState = { };
					_CrtMemCheckpoint(&crtMemState);
				#endif

				// Call the platform independent program entry point
				// -> Uses internally "GetCommandLine()" to fetch the command line arguments
				const int result = programEntryPoint(CommandLineArguments());

				// For memory leak detection
				#ifdef RHI_DEBUG
					_CrtMemDumpAllObjectsSince(&crtMemState);
				#endif

				// Done
				return result;
			}
	#else
		#ifdef UNICODE
			int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
		#else
			int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
		#endif
			{
				// Call the platform independent program entry point
				// -> Uses internally "GetCommandLine()" to fetch the command line arguments
				const int result = programEntryPoint(CommandLineArguments());

				// For memory leak detection
				#ifdef RHI_DEBUG
					_CrtDumpMemoryLeaks();
				#endif

				// Done
				return result;
			}
	#endif

// Linux implementation
#elif LINUX
	int main(int argc, char** argv)
	{
		// Call the platform independent program entry point
		return programEntryPoint(CommandLineArguments(argc, argv));
	}
#endif
