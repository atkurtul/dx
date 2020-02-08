#include "VertexArray.h"
#include "Clock.h"
#include "std/string.h"
#include "std/map.h"
#include "std/quaternion.h"


struct Pose
{
	quat Rot;
	vec3 Pos;
	vec3 Sca;
	Pose(float i, float j, float k, float s,
		float x, float y, float z,
		float sx, float sy, float sz) :
		Rot(i, j, k, s),
		Pos(x, y, z),
		Sca(sx, sy, sz)
	{}
};


inline static float sign(float x) {
	static int sign;
	sign = (*(int*)&x & 2147483648) + 1065353216;
	return (*(float*)&sign);
}

inline static mat4 interpolate(const Pose& p1, const Pose& p2, float blend)
{
	float t = (1.f - blend);
	quat q = p1.Rot * t;
	q += (sign(dot(p1.Rot, p2.Rot)) * blend) * p2.Rot;
	vec3 v = p1.Pos * t;
	v += p2.Pos * blend;
	return compose(q, v);
}

struct Bone
{
	mat4					InvBind;		//accumulated
	mat4					Bind;
	String					Name;
	Array<Pose>				Poses;
	Array<Bone*>			Children;
	mat4*					XF = nullptr;
	uint					ID;
	Bone*					Parent = nullptr;

	inline static uint			f1 = 0;
	inline static uint			f2 = 1;
	inline static float			blend = 0.5f;
	void SetPose(const mat4& parentxf)
	{
		if (Poses.empty())
		{
			mat4 pass = parentxf * Bind;
			*XF = pass * InvBind;
			for (auto bone : Children)
				bone->SetPose(pass);
		}
		else
		{
			mat4 pass = parentxf * interpolate(Poses[f1], Poses[f2], blend);
			*XF = pass * InvBind;
			for (auto bone : Children)
				bone->SetPose(pass);
		}
	}
	void Delete()
	{
		for (auto bone : Children)
		{
			bone->Delete();
			delete bone;
		}
	}
};


struct AnimationHandler
{
	Array<float>	ticks;
	float			time;
	Bone* Root;

	~AnimationHandler()
	{
		Root->Delete();
		delete Root;
	}

	uint index()
	{
		for (uint i = 0; i < ticks.size() - 1; i++) {
			if (time < ticks[i + 1]) {
				return i;
			}
		}
	}

	void update(float n)
	{
		time += Clock::dt * n;
		if (time >= ticks.back()) time = 0.f;
		if (time < 0.f) time = ticks.back();

		Bone::f1 = index();
		Bone::f2 = Bone::f1 + 1;
		Bone::blend = (time - ticks[Bone::f1]) / (ticks[Bone::f2] - ticks[Bone::f1]);

		Root->SetPose(mat4());
	}
};


class AnimatedModel
{
public:
	mat4 xform;
	VertexArray			mesh;
	AnimationHandler	anim;
	Array<mat4>			action_container;

	void SetPos(const vec3& v);
	AnimatedModel(const char* path);
	void Update(float n);
	void Draw();
};

class Model
{
public:
	mat4		xform;
	VertexArray	mesh;
	Model(const char* path);
	void Draw();
	void Rotate(const mat4& q);
};