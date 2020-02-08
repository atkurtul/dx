#include "AnimatedModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

List<String> GetNames(const aiScene* scene)
{
	List<String> jointIDs;
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; ++j)
		{
			String current = scene->mMeshes[i]->mBones[j]->mName.C_Str();
			if (jointIDs.find(current) == -1)
				jointIDs.push(XMOVE(current));
		}
	}
	return jointIDs;
}

Bone* ReadBones(aiNode* node, const List<String>& jointIDs, Bone* parent = nullptr)
{
	Bone* current = new Bone();
	current->Name = node->mName.C_Str();
	current->Bind = (mat4&)node->mTransformation;
	current->Parent = parent;
	current->ID = jointIDs.find(current->Name);
	if(current->Parent = parent) parent->Children.push(current);
	for(uint i = 0; i < node->mNumChildren; ++i)
		ReadBones(node->mChildren[i], jointIDs, current);
	return current;
}

Bone* FillBones(Bone* bone, const aiAnimation* anim, Array<mat4>& action_container, const mat4& parentInv)
{
	for (int i = 0; i < anim->mNumChannels; ++i)
	{
		if (bone->Name == anim->mChannels[i]->mNodeName.C_Str())
		{
			for (int j = 0; j < anim->mChannels[i]->mNumRotationKeys; ++j)
			{
				bone->Poses.emplace(
					anim->mChannels[i]->mRotationKeys[j].mValue.x,
					anim->mChannels[i]->mRotationKeys[j].mValue.y,
					anim->mChannels[i]->mRotationKeys[j].mValue.z,
					anim->mChannels[i]->mRotationKeys[j].mValue.w,
					anim->mChannels[i]->mPositionKeys[j].mValue.x,
					anim->mChannels[i]->mPositionKeys[j].mValue.y,
					anim->mChannels[i]->mPositionKeys[j].mValue.z,
					anim->mChannels[i]->mScalingKeys[j].mValue.x,
					anim->mChannels[i]->mScalingKeys[j].mValue.y,
					anim->mChannels[i]->mScalingKeys[j].mValue.z
				);
			}
		}
	}
	bone->XF = &action_container[bone->ID];
	bone->InvBind = inverse(bone->Bind) * parentInv;
	for (auto b : bone->Children)
		FillBones(b, anim, action_container, bone->InvBind);

	return bone;
}

namespace
{
	Bone* TrimDown(Bone* root)
	{
		auto lambda = [](Bone* const& bone) { return bone->ID == -1; };
		uint i = root->Children.findif(lambda);
		while (i != -1)
		{
			root->Children[i]->Delete();
			delete root->Children[i];
			root->Children.Remove(i);
			i = root->Children.findif(lambda);
		}
		for (auto bone : root->Children)
			TrimDown(bone);

		return root;
	}

	Bone* TrimUp(Bone* root)
	{
		if (root->Parent)
		{
			auto lambda = [](Bone* const& bone) { return bone->ID == -1; };
			uint i = root->Parent->Children.findif(lambda);
			while (i != -1)
			{
				root->Parent->Children[i]->Delete();
				delete root->Parent->Children[i];
				root->Parent->Children.Remove(i);
				i = root->Parent->Children.findif(lambda);
			}
			root->Parent->ID = -2;
			TrimUp(root->Parent);
		}
		if (root->ID != -2)
		{
			Bone* c = root->Parent;
			while (c) {
				Bone* t = c->Parent;
				delete c;
				c = t;
			}
		}
		return root;
	}

	Bone* FindRoot(Bone* root)
	{
		if (root->ID != -1)
			return TrimDown(TrimUp(root));
		for (auto b : root->Children)
		{
			Bone* re = FindRoot(b);
			if (re) return re;
		}
		return nullptr;
	}
}
struct AnimatedVertex
{
	vec3 v;
	vec3 n;
	vec2 t;
	vec4u j;
	vec4  w;
};

Array<AnimatedVertex>  Assemble(const aiScene* scene, const List<String>& names)
{
	Array<AnimatedVertex> buffer;
	int current = 0;
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		buffer.resize(buffer.size() + scene->mMeshes[i]->mNumVertices);
		for (int j = current; j < buffer.size(); ++j)
		{
			buffer[j].v = (vec3&)scene->mMeshes[i]->mVertices[j - current];
			buffer[j].n = (vec3&)scene->mMeshes[i]->mNormals[j - current];
			//if(scene->mMeshes[i]->mTextureCoords)
			//buffer[j].t = (vec2&)scene->mMeshes[i]->mTextureCoords[j - current];
		}
		current += scene->mMeshes[i]->mNumVertices;
	}
	current = 0;
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; ++j)
		{
			for (int k = 0; k < scene->mMeshes[i]->mBones[j]->mNumWeights; ++k)
			{
				auto weight = scene->mMeshes[i]->mBones[j]->mWeights[k];
				if (buffer[current + weight.mVertexId].w.x == 0.f)
				{
					buffer[current + weight.mVertexId].j.x = names.find(scene->mMeshes[i]->mBones[j]->mName.C_Str());
					buffer[current + weight.mVertexId].w.x = weight.mWeight;
				}
				else if (buffer[current + weight.mVertexId].w.y == 0.f)
				{
					buffer[current + weight.mVertexId].j.y = names.find(scene->mMeshes[i]->mBones[j]->mName.C_Str());
					buffer[current + weight.mVertexId].w.y = weight.mWeight;
				}
				else if (buffer[current + weight.mVertexId].w.z == 0.f)
				{
					buffer[current + weight.mVertexId].j.z = names.find(scene->mMeshes[i]->mBones[j]->mName.C_Str());
					buffer[current + weight.mVertexId].w.z = weight.mWeight;
				}
				else if (buffer[current + weight.mVertexId].w.w == 0.f)
				{
					buffer[current + weight.mVertexId].j.w = names.find(scene->mMeshes[i]->mBones[j]->mName.C_Str());
					buffer[current + weight.mVertexId].w.w = weight.mWeight;
				}
			}
		}
		current += scene->mMeshes[i]->mNumVertices;
	}

	return buffer;
}


