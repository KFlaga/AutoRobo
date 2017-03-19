/*
 * RoboMath.h
 *
 *  Created on: Mar 2, 2017
 *      Author: Kamil
 */

#ifndef ROBOMATH_H_
#define ROBOMATH_H_

#include "math.h"

#define roboMax(a,b) (a) > (b) ? (a) : (b)
#define roboMin(a,b) (a) > (b) ? (b) : (a)

typedef struct _Vector3
{
	float x;
	float y;
	float z;
} Vector3;

extern const Vector3 Vector3_Zero;
extern const Vector3 Vector3_UnitX;
extern const Vector3 Vector3_UnitY;
extern const Vector3 Vector3_UnitZ;
extern const Vector3 Vector3_NegUnitX;
extern const Vector3 Vector3_NegUnitY;
extern const Vector3 Vector3_NegUnitZ;
extern const Vector3 Vector3_One;

typedef struct _Vector2
{
	float x;
	float y;
} Vector2;

extern const Vector2 Vector2_Zero;
extern const Vector2 Vector2_UnitX;
extern const Vector2 Vector2_UnitY;
extern const Vector2 Vector2_NegUnitX;
extern const Vector2 Vector2_NegUnitY;
extern const Vector2 Vector2_One;

typedef struct _Quaternion
{
	float w;
	float x;
	float y;
	float z;
} Quaternion;


// Returns Vector3 : point-wise added 2 Vector3 'a' and 'b'
#define Vector3_Add(a, b) (Vector3){a.x + b.x,	a.y + b.y, a.z + b.z}
// Returns Vector3 : point-wise subtracted 2 Vector3 'a' and 'b'
#define Vector3_Sub(a, b) (Vector3){a.x - b.x,	a.y - b.y, a.z - b.z}
// Returns Vector3 : point-wise multiplied 2 Vector3 'a' and 'b'
#define Vector3_Mult(a, b) (Vector3){a.x * b.x,	a.y * b.y, a.z * b.z}
// Returns Vector3 : point-wise divided 2 Vector3 'a' and 'b'
#define Vector3_Div(a, b) (Vector3){a.x / b.x,	a.y / b.y, a.z / b.z}

// Returns Vector3 : point-wise added Vector3 'vec' and scalar 's'
#define Vector3_AddScalar(vec, s) (Vector3){vec.x + s, vec.y + s, vec.z + s}
// Returns Vector3 : point-wise subtracted Vector3 'vec' and scalar 's'
#define Vector3_SubScalar(vec, s) (Vector3){vec.x - s, vec.y - s, vec.z - s}
// Returns Vector3 : point-wise subtracted scalar 's' and Vector 'vec'
#define Vector3_ScalarSub(s, vec) (Vector3){s-vec.x, s-vec.y, s-vec.z}
// Returns Vector3 : point-wise multiplied Vector3 'vec' and scalar 's'
#define Vector3_MultScalar(vec, s) (Vector3){vec.x * s, vec.y * s, vec.z * s}
// Returns Vector3 : point-wise divided Vector3 'vec' and scalar 's'
#define Vector3_DivScalar(vec, s) (Vector3){vec.x / s, vec.y / s, vec.z / s}
// Returns Vector3 : point-wise divided scalar 's' and Vector 'vec'
#define Vector3_ScalarDiv(s, vec) (Vector3){s/vec.x, s/vec.y, s/vec.z}

// Returns sum of elements of Vector3
#define Vector3_Sum(a) (a.x + a.y + a.z)
// Returns dot product of 2 Vector3 'a' and 'b'
#define Vector3_Dot(a, b) (a.x * b.x + a.y * a.y + a.z * a.z)
// Returns cross product of 2 Vector3 'a' and 'b'
#define Vector3_Cross(a, b) (Vector3) {\
		a.y * b.z - a.z * b.y, \
		a.z * b.x - a.x * b.z, \
		a.x * b.y - a.y * b.x \
	}
// Creates direction vector. Angle is counted from OY in left-hand frame
#define Vector3_FromAngle_YX(angle) (Vector3){(float)(-sin(angle)), (float)(cos(angle)), 0.0f }
// Creates direction vector. Angle is counted from OX in left-hand frame
#define Vector3_FromAngle_XY(angle) (Vector3){(float)(cos(angle)), (float)(sin(angle)), 0.0f }

#define Vector3_FromXYZ(x, y, z) (Vector3){x,y,z}
// Returns squared length of Vector3
#define Vector3_LengthSquared(vec) Vector3_Dot(vec,vec)
// Returns length of Vector3
#define Vector3_Length(vec) (float)sqrt(Vector3_LengthSquared(vec))
// Returns squared distance from Vector3 'from' to Vector3 'to'
#define Vector3_DistanceSquared(from, to) (to.x-from.x)*(to.x-from.x)+\
	(to.y-from.y)*(to.y-from.y)+\
	(to.z-from.z)*(to.z-from.z)
// Returns distance from Vector3 'from' to Vector3 'to'
#define Vector3_Distance(from,to) (float)sqrt(Vector3_DistanceSquared(from,to)
// Returns normalized (len=1) Vector3
#define Vector3_Normalise(vec) Vector3_DivScalar(vec, Vector3_Length(vec))
// Returns Vector3 : vector 'vec' rotated around Z axis
static inline Vector3 Vector3_Rotate_XY(Vector3 vec, float angle)
{
	// vec = r*[cos(a),sin(a),z] : r is norm, a is angle of vec from X axis
	// vec_rotated = r*[cos(a)cos(da) - sin(a)*sin(da), sin(a)cos(da) + cos(a)sin(da),z]
	// vec_rotated = [vec.x*cos(da) - vec.y*sin(da), vec.y*cos(da) + vec.x*sin(da),z]
	// If angle is counted from Y axis then vec = r*[-sin(a),cos(a),z]
	// but final formula stays the same
	float cosda = (float)cos(angle);
	float sinda = (float)sin(angle);
	return (Vector3){
		vec.x * cosda - vec.y * sinda,
		vec.y * cosda + vec.x * sinda,
		vec.z
	};
}

