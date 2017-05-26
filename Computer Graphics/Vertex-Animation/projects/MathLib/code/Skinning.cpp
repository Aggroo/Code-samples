//
// Created by antgrn-3 on 9/26/16.
//
#include "Skinning.h"

namespace Animation
{
    Skinning::Skinning()
    {

    }

    Skinning::~Skinning()
    {

    }

    bool Skinning::LoadSkinMesh(std::string filename)
    {
        size_t result;
        uchar* buffer;
        size_t bufferSize;

        FILE* file = fopen(filename.c_str(),"r");
        if (!file)
        {
            printf("Unable to open file!\n");
            return false;
        }

        fseek(file, 0L, SEEK_END);
        bufferSize = ftell(file);
        rewind(file);
        buffer = new uchar[bufferSize];
        result = fread(buffer,1,bufferSize,file);
        fclose(file);


        ptr = buffer;

        Nvx2Header* header = (Nvx2Header*) ptr;
        ptr += sizeof(Nvx2Header);
        header->numIndices *= 3;

        if(header->magic != NVX2_MAGICNUMBER)
        {
            printf("Nvx2StreamReader: '%s' is not a nvx2 file!\n", filename.c_str());
            return false;
        }

        this->numGroups = header->numGroups;
        this->numVertices = header->numVertices;
        this->vertexWidth = header->vertexWidth;
        this->numIndices = header->numIndices;
        this->numEdges = header->numEdges;
        this->vertexComponentMask = header->vertexComponentMask;

        this->groupDataSize = 6 * sizeof(uint) * this->numGroups;
        this->vertexDataSize = this->numVertices * this->vertexWidth * sizeof(float);
        this->indexDataSize = this->numIndices * sizeof(int);

        this->groupDataPtr = header + 1;
        this->vertexDataPtr = ((uchar*)this->groupDataPtr) + this->groupDataSize;
        this->indexDataPtr = ((uchar*)this->vertexDataPtr) + this->vertexDataSize;

        assert(this->primGroups.empty());
        assert(this->numGroups > 0);
        assert(0 != this->groupDataPtr);
        Nvx2Group* group = (Nvx2Group*) this->groupDataPtr;
        uint j;
        for (j = 0; j < (size_t)this->numGroups; j++)
        {
            // setup a primitive group object
            PrimitiveGroup primGroup;
            //primGroup.SetBaseVertex(group->firstVertex);
            primGroup.baseVertex = group->firstVertex;
            primGroup.numVertices = group->numVertices;
            primGroup.baseIndex =group->firstTriangle * 3;
            primGroup.numIndices = group->numTriangles * 3;
            primGroup.primitiveTopology = PrimitiveGroup::Code::TriangleList;
            this->primGroups.push_back(primGroup);

            // set top next group
            group++;
        }

        assert(this->vertexComponents.empty());

        uint i;
        attrIndex = 0;
        size = 0;

        GLenum type;
        GLuint vertexStep = 0;
        for (i = 0; i < N2NumVertexComponents; i++)
        {
            VertexComponent::SemanticName sem;
            VertexComponent::Format fmt;
            uint index = 0;
            GLboolean normalized = GL_FALSE;

            GLuint valueNums = 0;

            if (vertexComponentMask & (1<<i))
            {
                switch (1<<i)
                {
                    case N2Coord:        type = GL_FLOAT; sem = VertexComponent::Position; valueNums = 3; fmt = VertexComponent::Float3; break;
                    case N2Normal:       type = GL_FLOAT; sem = VertexComponent::Normal; valueNums = 3; fmt = VertexComponent::Float3; break;
                    case N2NormalB4N:    type = GL_UNSIGNED_BYTE; normalized = GL_TRUE; sem = VertexComponent::Normal; valueNums = 4; fmt = VertexComponent::Byte4N; break;
                    case N2Uv0:          type = GL_FLOAT; sem = VertexComponent::TexCoord1; valueNums = 2; fmt = VertexComponent::Float2; index = 0; break;
                    case N2Uv0S2:        type = GL_SHORT; sem = VertexComponent::TexCoord1; valueNums = 2; fmt = VertexComponent::Short2; index = 0; break;
                    case N2Uv1:          type = GL_FLOAT; sem = VertexComponent::TexCoord2; valueNums = 2; fmt = VertexComponent::Float2; index = 1; break;
                    case N2Uv1S2:        type = GL_SHORT; sem = VertexComponent::TexCoord2; valueNums = 2; fmt = VertexComponent::Short2; index = 1; break;
                        /*
                    case N2Uv2:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 2; break;
                    case N2Uv2S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 2; break;
                    case N2Uv3:          sem = VertexComponent::TexCoord;     fmt = VertexComponent::Float2; index = 3; break;
                    case N2Uv3S2:        sem = VertexComponent::TexCoord;     fmt = VertexComponent::Short2; index = 3; break;
                    */
                    case N2Color:        type = GL_FLOAT; sem = VertexComponent::Color; valueNums = 4; fmt = VertexComponent::Float4; break;
                    case N2ColorUB4N:    type = GL_UNSIGNED_BYTE; normalized = GL_TRUE; sem = VertexComponent::Color; valueNums = 4; fmt = VertexComponent::UByte4N; break;
                    case N2Tangent:      type = GL_FLOAT; sem = VertexComponent::Tangent; valueNums = 3; fmt = VertexComponent::Float3; break;
                    case N2TangentB4N:   type = GL_UNSIGNED_BYTE; normalized = GL_TRUE; sem = VertexComponent::Tangent; valueNums = 4; fmt = VertexComponent::Byte4N; break;
                    case N2Binormal:     type = GL_FLOAT; sem = VertexComponent::Binormal; valueNums = 3; fmt = VertexComponent::Float3; break;
                    case N2BinormalB4N:  type = GL_UNSIGNED_BYTE; normalized = GL_TRUE; sem = VertexComponent::Binormal; valueNums = 4; fmt = VertexComponent::Byte4N; break;
                    case N2Weights:      type = GL_FLOAT; sem = VertexComponent::SkinWeights; valueNums = 4; fmt = VertexComponent::Float4; break;
                    case N2WeightsUB4N:  type = GL_UNSIGNED_BYTE; normalized = GL_TRUE; sem = VertexComponent::SkinWeights; valueNums = 4; fmt = VertexComponent::UByte4N; break;
                    case N2JIndices:     type = GL_FLOAT; sem = VertexComponent::SkinJIndices; valueNums = 4; fmt = VertexComponent::Float4; break;
                    case N2JIndicesUB4:  type = GL_UNSIGNED_BYTE; sem = VertexComponent::SkinJIndices; valueNums = 4; fmt = VertexComponent::UByte4; break;
                    default:
                        printf("Invalid Nebula2 VertexComponent in Nvx2StreamReader::SetupVertexComponents\n");
                        sem = VertexComponent::Position;
                        fmt = VertexComponent::Float3;
                        break;
                }
                size += GetByteSize(fmt);
                vertexStep += GetByteSize(fmt);

                offset.push_back(vertexStep);
                types.push_back(type);
                typeNumbers.push_back(valueNums);
                norms.push_back(normalized);

                attrIndex++;
                this->vertexComponents.push_back(Animation::VertexComponent(sem, index, fmt));
            }
        }

        return true;
    }

