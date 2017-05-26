#pragma once
#include <iostream>
#include <math.h>

namespace CGMath
{
	class vector4D
	{
	public:
		inline vector4D()
		{

			for (int i = 0; i < 3; i++)
			{
				vec[i] = 0;
			}

			vec[3] = 1;
		}
		inline vector4D(int x, int y, int z)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
			vec[3] = 1;
		}
		inline vector4D(float x, float y, float z)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
			vec[3] = 1;
		}
		inline vector4D(float x, float y, float z , float w)
		{
			vec[0] = x;
			vec[1] = y;
			vec[2] = z;
			vec[3] = w;
		}
		inline ~vector4D()
		{

		}

		inline vector4D operator+(vector4D vect)
		{
			vector4D new_vec;

			for (int i = 0; i < 3; i++)
			{
				new_vec[i] = vec[i] + vect[i];
			}

			return new_vec;
		}
		inline vector4D operator-(vector4D& vect)
		{
			vector4D new_vec;

			for (int i = 0; i < 3; i++)
			{
				new_vec[i] = vec[i] - vect[i];
			}

			return new_vec;
		}

		inline vector4D operator*(vector4D& vect)
		{
			vector4D new_vec;

			for (int i = 0; i < 3; i++)
			{
				new_vec[i] = vec[i] * vect[i];
			}

			return new_vec;
		}

		inline vector4D operator*(int& constant)
		{
			vector4D new_vec;

			for (int i = 0; i < 3; i++)
			{
				new_vec[i] = vec[i] * constant;
			}

			return new_vec;
		}

		inline vector4D operator/(vector4D& vect)
		{
			vector4D new_vec;

			for (int i = 0; i < 3; i++)
			{
				new_vec[i] = vec[i] / vect[i];
			}

			return new_vec;
		}

		inline vector4D operator/(float& constant)
		{
			vector4D new_vec;

			for (int i = 0; i < 3; i++)
			{
				new_vec[i] = vec[i] / constant;
			}

			return new_vec;
		}

		inline float &operator[](int pos)
		{
			return vec[pos];
		}
		
		inline float x() const
		{
			return vec[0];
		}
		inline float y() const
		{
			return vec[1];
		}
		inline float z() const
		{
			return vec[2];
		}

		inline float w() const
		{
			return vec[3];
		}

		inline float& x()
		{
			return vec[0];
		}
		inline float& y()
		{
			return vec[1];
		}
		inline float& z()
		{
			return vec[2];
		}

		inline float& w()
		{
			return vec[3];
		}

		inline vector4D vecMult(float constant)
		{
			vector4D new_vec;

			for (int i = 0; i < 3; i++)
			{
				new_vec[i] = vec[i] * constant;
			}
			return new_vec;
		}

		inline float vecLength()
		{
			float length;
			float tmp = 0;

			for (int i = 0; i < 3; i++)
			{
				tmp += pow(vec[i], 2);
			}

			length = sqrt(tmp);

			return length;
		}

		inline vector4D vecNorm()
		{
			vector4D new_vec;

			for (int i = 0; i < 3; i++)
			{
				new_vec[i] = vec[i] / vecLength();
			}

			return new_vec;
		}

		inline float dotProd(vector4D vect)
		{
			float result = 0;

			for (int i = 0; i < 3; i++)
			{
				result += vec[i] * vect[i];
			}
			return result;
		}

		inline vector4D crossProd(vector4D vect)
		{
			vector4D new_vec;

			new_vec[0] = vec[1] * vect[2] - vec[2] * vect[1];
			new_vec[1] = vec[2] * vect[0] - vec[0] * vect[2];
			new_vec[2] = vec[0] * vect[1] - vec[1] * vect[0];
		

			return new_vec;
		}



		inline friend std::ostream& operator<<(std::ostream& stream, const vector4D& vect)
		{
			for (int y = 0; y < 3; y++)
			{
				stream << vect.vec[y] << " ";

			}
			std::cout << std::endl;
			return stream;
		}


	private:
		float vec[4];
	
	};
	
	typedef CGMath::vector4D vec4;
}

