#include "Skeleton.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


using namespace tinyxml2;


namespace CGMath
{
	Skeleton::Skeleton()
	{

	}

	Skeleton::~Skeleton()
	{
		
	}
	
	void Skeleton::CreateSkeleton(const char* filename)
	{
		XMLDocument doc;
		doc.LoadFile(filename);
		XMLElement* elem = doc.FirstChildElement();
		const char* elemName = elem->Name();
		while(true)
		{
			if(strcmp(elemName, "Skinlist") && strcmp(elemName, "Joint"))
			{
				elem = elem->FirstChildElement();
				elemName = elem->Name();

			}
			else
			{
                if(!strcmp(elemName, "Skinlist"))
                    skelName = elem->FirstAttribute()->Value();
				elem = elem->NextSiblingElement();
				if(elem == nullptr)
					break;
				
				elemName = elem->Name();
				Joint j;
				j.name = elem->Attribute("name");
				j.parentIndex = std::atoi(elem->Attribute("parent"));
				j.jointIndex = std::atoi(elem->Attribute("index"));
				std::string str;
				str = elem->Attribute("position");
				j.position = vectorStringParse(str);
				str = elem->Attribute("rotation");
				j.rotation = vectorStringParse(str);
				str = elem->Attribute("scale");
				j.scale = vectorStringParse(str);
				mat4 mat;
				mat = mat4::translationMatrix(j.position);
				j.mat = mat;
				jointMatrices.push_back(j.mat);
				jointArray.push_back(j);
			}
		}
		XMLElement* elemJoints = doc.FirstChildElement()->FirstChildElement()->FirstChildElement()->NextSiblingElement()->FirstChildElement()->FirstChildElement()->FirstChildElement();
		std::vector<std::string> vec = tokenize(elemJoints->GetText(), ", ");
		for(int i = 0; i < vec.size(); i++)
		{
			jointIndex.push_back(std::atoi(vec[i].c_str()));
		}
		SetJointChilds();
		JointWorldPosition(0);
		std::cout << "Done" << std::endl;
	}
	
	void Skeleton::SetJointChilds()
	{
		for(int i = 0; i < jointArray.size(); ++i)
		{
			if(jointArray[i].parentIndex == -1)
				continue;
			jointArray[jointArray[i].parentIndex].childIndices.push_back(i);
		}
	}
	
	void Skeleton::JointWorldPosition(int index)
	{
		if(jointArray[index].childIndices.size() == 0)
		{
			return;
		}
		jointArray[index].mat = jointArray[index].jointMatrix();
		for(int i = index; i < jointArray.size(); ++i)
		{
			for(int j = 0; j < jointArray[i].childIndices.size(); ++j)
			{
				jointArray[jointArray[i].childIndices[j]].mat = (jointArray[i].mat*jointArray[jointArray[i].childIndices[j]].jointMatrix());
			}
		}

        for(int i = 0; i < jointArray.size(); ++i)
        {
            originalMatrices.push_back(jointArray[i].mat.invert());
        }

	}
	
	void Skeleton::JointUpdatePosition(int index, matrix4D m)
	{
		if(jointArray[index].childIndices.size() == 0)
		{
			jointArray[index].mat = jointArray[jointArray[index].parentIndex].mat*jointArray[index].jointMatrix(m);
			return;
		}
		if(jointArray[index].parentIndex != -1)
			jointArray[index].mat = jointArray[jointArray[index].parentIndex].mat*jointArray[index].jointMatrix(m);
		else
			jointArray[index].mat = jointArray[index].jointMatrix(m);
		for(int i = index; i < jointArray.size(); ++i)
		{
			for(int j = 0; j < jointArray[i].childIndices.size(); ++j)
			{
				jointArray[jointArray[i].childIndices[j]].mat = (jointArray[i].mat*jointArray[jointArray[i].childIndices[j]].jointMatrix());
			}
		}
	}
	
    void Skeleton::JointUpdatePosition(int index, matrix4D pos, matrix4D rot)
	{
		if(jointArray[index].childIndices.size() == 0)
		{
			jointArray[index].mat = jointArray[jointArray[index].parentIndex].mat*jointArray[index].jointMatrix(pos,rot);
			return;
		}
		if(jointArray[index].parentIndex != -1)
			jointArray[index].mat = jointArray[jointArray[index].parentIndex].mat*jointArray[index].jointMatrix(pos,rot);
		else
			jointArray[index].mat = jointArray[index].jointMatrix(pos,rot);
		for(int i = index; i < jointArray.size(); ++i)
		{
			for(int j = 0; j < jointArray[i].childIndices.size(); ++j)
			{
				jointArray[jointArray[i].childIndices[j]].mat = (jointArray[i].mat*jointArray[jointArray[i].childIndices[j]].jointMatrix());
			}
		}

        for(int i = 0; i < jointArray.size(); ++i)
        {
            jointMatrices[i] = (jointArray[i].mat*originalMatrices[i]);
        }
		
	}
	
	
	Skeleton::Joint& Skeleton::GetJoint(int index)
	{
		return jointArray[index];
	}

	
	std::vector<Skeleton::Joint>& Skeleton::GetJointArray()
	{
		return this->jointArray;
	}
	
	vector4D Skeleton::vectorStringParse(std::string s)
	{
		std::vector<std::string> tokens = tokenize(s,",");
		float x = std::atof(tokens[0].c_str());
		float y = std::atof(tokens[1].c_str());
		float z = std::atof(tokens[2].c_str());
		float w = std::atof(tokens[3].c_str());
		
		vec4 vec(x,y,z,w);
		
		return vec;
	}
	
	std::vector<std::string> Skeleton::tokenize(std::string str, std::string delim)
	{
		std::vector<std::string> tokens;
		
		int i = 0;
		auto pos = str.find(delim);
		while(pos != std::string::npos)
		{
			tokens.push_back(str.substr(i, pos-i));
			i = ++pos;
			pos = str.find(delim, pos);
			
			if (pos == std::string::npos)
				tokens.push_back(str.substr(i, str.length()));
		}
		
		return tokens;
	}

    std::string Skeleton::GetSkelName() {
        return skelName;
    }


}