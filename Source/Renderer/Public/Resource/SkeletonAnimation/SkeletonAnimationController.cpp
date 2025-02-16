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
#include "Renderer/Public/Resource/SkeletonAnimation/SkeletonAnimationController.h"
#include "Renderer/Public/Resource/SkeletonAnimation/SkeletonAnimationEvaluator.h"
#include "Renderer/Public/Resource/SkeletonAnimation/SkeletonAnimationResourceManager.h"
#include "Renderer/Public/Resource/Skeleton/SkeletonResourceManager.h"
#include "Renderer/Public/Resource/Skeleton/SkeletonResource.h"
#include "Renderer/Public/IRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace Renderer
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void SkeletonAnimationController::startSkeletonAnimationByResourceId(SkeletonAnimationResourceId skeletonAnimationResourceId)
	{
		clear();
		mSkeletonAnimationResourceId = skeletonAnimationResourceId;
		if (isValid(skeletonAnimationResourceId))
		{
			mRenderer.getSkeletonAnimationResourceManager().getResourceByResourceId(skeletonAnimationResourceId).connectResourceListener(*this);
		}
	}

	void SkeletonAnimationController::startSkeletonAnimationByAssetId(AssetId skeletonAnimationAssetId)
	{
		clear();
		mRenderer.getSkeletonAnimationResourceManager().loadSkeletonAnimationResourceByAssetId(skeletonAnimationAssetId, mSkeletonAnimationResourceId, this);
	}

	void SkeletonAnimationController::clear()
	{
		if (isValid(mSkeletonAnimationResourceId))
		{
			disconnectFromResourceById(mSkeletonAnimationResourceId);
			setInvalid(mSkeletonAnimationResourceId);
		}
		destroySkeletonAnimationEvaluator();
		mTimeInSeconds = 0.0f;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual Renderer::IResourceListener methods ]
	//[-------------------------------------------------------]
	void SkeletonAnimationController::onLoadingStateChange(const IResource& resource)
	{
		if (resource.getLoadingState() == IResource::LoadingState::LOADED)
		{
			createSkeletonAnimationEvaluator();
		}
		else
		{
			destroySkeletonAnimationEvaluator();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SkeletonAnimationController::createSkeletonAnimationEvaluator()
	{
		RHI_ASSERT(mRenderer.getContext(), nullptr == mSkeletonAnimationEvaluator, "No useless update calls, please")
		SkeletonAnimationResourceManager& skeletonAnimationResourceManager = mRenderer.getSkeletonAnimationResourceManager();
		mSkeletonAnimationEvaluator = new SkeletonAnimationEvaluator(skeletonAnimationResourceManager, mSkeletonAnimationResourceId);

		// Register skeleton animation controller
		skeletonAnimationResourceManager.mSkeletonAnimationControllers.push_back(this);
	}

	void SkeletonAnimationController::destroySkeletonAnimationEvaluator()
	{
		if (nullptr != mSkeletonAnimationEvaluator)
		{
			{ // Unregister skeleton animation controller
				SkeletonAnimationResourceManager::SkeletonAnimationControllers& skeletonAnimationControllers = mRenderer.getSkeletonAnimationResourceManager().mSkeletonAnimationControllers;
				SkeletonAnimationResourceManager::SkeletonAnimationControllers::iterator iterator = std::find(skeletonAnimationControllers.begin(), skeletonAnimationControllers.end(), this);
				RHI_ASSERT(mRenderer.getContext(), iterator != skeletonAnimationControllers.end(), "Invalid skeleton animation controller")
				skeletonAnimationControllers.erase(iterator);
			}

			// Destroy skeleton animation evaluator
			delete mSkeletonAnimationEvaluator;
			mSkeletonAnimationEvaluator = nullptr;
		}
	}

	void SkeletonAnimationController::update(float pastSecondsSinceLastFrame)
	{
		// Sanity check
		RHI_ASSERT(mRenderer.getContext(), pastSecondsSinceLastFrame > 0.0f, "No negative time, please")
		RHI_ASSERT(mRenderer.getContext(), nullptr != mSkeletonAnimationEvaluator, "No useless update calls, please")

		// Advance time and evaluate state
		mTimeInSeconds += pastSecondsSinceLastFrame;
		mSkeletonAnimationEvaluator->evaluate(mTimeInSeconds);

		{ // Tell the controlled skeleton resource about the new state
			SkeletonResource& skeletonResource = mRenderer.getSkeletonResourceManager().getById(mSkeletonResourceId);
			const SkeletonAnimationEvaluator::BoneIds& boneIds = mSkeletonAnimationEvaluator->getBoneIds();
			const SkeletonAnimationEvaluator::TransformMatrices& transformMatrices = mSkeletonAnimationEvaluator->getTransformMatrices();
			glm::mat4* localBoneMatrices = skeletonResource.getLocalBoneMatrices();
			for (size_t i = 0; i < boneIds.size(); ++i)
			{
				const uint32_t boneIndex = skeletonResource.getBoneIndexByBoneId(boneIds[i]);
				if (isValid(boneIndex))
				{
					localBoneMatrices[boneIndex] = transformMatrices[i];
				}
			}
			skeletonResource.localToGlobalPose();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // Renderer
