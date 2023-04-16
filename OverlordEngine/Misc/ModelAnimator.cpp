#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{

	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		float deltaTima = sceneContext.pGameTime->GetElapsed();

		//1. 
		//Calculate the passedTicks (see the lab document)
		auto passedTicks = deltaTima * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		passedTicks = fmodf(passedTicks, m_CurrentClip.duration);

		//2. 
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0.f)
			{
				m_TickCount += m_CurrentClip.duration;
			}
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.duration)
			{
				m_TickCount -= m_CurrentClip.duration;
			}
		}

		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		auto upperBoundPredicate = [](float valueToCompare, const AnimationKey& key)
		{
			return valueToCompare < key.tick;
		};
		auto upper = std::upper_bound(m_CurrentClip.keys.begin(), m_CurrentClip.keys.end(), m_TickCount, upperBoundPredicate);
		keyB = *upper;
		keyA = (upper == m_CurrentClip.keys.begin()) ? *upper : *(upper - 1);

		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		//(value - start) / (end - start)
		float blendFactor = (m_TickCount - keyA.tick) / (keyB.tick - keyA.tick);
		
		//FOR every boneTransform in a key (So for every bone)
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector
		for (USHORT i = 0; i <m_pMeshFilter->GetBoneCount() ; i++)
		{
			auto transformA = keyA.boneTransforms[i];
			auto transformB = keyB.boneTransforms[i];

			XMVECTOR transA, scaleA, rotQuatA, transB, scaleB, rotQuatB;
			XMMatrixDecompose(&scaleA, &rotQuatA, &transA, XMLoadFloat4x4(&transformA));
			XMMatrixDecompose(&scaleB, &rotQuatB, &transB, XMLoadFloat4x4(&transformB));

			auto transLerped = XMVectorLerp(transA, transB, blendFactor);
			auto scaleLerped = XMVectorLerp(scaleA, scaleB, blendFactor);
			auto rotLerped = XMQuaternionSlerp(rotQuatA, rotQuatB, blendFactor);
			auto transformLerped = XMMatrixAffineTransformation(scaleLerped, XMVectorZero(), rotLerped, transLerped);

			XMStoreFloat4x4(&m_Transforms[i], transformLerped);
		}
		
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	auto clips = m_pMeshFilter->GetAnimationClips();
	auto searchPredicate = [&clipName](const AnimationClip& clip)
	{
		return clip.name == clipName;
	};
	auto elem = std::find_if(clips.begin(), clips.end(), searchPredicate);
	//If found,

	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
	if (elem == clips.end())
	{
		Reset(true);
		Logger::LogWarning(L"Animation does not exist: " + clipName);
	}
	else
	{
		SetAnimation(*elem);
	}
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{

	//Set m_ClipSet to false
	m_ClipSet = false;
	auto clips = m_pMeshFilter->GetAnimationClips();
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)

	if (!(clipNumber < clips.size()))
	{
		Reset(true);
		Logger::LogWarning(L"Animation does not exist with the following clipNumer: " + clipNumber);
	}
	else
	{
		SetAnimation(clips[clipNumber]);
	}
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	
	//Set m_ClipSet to true
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;
	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	
	//If pause is true, set m_IsPlaying to false
	if (pause)
	{
		m_IsPlaying = false;
	}
	m_TickCount = 0;
	m_AnimationSpeed = 1.0f;
	//Set m_TickCount to zero
	//Set m_AnimationSpeed to 1.0f

	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)

	if (m_ClipSet)
	{
		auto boneTransform = m_CurrentClip.keys[0].boneTransforms;
		m_Transforms.assign(boneTransform.begin(), boneTransform.end());
	}
	else
	{
		XMFLOAT4X4 identityMat{};
		XMStoreFloat4x4(&identityMat, XMMatrixIdentity());
		m_Transforms.assign(static_cast<size_t>(m_pMeshFilter->GetBoneCount()), identityMat);
	}
}