    MeshResources* Skinning::GenSkinningMesh()
    {
        MeshResources* mesh = new MeshResources();
        mesh->genSkinnedBuffer(vertexDataSize, (uchar*)vertexDataPtr, indexDataSize, (uchar*)indexDataPtr, this->numIndices);
        for(int i = 0; i < attrIndex; ++i)
        {
            if(i == 0)
                glVertexAttribPointer(vertexComponents[i].semName, typeNumbers[i], types[i], norms[i], size, NULL);
            else
                glVertexAttribPointer(vertexComponents[i].semName, typeNumbers[i], types[i], norms[i], size, (GLvoid*)offset[i-1]);
            glEnableVertexAttribArray(vertexComponents[i].semName);
        }
        return mesh;
    }

    size_t Skinning::GetByteSize(VertexComponent::Format format) const
    {
        switch (format)
        {
            case VertexComponent::Format::Float:     return 4;
            case VertexComponent::Format::Float2:    return 8;
            case VertexComponent::Format::Float3:    return 12;
            case VertexComponent::Format::Float4:    return 16;
            case VertexComponent::Format::UByte4:    return 4;
            case VertexComponent::Format::Byte4:		return 4;
            case VertexComponent::Format::Short2:    return 4;
            case VertexComponent::Format::Short4:    return 8;
            case VertexComponent::Format::UByte4N:   return 4;
            case VertexComponent::Format::Byte4N:	return 4;
            case VertexComponent::Format::Short2N:   return 4;
            case VertexComponent::Format::Short4N:   return 8;

        }
        printf("Can't happen\n");
        return 0;
    }


}