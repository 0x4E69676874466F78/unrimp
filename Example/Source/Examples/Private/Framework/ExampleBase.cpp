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
#include "Examples/Private/Framework/ExampleBase.h"
#include "Examples/Private/Framework/IApplicationFrontend.h"


//[-------------------------------------------------------]
//[ Public methods                                        ]
//[-------------------------------------------------------]
Rhi::IRhi* ExampleBase::getRhi() const
{
	return (nullptr != mApplicationFrontend) ? mApplicationFrontend->getRhi() : nullptr;
}

Rhi::IRenderTarget* ExampleBase::getMainRenderTarget() const
{
	return (nullptr != mApplicationFrontend) ? mApplicationFrontend->getMainRenderTarget() : nullptr;
}

Renderer::IRenderer* ExampleBase::getRenderer() const
{
	return (nullptr != mApplicationFrontend) ? mApplicationFrontend->getRenderer() : nullptr;
}

Renderer::IRenderer& ExampleBase::getRendererSafe() const
{
	ASSERT(nullptr != mApplicationFrontend, "Invalid application frontend")
	Renderer::IRenderer* renderer = mApplicationFrontend->getRenderer();
	ASSERT(nullptr != renderer, "Invalid renderer instance")
	return *renderer;
}

RendererToolkit::IRendererToolkit* ExampleBase::getRendererToolkit()
{
	return (nullptr != mApplicationFrontend) ? mApplicationFrontend->getRendererToolkit() : nullptr;
}

void ExampleBase::switchExample(const char* exampleName, const char* rendererName)
{
	ASSERT(nullptr != exampleName, "Invalid example name")
	if (nullptr != mApplicationFrontend)
	{
		mApplicationFrontend->switchExample(exampleName, rendererName);
	}
}

void ExampleBase::exit()
{
	if (nullptr != mApplicationFrontend)
	{
		mApplicationFrontend->exit();
	}
}
