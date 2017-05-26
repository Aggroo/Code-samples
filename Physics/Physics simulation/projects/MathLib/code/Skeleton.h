#pragma once
#include "GL/glew.h"
#include "config.h"
#include "matrix4D.h"
#include "vector3D.h"
#include "tinyxml2.h"
#include <vector>

namespace CGMath
{
	
class Skeleton
{
public:
	Skeleton();
	~Skeleton();
	
	
	//Joint structure. Makes up a single joint in a skeleton
	struct Joint
	{
		std::string name;
		int parentIndex;
		int jointIndex;
		std::vector<int> childIndices;
		vector4D position;
		Quaternion rotation;
		vector4D scale;
		matrix4D mat;
		
		matrix4D jointMatrix()
		{
			matrix4D matrice;
			matrice = mat4::translationMatrix(position)* mat4::RotationQuaternion(rotation) *mat4::vectorScaling(scale);
			return matrice;
		}
		matrix4D jointMatrix(matrix4D rot)
		{
			matrix4D matrice;
			matrice = mat4::translationMatrix(position)* rot *mat4::vectorScaling(scale);
			return matrice;
		}
		matrix4D jointMatrix(matrix4D pos,matrix4D rot)
		{
			matrix4D matrice;
			matrice = pos*rot *mat4::vectorScaling(scale);
			return matrice;
		}
	};
	
	//Create a skeleton of joints from a xml file 
	void CreateSkeleton(const char* filename);
	
	//Loops through the jointArray and assign all child joints
	void SetJointChilds();
	//Loop through the jointArray to place them correct in the world
	void JointWorldPosition(int index);
	//Loop through the jointArray to update a joint and it's childs matrix
	void JointUpdatePosition(int index,  matrix4D pos, matrix4D rot);
	
	void JointUpdatePosition(int index,  matrix4D m);
	
	//Returns a joint from the jointArray
	Joint& GetJoint(int index);
    std::string GetSkelName();
	
	std::vector<Joint>& GetJointArray();
    std::vector<matrix4D> jointMatrices;
    std::vector<matrix4D> originalMatrices;

	std::vector<int> jointIndex;
	
	vector4D vectorStringParse(std::string s);	
	std::vector<std::string> tokenize(std::string str, std::string delim);


private:
	int jointArraySize;
	std::string skelName;
    std::vector<Joint> jointArray;

	
	
	
};

}