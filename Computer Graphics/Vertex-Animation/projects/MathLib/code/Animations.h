#ifndef ANIMATIONS_H
#define ANIMATIONS_H
#include "naxfileformatstructs.h"
#include "vector4D.h"
#include <string>
#include <cstring>
#include <vector>
#include "Skeleton.h"

namespace Animation
{
	
enum Code
    {
        Translation,    //> keys in curve describe a translation
        Scale,          //> keys in curve describe a scale
        Rotation,       //> keys in curve describe a rotation quaternion
        Color,          //> keys in curve describe a color
        Velocity,       //> keys describe a linear velocition
        Float4,         //> generic 4D key

        NumCurveTypes,
        InvalidCurveType,
};

struct AnimClip
{
	std::string name;
	uint startKeyIndex;
	uint numKeys;			//Number of keys in from firstKeyIndex in a AnimCurve
	uint keyStride;
	ushort keyDuration;
};

struct AnimCurve
{
	CGMath::vec4 staticKey;
	int firstKeyIndex;		//Start key in keyBuffer of which the AnimCurve starts
	bool isActive;
	bool isStatic;
	Code curveType;
};

class Animations
{
public:
Animations();
~Animations();

bool LoadAnimation(std::string filename);
void PlayAnimation(int clipIndex, int keyIndex, CGMath::Skeleton* skel, float deltaTime);
std::string GetClipName(int clipIndex);
int GetClipAmount();


private:
	
	std::vector<AnimClip> clips;
	std::vector<std::vector<AnimCurve>> curves;
	CGMath::quaternions* keyBuffer;
};

}
#endif // ANIMATIONS_H