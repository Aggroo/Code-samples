#pragma once
#include <iostream>
#include <math.h>

namespace CGMath
{
	class vector2D
	{
	public:
		//Konstruktor
		inline vector2D()
		{

			for (int i = 0; i < 2; i++)
			{
				vec[i] = 0;
			}
		}

		inline vector2D(int x, int y)
		{
			vec[0] = x;
			vec[1] = y;
		}

		inline vector2D(float x, float y)
		{
			vec[0] = x;
			vec[1] = y;
		}

		inline vector2D(float* arr)
		{
			vec[0] = arr[0];
			vec[1] = arr[1];
		}

		//Dekonstruktor
		inline ~vector2D()
		{

		}

		//vector + vector operators överlagring
		inline vector2D operator+(vector2D vect)
		{
			vector2D new_vec;

			for (int i = 0; i < 2; i++)
			{
				new_vec[i] = vec[i] + vect[i];
			}

			return new_vec;
		}
		inline vector2D operator-(vector2D& vect)
		{
			vector2D new_vec;

			for (int i = 0; i < 2; i++)
			{
				new_vec[i] = vec[i] - vect[i];
			}

			return new_vec;
		}

		inline vector2D operator*(const float& constant)
		{
			vector2D new_vec;

			for (int i = 0; i < 2; i++)
			{
				new_vec[i] = vec[i] * constant;
			}

			return new_vec;
		}

		inline vector2D operator/(const float& constant)
		{
			vector2D new_vec;

			for (int i = 0; i < 2; i++)
			{
				new_vec[i] = vec[i] / constant;
			}

			return new_vec;
		}

		inline float &operator[](int pos)
		{
			return vec[pos];
		}

		inline float& x()
		{
			return vec[0];
		}
		inline float& y()
		{
			return vec[1];
		}
		

		inline vector2D vecMult(float constant)
		{
			vector2D new_vec;

			for (int i = 0; i < 2; i++)
			{
				new_vec[i] = vec[i] * constant;
			}
			return new_vec;
		}

		inline float vecLength()
		{
			float length;
			float tmp = 0;

			for (int i = 0; i < 2; i++)
			{
				tmp += pow(vec[i], 2);
			}

			length = sqrt(tmp);

			return length;
		}

		inline vector2D vecNorm()
		{
			vector2D new_vec;
		
			for (int i = 0; i < 2; i++)
			{
				new_vec[i] = vec[i] / vecLength();
			}
		
			return new_vec;
		}

		inline float dotProd(vector2D vect)
		{
			float result = 0;

			for (int i = 0; i < 2; i++)
			{
				result += vec[i] * vect[i];
			}
			return result;
		}


		inline friend std::ostream& operator<<(std::ostream& stream, const vector2D& vect)
		{
			for (int y = 0; y < 2; y++)
			{
				stream << vect.vec[y] << " ";

			}
			std::cout << std::endl;
			return stream;
		}


	private:
		float vec[2];
	
	};
}

typedef CGMath::vector2D vec2;
