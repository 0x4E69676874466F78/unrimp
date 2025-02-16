/*********************************************************\
 * Copyright (c) 2012-2021 The Unrimp Team
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


// Amalgamated/unity build
#ifdef _WIN32
	// Disable warnings in external headers, we can't fix them
	__pragma(warning(disable: 4355))	// warning C4355: 'this': used in base member initializer list
	__pragma(warning(disable: 4619))	// warning C4619: #pragma warning: there is no warning number '4351'
	__pragma(warning(disable: 4777))	// warning C4777: 'snprintf' : format string '%Iu' requires an argument of type 'unsigned int', but variadic argument 1 has type 'const size_t'
	__pragma(warning(disable: 4826))	// warning C4826: Conversion from 'aiBone **' to 'uint64_t' is sign-extended. This may cause unexpected runtime behavior.
#endif
#include "Configuration.h"
// AssetLib
	// Common
	#include "code/Common/AssertHandler.cpp"
	#include "code/Common/Assimp.cpp"
	#include "code/Common/BaseImporter.cpp"
	#include "code/Common/BaseProcess.cpp"
	#include "code/Common/Bitmap.cpp"
	#include "code/Common/CreateAnimMesh.cpp"
	#include "code/Common/DefaultIOStream.cpp"
	#include "code/Common/DefaultIOSystem.cpp"
	#include "code/Common/DefaultLogger.cpp"
	#undef max	// Get rid of some nasty OS macros
	#include "code/Common/Exporter.cpp"
	#undef min				// Get rid of some nasty OS macros
	#undef CreateDirectory	// Get rid of some nasty OS macros
	#undef DeleteFile		// Get rid of some nasty OS macros
	#include "code/Common/Importer.cpp"
	#include "code/Common/ImporterRegistry.cpp"
	#include "code/Common/material.cpp"
	#include "code/Common/PostStepRegistry.cpp"
	#include "code/Common/RemoveComments.cpp"
	#include "code/Common/scene.cpp"
	#include "code/Common/SceneCombiner.cpp"
	#include "code/Common/ScenePreprocessor.cpp"
	#include "code/Common/SGSpatialSort.cpp"
	#include "code/Common/simd.cpp"
	#include "code/Common/SkeletonMeshBuilder.cpp"
	#include "code/Common/SpatialSort.cpp"
	#include "code/Common/StandardShapes.cpp"
	#include "code/Common/Subdivision.cpp"
	#include "code/Common/TargetAnimation.cpp"
	#include "code/Common/Version.cpp"
	#include "code/Common/VertexTriangleAdjacency.cpp"
	#include "code/Common/ZipArchiveIOSystem.cpp"
	// Blender
	#include "code/AssetLib/Blender/BlenderScene.cpp"
	// Collada
	#include "code/AssetLib/Collada/ColladaLoader.cpp"
	// MD5
	#include "code/AssetLib/MD5/MD5Loader.cpp"
	// Material
	#include "code/Material/MaterialSystem.cpp"