// Returns Vector2 : point-wise added 2 Vector2 'a' and 'b'
#define Vector2_Add(a, b) (Vector2){a.x + b.x,	a.y + b.y}
// Returns Vector2 : point-wise subtracted 2 Vector2 'a' and 'b'
#define Vector2_Sub(a, b) (Vector2){a.x - b.x,	a.y - b.y}
// Returns Vector2 : point-wise multiplied 2 Vector2 'a' and 'b'
#define Vector2_Mult(a, b) (Vector2){a.x * b.x,	a.y * b.y}
// Returns Vector2 : point-wise divided 2 Vector2 'a' and 'b'
#define Vector2_Div(a, b) (Vector2){a.x / b.x,	a.y / b.y}

// Returns Vector2 : point-wise added Vector2 'vec' and scalar 's'
#define Vector2_AddScalar(vec, s) (Vector2){vec.x + s, vec.y + s}
// Returns Vector2 : point-wise subtracted Vector2 'vec' and scalar 's'
#define Vector2_SubScalar(vec, s) (Vector2){vec.x - s, vec.y - s}
// Returns Vector2 : point-wise subtracted scalar 's' and Vector 'vec'
#define Vector2_ScalarSub(s, vec) (Vector2){s-vec.x, s-vec.y}
// Returns Vector2 : point-wise multiplied Vector2 'vec' and scalar 's'
#define Vector2_MultScalar(vec, s) (Vector2){vec.x * s, vec.y * s}
// Returns Vector2 : point-wise divided Vector2 'vec' and scalar 's'
#define Vector2_DivScalar(vec, s) (Vector2){vec.x / s, vec.y / s}
// Returns Vector2 : point-wise divided scalar 's' and Vector 'vec'
#define Vector2_ScalarDiv(s, vec) (Vector2){s/vec.x, s/vec.y}

// Returns sum of elements of Vector2
#define Vector2_Sum(a) (a.x + a.y)
// Returns dot product of 2 Vector2 'a' and 'b'
#define Vector2_Dot(a, b) (a.x * b.x + a.y * a.y)
// Returns cross product of 2 Vector2 'a' and 'b'
#define Vector2_Cross(a, b) (a.x * b.y - a.y * b.x)

// Creates direction vector. Angle is counted from OY in left-hand frame
#define Vector2_FromAngle_YX(angle) (Vector2){(float)(-sin(angle)), (float)(cos(angle))}
// Creates direction vector. Angle is counted from OX in left-hand frame
#define Vector2_FromAngle_XY(angle) (Vector2){(float)(cos(angle)), (float)(sin(angle))}

#define Vector2_FromXY(x, y) (Vector2){x,y}
// Returns squared length of Vector2
#define Vector2_LengthSquared(vec) Vector2_Dot(vec,vec)
// Returns length of Vector2
#define Vector2_Length(vec) ((float)sqrt(Vector2_LengthSquared(vec)))
// Returns squared distance from Vector2 'from' to Vector2 'to'
#define Vector2_DistanceSquared(from, to) (to.x-from.x)*(to.x-from.x)+(to.y-from.y)*(to.y-from.y)
// Returns distance from Vector2 'from' to Vector2 'to'
#define Vector2_Distance(from,to) ((float)sqrt(Vector2_DistanceSquared(from,to))
// Returns normalized (len=1) Vector2
static inline Vector2 Vector2_Normalise(Vector2 vec)
{
	float l = Vector2_Length(vec);
	return Vector2_DivScalar(vec, l);
}
// Returns Vector2 : vector 'vec' rotated around Z axis
static inline Vector2 Vector2_Rotate_XY(Vector2 vec, float angle)
{
	// vec = r*[cos(a),sin(a),z] : r is norm, a is angle of vec from X axis
	// vec_rotated = r*[cos(a)cos(da) - sin(a)*sin(da), sin(a)cos(da) + cos(a)sin(da),z]
	// vec_rotated = [vec.x*cos(da) - vec.y*sin(da), vec.y*cos(da) + vec.x*sin(da),z]
	// If angle is counted from Y axis then vec = r*[-sin(a),cos(a),z]
	// but final formula stays the same
	float cosda = (float)cos(angle);
	float sinda = (float)sin(angle);
	return (Vector2){
		vec.x * cosda - vec.y * sinda,
		vec.y * cosda + vec.x * sinda
	};
}
// Returns angle in radiand between 2 vectors in range (-pi,pi)
static inline float Vector2_AngleBetween(Vector2 vec1, Vector2 vec2)
{
	// Angle is simple acos(v1*v2/|v1||v2|), but it is in (0,pi)
	// To determine angle sign we need:
	// dot(v1,v2) = |v1||v2|cosa, cross(v1,v2) = |v1||v2|sina,
	// tana = sina/cosa = cross/dot
	// ang = atan2(cross, dot)
	float dot = Vector2_Dot(vec1, vec2);
	float cross = Vector2_Cross(vec1, vec2);
	return atan2f(cross, dot);
}
#endif /* ROBOMATH_H_ */
