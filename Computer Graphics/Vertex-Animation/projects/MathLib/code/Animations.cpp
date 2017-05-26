#include "Animations.h"
#include <iostream>
#include <fstream>
#include "matrix4D.h"

namespace Animation
{
Animations::Animations()
{

}

Animations::~Animations()
{
	delete[] keyBuffer;
}

bool Animations::LoadAnimation(std::string filename)
{
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::binary);
	
	char buf12[12];
	char buf16[16];
	char buf24[24];
	char buf64[64];
	char eventBuf64[64];
	
	if(file.fail())
	{
		printf("ERROR: Cannot open the file\n");
		return false;
	}
	
	Nax3Header* header;
	file.read(&buf12[0], sizeof(Nax3Header));
	header = (Nax3Header*) buf12;
	
	if(NAX3_MAGICNUMBER != header->magic)
	{
		printf("Not a valid file\n");
		return false;
	}

	if(header->numClips > 0)
	{
		int clipIndex;
		int numClips = header->numClips;
		keyBuffer = new CGMath::quaternions[header->numKeys];

		for(clipIndex = 0; clipIndex < numClips; clipIndex++)
		{
			Nax3Clip* naxClip;
			file.read(&buf64[0], sizeof(Nax3Clip));
			naxClip = (Nax3Clip*)buf64;
			
			AnimClip clip;
			clip.name = naxClip->name;
			clip.startKeyIndex = naxClip->startKeyIndex;
			clip.numKeys = naxClip->numKeys;
			clip.keyStride = naxClip->keyStride;
			clip.keyDuration = naxClip->keyDuration;
			
			clips.push_back(clip);
			int eventIndex;
			for(eventIndex = 0; eventIndex < naxClip->numEvents; eventIndex++)
			{
				file.read(&eventBuf64[0], sizeof(Nax3AnimEvent));
				Nax3AnimEvent* event = (Nax3AnimEvent*)eventBuf64;
				//Don't care about events
			}
			
			int curveIndex;
			std::vector<AnimCurve> curveVec;
			for(curveIndex = 0; curveIndex < naxClip->numCurves; curveIndex++)
			{
				AnimCurve curve;
				Nax3Curve* naxCurve;
				file.read(&buf24[0], sizeof(Nax3Curve));
				naxCurve = (Nax3Curve*) buf24;
				
				curve.firstKeyIndex = naxCurve->firstKeyIndex;
				curve.staticKey = CGMath::vector4D(naxCurve->staticKeyX, naxCurve->staticKeyY, naxCurve->staticKeyZ,naxCurve->staticKeyW);
				curve.isActive = naxCurve->isActive;
				curve.isStatic = naxCurve->isStatic;
				curve.curveType = (Code)naxCurve->curveType;
				curveVec.push_back(curve);
				
				
			}
			curves.push_back(curveVec);
			
		}
		
		int bufferSize = sizeof(CGMath::quaternions)*header->numKeys;
		char* arr = new char[bufferSize];
		file.read(arr, bufferSize);
		memcpy(keyBuffer, (CGMath::quaternions*)arr, 16*header->numKeys);
		printf("Animations Loaded!\n");
	}
	
	file.close();
	return true;
}

void Animations::PlayAnimation(int clipIndex, int keyIndex, CGMath::Skeleton* skel, float deltaTime)
{
	//printf("Clip Name: %s\nClip startKeyIndex: %i\n",clips[clipIndex].name.c_str(), clips[clipIndex].startKeyIndex);

	int keyIncrement = (keyIndex += deltaTime) % clips[clipIndex].numKeys;
	//printf("Curvetype: %i\n",curves[clipIndex][keyIndex].curveType);
	CGMath::matrix4D posMat;
	CGMath::matrix4D rotMat;
	for(int j = 0; j < curves[clipIndex].size(); j++)
	{
		int joint = j*0.25f;
		if(curves[clipIndex][j].curveType == Code::Translation)
		{
			posMat = CGMath::matrix4D::translationMatrix(CGMath::quaternions::QuaternionToVec4(keyBuffer[curves[clipIndex][j].firstKeyIndex+keyIncrement*curves[clipIndex].size()]));
		}
		if(curves[clipIndex][j].curveType == Code::Rotation)
		{
			rotMat = CGMath::matrix4D::RotationQuaternion(keyBuffer[curves[clipIndex][j].firstKeyIndex+keyIncrement*curves[clipIndex].size()]);
		}
		skel->JointUpdatePosition(joint, posMat,rotMat);
	}

}

std::string Animations::GetClipName(int clipIndex)
{
	return clips[clipIndex].name;
}

int Animations::GetClipAmount()
{
	return clips.size();
}


}