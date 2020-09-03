#include <stdio.h>
#include <math.h>


typedef struct vec4 {
	float x;
	float y;
	float z;
	float w;
} vec4;

typedef struct vec3 {
	float x;
	float y;
	float z;
} vec3;

typedef struct mat4 {
	union  {
		vec4 columns[4];
		float elements[16];
	}
} mat4;

mat4 mat4_translate_matrix(float x, float y,float z)
{
	mat4 matrix;
	float* elements = matrix.elements;

	memset(elements, 0, sizeof(float)*16);
	
	// Diagonal
	elements[0 + 0*4] = 1;
	elements[1 + 1*4] = 1;
	elements[2 + 2*4] = 1;
	elements[3 + 3*4] = 1;

	// Last column
	elements[3 + 0*4] = x;
	elements[3 + 1*4] = y;
	elements[3 + 2*4] = z;
}

vec3 vec3_normalize(vec3 vector)
{
	// Length
	float length_squared = vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;

	float length = sqrt(length_squared);	

	vector.x /= length;
	vector.y /= length;
	vector.z /= length;

	return vector;
}

mat4 mat4_rotate_matrix(float radians, vec3 axis)
{

	vec3 n_axis = vec3_normalize(axis);

	mat4 matrix;
	float* elements = matrix.elements;


	memset(elements, 0, sizeof(float)*16);
	// Bottom Row
	elements[0 + 3*4] = 0;
	elements[1 + 3*4] = 0;
	elements[2 + 3*4] = 0;
	elements[3 + 3*4] = 1;
	
	// Rightmost column
	elements[3 + 0*4] = 0;
	elements[3 + 1*4] = 0;
	elements[3 + 2*4] = 0;

	float c = sin(radians + M_PI/2);
	float s = sin(radians);

	float omc = 1 - c;

	elements[0 + 0*4] = omc*n_axis.x*n_axis.x + c;
	elements[1 + 0*4] = omc*n_axis.x*n_axis.y - s*n_axis.z;
	elements[2 + 0*4] = omc*n_axis.x*n_axis.z + s*n_axis.y;

	elements[0 + 1*4] = omc*n_axis.x*n_axis.y + s*n_axis.z;
	elements[1 + 1*4] = omc*n_axis.y*n_axis.y + c;
	elements[2 + 1*4] = omc*n_axis.y*n_axis.z - s*n_axis.x;

	elements[0 + 2*4] = omc*n_axis.x*n_axis.z - s*n_axis.y;
	elements[1 + 2*4] = omc*n_axis.y*n_axis.z + s*n_axis.x;
	elements[2 + 2*4] = omc*n_axis.z*n_axis.z + c ;
}

mat4 composite_transform_matrix(float tx, float ty, float tz, float radians, vec3 axis)
{
	vec3 n_axis = vec3_normalize(axis);

	mat4 matrix;
	float* elements = matrix.elements;


	memset(elements, 0, sizeof(float)*16);
	// Bottom Row
	elements[0 + 3*4] = 0;
	elements[1 + 3*4] = 0;
	elements[2 + 3*4] = 0;
	elements[3 + 3*4] = 1;
	
	// Rightmost column
	elements[3 + 0*4] = tx;
	elements[3 + 1*4] = ty;
	elements[3 + 2*4] = tz;

	float c = sin(radians + M_PI/2);
	float s = sin(radians);

	float omc = 1 - c;

	elements[0 + 0*4] = omc*n_axis.x*n_axis.x + c;
	elements[1 + 0*4] = omc*n_axis.x*n_axis.y - s*n_axis.z;
	elements[2 + 0*4] = omc*n_axis.x*n_axis.z + s*n_axis.y;

	elements[0 + 1*4] = omc*n_axis.x*n_axis.y + s*n_axis.z;
	elements[1 + 1*4] = omc*n_axis.y*n_axis.y + c;
	elements[2 + 1*4] = omc*n_axis.y*n_axis.z - s*n_axis.x;

	elements[0 + 2*4] = omc*n_axis.x*n_axis.z - s*n_axis.y;
	elements[1 + 2*4] = omc*n_axis.y*n_axis.z + s*n_axis.x;
	elements[2 + 2*4] = omc*n_axis.z*n_axis.z + c ;
}