AnimatedModel::AnimatedModel(const char* path)
{
	Assimp::Importer I;
	const aiScene* scene = I.ReadFile(path, 
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	auto names = GetNames(scene);
	anim.Root = FillBones(FindRoot(ReadBones(scene->mRootNode, names)), 
		scene->mAnimations[0], action_container.resize(names.size()), mat4());

	anim.ticks.resize(scene->mAnimations[0]->mChannels[0]->mNumRotationKeys);
	for (int i = 0; i < anim.ticks.size(); ++i)
		anim.ticks[i] = scene->mAnimations[0]->mChannels[0]->mRotationKeys[i].mTime;

	mesh.Buffer(Assemble(scene, names), 1);
}

void AnimatedModel::Update(float n)
{
	anim.update(n);
}

void AnimatedModel::Draw()
{
	Uniform(xform, 2);
	Uniform(action_container, 3);
	mesh.Draw();
}

void AnimatedModel::SetPos(const vec3& v)
{
	xform.col[3].xyz = v;
}

struct Vertex
{
	vec3 v;
	vec3 n;
	vec2 t;
};

Array<Vertex> Assemble(const aiScene* scene)
{
	Array<Vertex> buffer;
	for (int i = 0; i < scene->mNumMeshes; ++i)
	{
		auto mesh = scene->mMeshes[i];
		for (int j = 0; j < mesh->mNumVertices; ++j)
		{
			auto& v = (vec3&)mesh->mVertices[j];
			buffer.push({
				v,
				(vec3&)mesh->mNormals[j],
				mesh->HasTextureCoords(0) ?
				(vec2&)mesh->mTextureCoords[0][j] : 0
				});
		}
	}
	return buffer;
}

Model::Model(const char* path)
{
	Assimp::Importer I;
	const aiScene* scene = I.ReadFile(path,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	mesh.Buffer(Assemble(scene), 1);
}

void Model::Draw()
{
	Uniform(xform, 2);
	mesh.Draw();
}


void Model::Rotate(const mat4& m)
{
	xform = m * xform;
}

Array<Vertex> Calc(const vec3& min, const vec3& max)
{
	Array<Vertex> buffer(36);
	buffer[0].v  = { min.x, min.y, min.z };
	buffer[1].v  = { min.x, max.y, min.z };
	buffer[2].v  = { max.x, min.y, min.z };
	buffer[3].v  = { max.x, max.y, min.z };
	buffer[4].v  = { max.x, min.y, min.z };
	buffer[5].v  = { min.x, max.y, min.z };
	buffer[6].v  = { max.x, min.y, min.z };
	buffer[7].v  = { max.x, max.y, min.z };
	buffer[8].v  = { max.x, min.y, max.z };
	buffer[9].v  = { max.x, max.y, max.z };
	buffer[10].v = { max.x, min.y, max.z };
	buffer[11].v = { max.x, max.y, min.z };
	buffer[12].v = { max.x, min.y, max.z };
	buffer[13].v = { max.x, max.y, max.z };
	buffer[14].v = { min.x, min.y, max.z };
	buffer[15].v = { min.x, max.y, max.z };
	buffer[16].v = { min.x, min.y, max.z };
	buffer[17].v = { max.x, max.y, max.z };
	buffer[18].v = { min.x, min.y, max.z };
	buffer[19].v = { min.x, max.y, max.z };
	buffer[20].v = { min.x, min.y, min.z };
	buffer[21].v = { min.x, max.y, min.z };
	buffer[22].v = { min.x, min.y, min.z };
	buffer[23].v = { min.x, max.y, max.z };
	buffer[24].v = { min.x, max.y, min.z };
	buffer[25].v = { min.x, max.y, max.z };
	buffer[26].v = { max.x, max.y, min.z };
	buffer[27].v = { max.x, max.y, max.z };
	buffer[28].v = { max.x, max.y, min.z };
	buffer[29].v = { min.x, max.y, max.z };
	buffer[30].v = { min.x, min.y, max.z };
	buffer[31].v = { min.x, min.y, min.z };
	buffer[32].v = { max.x, min.y, max.z };
	buffer[33].v = { max.x, min.y, min.z };
	buffer[34].v = { max.x, min.y, max.z };
	buffer[35].v = { min.x, min.y, min.z };
	return buffer;
}