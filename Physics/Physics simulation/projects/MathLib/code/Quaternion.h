#ifndef QUATERNIONS_H
#define QUATERNIONS_H
#include "vector3D.h"
#include "vector4D.h"


namespace CGMath
{
class Quaternion
{
public:
	inline Quaternion()
	{
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
		this->w = 1.0f;
	}
	
	inline Quaternion(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	
	inline Quaternion(float x, float y, float z)
	{
		set(x,y,z);
	}
	
	inline Quaternion(const vec3& vec)
	{
		set(vec.x(),vec.y(),vec.z());
	}
	
	
	inline ~Quaternion()
	{
		
	}
	
	//! Equality operator
	inline bool operator==(const Quaternion& other) const
	{
		return ((x == other.X()) &&
			(y == other.Y()) &&
			(z == other.Z()) &&
			(w == other.W()));
	}

	//! inequality operator
	inline bool operator!=(const Quaternion& other) const
	{
		return !(*this == other);
	}

	//= Assignment operator
	inline Quaternion& operator=(const Quaternion& other)
	{
		x = other.X();
		y = other.Y();
		z = other.Z();
		w = other.W();
		return *this;
	}
	
		//= Assignment operator
	inline Quaternion& operator=(const vec4& other)
	{
		x = other.x();
		y = other.y();
		z = other.z();
		w = other.w();
		return *this;
	}
	
	//! Add operator
	inline Quaternion operator+(const Quaternion& other) const
	{
		return Quaternion(this->x+other.X(), this->y+other.Y(), this->z+other.Z(), this->w+other.W());
	}
	
	//! Multiplication operator
	//! Be careful, unfortunately the operator order here is opposite of that in CMatrix4::operator*
	inline Quaternion operator*(const Quaternion& other) const
	{
		Quaternion tmp;

		tmp.W((other.W() * w) - (other.X() * x) - (other.Y() * y) - (other.Z() * z));
		tmp.X((other.W() * x) + (other.X() * w) + (other.Y() * z) - (other.Z() * y));
		tmp.Y((other.W() * y) + (other.Y() * w) + (other.Z() * x) - (other.X() * z));
		tmp.Z((other.W() * z) + (other.Z() * w) + (other.X() * y) - (other.Y() * x));

		return tmp;
	}

	//! Multiplication operator with scalar
	inline Quaternion operator*(float s) const
	{
		return Quaternion(s*this->x, s*this->y, s*this->z, s*this->w);
	}

	//! Multiplication operator
	inline vec3 operator*(const vec3& v) const
	{
		float x = this->x * v.x();
		float y = this->y * v.y();
		float z = this->z * v.z();
		return vec3(x,y,z);
	}

	void operator *=(const Quaternion &multiplier)
	{
		Quaternion q = *this;
		w = q.W()*multiplier.W() - q.X()*multiplier.X() -
			q.Y()*multiplier.Y() - q.Z()*multiplier.Z();
		x = q.W()*multiplier.X() + q.X()*multiplier.W() +
			q.Y()*multiplier.Z() - q.Z()*multiplier.Y();
		y = q.W()*multiplier.Y() + q.Y()*multiplier.W() +
			q.Z()*multiplier.X() - q.X()*multiplier.Z();
		z = q.W()*multiplier.Z() + q.Z()*multiplier.W() +
			q.X()*multiplier.Y() - q.Y()*multiplier.X();
	}
	
	// multiplication operator
	inline Quaternion& operator*=(float s)
	{
		this->x*=s;
		this->y*=s;
		this->z*=s;
		this->w*=s;
		return *this;
	}
	
	//! Normalizes the quaternion
	inline Quaternion& normalize()
	{
		float n = x*x + y*y + z*z + w*w;

		if (n == 1)
			return *this;

		n = 1.0f / sqrtf(n);
		return (*this *= n);
	}

	inline void AddScaleVector(const vec4& vec, float scale)
	{
		Quaternion q(vec.x()*scale, vec.y()*scale,vec.z()*scale, 0);

		q *= *this;
		this->x += q.X()*0.5f;
		this->y += q.Y()*0.5f;
		this->z += q.Z()*0.5f;
		this->w += q.W()*0.5f;
	}
	
	//! Sets new quaternion based on Euler angles (radians)
	inline Quaternion& set(float x, float y, float z)
	{
		double angle;

		angle = x * 0.5;
		const double sr = sin(angle);
		const double cr = cos(angle);

		angle = y * 0.5;
		const double sp = sin(angle);
		const double cp = cos(angle);

		angle = z * 0.5;
		const double sy = sin(angle);
		const double cy = cos(angle);

		const double cpcy = cp * cy;
		const double spcy = sp * cy;
		const double cpsy = cp * sy;
		const double spsy = sp * sy;

		this->x = (float)(sr * cpcy - cr * spsy);
		this->y = (float)(cr * spcy + sr * cpsy);
		this->z = (float)(cr * cpsy - sr * spcy);
		this->w = (float)(cr * cpcy + sr * spsy);

		return normalize();
	}

	//! Sets new quaternion based on Euler angles (radians)
	inline Quaternion& set(const vec3& vec)
	{
		return set(vec.x(), vec.y(), vec.z());
	}

	//! Sets new quaternion from other quaternion
	inline Quaternion& set(const Quaternion& quat)
	{
		return (*this=quat);
	}
	
	//Quaternion slerp, float t is the interval
	inline static Quaternion QuatSlerp(Quaternion* from, Quaternion * to, float t)
	{
		float           to1[4];
		double        omega, cosom, sinom, scale0, scale1;
		// calc cosine
		cosom = from->x * to->x + from->y * to->y + from->z * to->z + from->w * to->w;
		
		// adjust signs (if necessary)
		if ( cosom <0.0 ){ cosom = -cosom; to1[0] = - to->x;
			to1[1] = - to->y;
			to1[2] = - to->z;
			to1[3] = - to->w;
			} else  {
			to1[0] = to->x;
			to1[1] = to->y;
			to1[2] = to->z;
			to1[3] = to->w;
		}
		// calculate coefficients
		if ( (1.0 - cosom) > 0.3f ) 
		{
			// standard case (slerp)
			omega = acos(cosom);
			sinom = sin(omega);
			scale0 = sin((1.0 - t) * omega) / sinom;
			scale1 = sin(t * omega) / sinom;
		} 
		else 
		{        
			// "from" and "to" Quaternion are very close
			//  ... so we can do a linear interpolation
			scale0 = 1.0 - t;
			scale1 = t;
		}
		// calculate final values
		Quaternion res;
		res.x = scale0 * from->x + scale1 * to1[0];
		res.y = scale0 * from->y + scale1 * to1[1];
		res.z = scale0 * from->z + scale1 * to1[2];
		res.w = scale0 * from->w + scale1 * to1[3];
		return res;
	}
	
	inline static Quaternion AxisRotation(float angle, vec3 vec)
	{
		Quaternion q;
		float a = angle*0.5f;
		q.x = cos(a);
		q.y = sin(a)*cos(vec.x());
		q.z = sin(a)*cos(vec.y());
		q.w = sin(a)*cos(vec.z());
		return q;
	}
	
	inline static vector4D QuaternionToVec4(Quaternion quat)
	{
		vector4D vec;
		vec[0] = quat.x;
		vec[1] = quat.y;
		vec[2] = quat.z;
		
		return vec;
	}
	
	inline float X() const
	{
		return this->x;
	}
	
	inline float Y() const
	{
		return this->y;
	}
	
	inline float Z() const
	{
		return this->z;
	}
	
	inline float W() const
	{
		return this->w;
	}
	
	inline void X(float val)
	{
		this->x = val;
	}
	
	inline void Y(float val)
	{
		this->y = val;
	}
	
	inline void Z(float val)
	{
		this->z = val;
	}
	
	inline void W(float val)
	{
		this->w = val;
	}
	
	
	
private:
	float x, y, z, w;
	
};	
}


#endif // QUATERNIONS_H
