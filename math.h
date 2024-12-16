#pragma once
//���滻
#define SQ(x) (x) * (x)
#define _USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#include<string>
#include"GamesEngineeringBase .h"


//�����Բ�ֵ���� lerp 
template<typename T>
static T lerp(const T a, const T b, float t)
{
	return a * (1.0f - t) + (b * t);
}

class vec3 {
public:
	union {
		float v[3];
		struct { float x, y, z; };
	};
	vec3() {
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}
	vec3(float _x,float _y,float _z) {
		v[0] = _x;
		v[1] = _y;
		v[2] = _z;
	}

	bool operator!=(const vec3& pVec) const {
		return v[0] != pVec.v[0] || v[1] != pVec.v[1] || v[2] != pVec.v[2];
	}

	vec3 operator+(const vec3& pVec)const
	{
		return vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
	}
	vec3 operator-(const vec3& pvec)const {
		return  vec3(v[0] - pvec.v[0], v[1] - pvec.v[1], v[2] - pvec.v[2]);
	}
	vec3 operator*(const vec3& pvec)const {
		return  vec3(v[0] *pvec.v[0], v[1] * pvec.v[1], v[2] * pvec.v[2]);
	}
	vec3 operator/(const vec3& pvec)const {
		return  vec3(v[0] / pvec.v[0], v[1] / pvec.v[1], v[2] / pvec.v[2]);
	}
	
	vec3& operator+=(const vec3& pVec)
	{
		v[0] += pVec.v[0];
		v[1] += pVec.v[1];
		v[2] += pVec.v[2];
		return *this;
	}
	vec3& operator-=(const vec3& pvec) {
		x -= pvec.x;
		y -= pvec.y;
		z -= pvec.z;
		return *this;
	}
	vec3& operator*=(const vec3& pVec)
	{
		v[0] *= pVec.v[0];
		v[1] *= pVec.v[1];
		v[2] *= pVec.v[2];
		return *this;
	}
	vec3& operator/=(const vec3& pvec) {
		x /= pvec.x;
		y /= pvec.y;
		z /= pvec.z;
		return *this;
	}

	vec3 operator+(const float v)const {
		return vec3(x + v, y + v, z + v);
	}
	vec3 operator-(const float v)const {
		return vec3(x - v, y - v, z - v);
	}
	vec3 operator*(const float v)const {
		return vec3(x * v, y * v, z * v);
	}
	vec3 operator/(const float v)const {
		return vec3(x / v, y / v, z / v);
	}

	vec3& operator+=(const float v) {
		x += v;
		y += v;
		z += v;
		return *this;
	}
	vec3& operator-=(const float v) {
		x -= v;
		y -= v;
		z -= v;
		return *this;
	}
	vec3& operator*=(const float v) {
		x *= v;
		y *= v;
		z *= v;
		return *this;
	}
	vec3& operator/=(const float v) {
		x /= v;
		y /= v;
		z /= v;
		return *this;
	}

	vec3 operator-() const
	{
		return vec3(-v[0], -v[1], -v[2]);
	}

	//��ȡ����
	float Length() {
		return sqrtf(LengthSQ());
	}
	//��ȡ���ȵ�ƽ��
	float LengthSQ() {
		return SQ(x) + SQ(y) + SQ(z);
	}
	//����һ����׼����ĵ�λ����
	vec3 normalize(void)
	{
		float len = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		return vec3(x * len, y * len, z * len);
	}
	//����������׼�����������ĳ���
	float normalize_GetLength()
	{
		float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		float len = 1.0f / length;
		v[0] *= len; v[1] *= len; v[2] *= len;
		return length;
	}
	//���������ĵ��
	float Dot(const vec3& pVec) const
	{
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
	}
	//���ص��Ǵ���������˴�����
	vec3 Cross(const vec3& v1)
	{
		return vec3(v1.v[1] * v[2] - v1.v[2] * v[1],
			v1.v[2] * v[0] - v1.v[0] * v[2],
			v1.v[0] * v[1] - v1.v[1] * v[0]);
	}
	//���ر�������xyz�����Ǹ�
	float Max() const
	{
		return max(x, max(y, z));
	}
	//���ر�������xyz��С���Ǹ�
	float Min() const
	{
		return min(x, min(y, z));
	}
};

class Matrix {
public:
	union
	{
		float a[4][4];
		float m[16];
	};
	//���캯����Ĭ��Ϊ��λ����
	Matrix() {
		identity();
	}
	Matrix(float *othermatrix) {
		memcpy(&m, othermatrix, 16 * sizeof(float));
	}
	Matrix(float n1, float n2, float n3, float n4, float n5, float n6, float n7, float n8, float n9, float n10, float n11, float n12, float n13, float n14, float n15, float n16) {
		m[0] = n1;
		m[1] = n2;
		m[2] = n3;
		m[3] = n4;
		m[4] = n5;
		m[5] = n6;
		m[6] = n7;
		m[7] = n8;
		m[8] = n9;
		m[9] = n10;
		m[10] = n11;
		m[11] = n12;
		m[12] = n13;
		m[13] = n14;
		m[14] = n15;
		m[15] = n16;
	}
	//ת��Ϊ��λ����
	void identity() {
		memset(m, 0, 16 * sizeof(m[0]));
		m[0] = 1;
		m[5] = 1;
		m[10] = 1;
		m[15] = 1;
	}
	//��һ������ĵ�����Ա�����ı任�����������㡣Ҳ�����ñ������������ٰѽ����ص�
	vec3 mulPoint( vec3& v)  {
		// ����ά����չΪ��ά������� (x, y, z, 1)
		float x = v.x, y = v.y, z = v.z;

		// ʹ�þ�����б任
		float w = m[12] * x + m[13] * y + m[14] * z + m[15]; // ����w����
		if (w == 0.0f) {
			return vec3(a[0][3],a[1][3],a[2][3]); // �������ѡ�������Ĵ�����
		}

		// ���ر任�����ά����
		vec3 result;
		result.x = (m[0] * x + m[1] * y + m[2] * z + m[3]) / w;
		result.y = (m[4] * x + m[5] * y + m[6] * z + m[7]) / w;
		result.z = (m[8] * x + m[9] * y + m[10] * z + m[11]) / w;

		return result;
	}

	//����һ��͸��ͶӰ����
	Matrix projection(float aspect,float fov,float znear,float zfar) {
		identity();
		float _tan = (fov / 2) * M_PI / 180.f;
		_tan = tan(_tan);
		Matrix result;
		result.m[0] = 1 / (aspect * _tan);
		result.m[5] = 1 / _tan;
		result.m[10] = (-zfar) / (zfar - znear);
		result.m[11] = (-zfar * znear) / (zfar - znear);
		result.m[14] = -1;
		result.m[15] = 0;
		return result;
	}
	Matrix orthographic(float left, float right, float bottom, float top, float znear, float zfar) {
		identity();
		Matrix result;
		result.m[0] = 2.0f / (right - left);
		result.m[1] = 0.0f;
		result.m[2] = 0.0f;
		result.m[3] = 0.0f;
		result.m[4] = 0.0f;
		result.m[5] = 2.0f / (top - bottom);
		result.m[6] = 0.0f;
		result.m[7] = 0.0f;
		result.m[8] = 0.0f;
		result.m[9] = 0.0f;
		result.m[10] = -1.0f / (zfar - znear); 
		result.m[11] = -znear / (zfar - znear);
		result.m[12] = -(right + left) / (right - left);
		result.m[13] = -(top + bottom) / (top - bottom);
		result.m[14] = 0.0f; 
		result.m[15] = 1.0f;
		return result;
	}
	//��ͼ�任 lookat from ��to��������  up��һ������
	Matrix lookat(vec3 from,vec3 to,vec3 up) {
		identity();
		vec3 dir = (from - to).normalize();//��ǰ����
		vec3 right = up.Cross(dir).normalize(); //��������
		vec3 upPrime = dir.Cross(right);
		Matrix result;

		result.m[0] = right.x;
		result.m[1] = right.y;
		result.m[2] = right.z;

		result.m[4] = upPrime.x;
		result.m[5] = upPrime.y;
		result.m[6] = upPrime.z;

		result.m[8] = dir.x;
		result.m[9] = dir.y;
		result.m[10] = dir.z;

		result.m[3] = -from.Dot(right);
		result.m[7] = -from.Dot(upPrime);
		result.m[11] = -from.Dot(dir);

		result.m[12] = 0.f;
		result.m[13] = 0.f;
		result.m[14] = 0.f;
		result.m[15] = 1.f;

		return result;
	}

    Matrix fromAxes(const vec3& right, const vec3& up, const vec3& forward) {
		Matrix result;
		result.m[0] = right.x;    result.m[1] = right.y;    result.m[2] = right.z;    result.m[3] = 0.0f;
		result.m[4] = up.x;       result.m[5] = up.y;       result.m[6] = up.z;       result.m[7] = 0.0f;
		result.m[8] = forward.x;  result.m[9] = forward.y;  result.m[10] = forward.z;  result.m[11] = 0.0f;
		result.m[12] = 0.0f;       result.m[13] = 0.0f;       result.m[14] = 0.0f;       result.m[15] = 1.0f;
		return result;
	}


	//����x����ת��������ǽǶ�
	vec3 rotationX( vec3& v, float degree) {
		float _cos=degree* M_PI/180.f ;
		float _sin = degree * M_PI / 180.f;
		_cos = cos(_cos);
		_sin = sin(_sin);
		identity();
		m[5] = _cos;
		m[6] = -_sin;
		m[9] = _sin;
		m[10] = _cos;
		return mulPoint(v);
	}
	//����y����ת��������ǽǶ�
	vec3 rotationY( vec3& v, float degree) {
		float _cos = degree * M_PI / 180.f;
		float _sin = degree * M_PI / 180.f;
		_cos = cos(_cos);
		_sin = sin(_sin);
		identity();
		m[0] = _cos;
		m[2] = _sin;
		m[8] = -_sin;
		m[10] = _cos;
		return mulPoint(v);
	}
	//����z����ת��������ǽǶ�
	vec3 rotationZ( vec3& v, float degree) {
		float _cos = degree * M_PI / 180.f;
		float _sin = degree * M_PI / 180.f;
		_cos = cos(_cos);
		_sin = sin(_sin);
		identity();
		m[0] = _cos;
		m[1] = -_sin;
		m[4] = _sin;
		m[5] = _cos;
		return mulPoint(v);
	}
	//����������v��ת��������ǽǶ�,������vvv��Ϊ��
	vec3 rotationV( vec3& v, float degree,vec3 vvv) {
		float _cos = degree * M_PI / 180.f;
		float _sin = degree * M_PI / 180.f;
		_cos = cos(_cos);
		_sin = sin(_sin);
		vvv=vvv.normalize();
		float x = vvv.x;
		float y = vvv.y;
		float z = vvv.z;
		identity();
		m[0] = _cos + x * x * (1 - _cos);
		m[1] = x * y * (1 - _cos) - z * _sin;
		m[2] = x * z * (1 - _cos) + y * _sin;
		m[4] = y * x * (1 - _cos) + z * _sin;
		m[5] = _cos + y * y * (1 - _cos);
		m[6]= y * z * (1 - _cos) - x * _sin;
		m[8] = x * z * (1 - _cos) - y * _sin;
		m[9] = y * z * (1 - _cos) + x * _sin;
		m[10] = _cos + z * z * (1 - _cos);
		return mulPoint(v);
	}
	//����������v��ת��������ǻ���,������vvv��Ϊ��,���ؾ���
	Matrix rotationV( float degree, vec3 vvv) {
		float _cos = cos(degree);
		float _sin = sin(degree);
		vvv = vvv.normalize();
		float x = vvv.x;
		float y = vvv.y;
		float z = vvv.z;
		identity();
		m[0] = _cos + x * x * (1 - _cos);
		m[1] = x * y * (1 - _cos) - z * _sin;
		m[2] = x * z * (1 - _cos) + y * _sin;
		m[4] = y * x * (1 - _cos) + z * _sin;
		m[5] = _cos + y * y * (1 - _cos);
		m[6] = y * z * (1 - _cos) - x * _sin;
		m[8] = x * z * (1 - _cos) - y * _sin;
		m[9] = y * z * (1 - _cos) + x * _sin;
		m[10] = _cos + z * z * (1 - _cos);
		return *this;
	}
	//ƽ��
	static Matrix translation(vec3 v) {
		Matrix mat;
		mat.identity();
		mat.a[0][3] = v.x;
		mat.a[1][3] = v.y;
		mat.a[2][3] = v.z;
		return mat;
	}
	//����
	 Matrix scaling(const vec3 v) {
		identity();
		m[0] = v.x;
		m[5] = v.y;
		m[10] = v.z;
		return *this;
	}

	//����˷�
	Matrix mul(const Matrix& matrix) const
	{
		Matrix ret;
		ret.m[0] = m[0] * matrix.m[0] + m[4] * matrix.m[1] + m[8] * matrix.m[2] + m[12] * matrix.m[3];
		ret.m[1] = m[1] * matrix.m[0] + m[5] * matrix.m[1] + m[9] * matrix.m[2] + m[13] * matrix.m[3];
		ret.m[2] = m[2] * matrix.m[0] + m[6] * matrix.m[1] + m[10] * matrix.m[2] + m[14] * matrix.m[3];
		ret.m[3] = m[3] * matrix.m[0] + m[7] * matrix.m[1] + m[11] * matrix.m[2] + m[15] * matrix.m[3];
		ret.m[4] = m[0] * matrix.m[4] + m[4] * matrix.m[5] + m[8] * matrix.m[6] + m[12] * matrix.m[7];
		ret.m[5] = m[1] * matrix.m[4] + m[5] * matrix.m[5] + m[9] * matrix.m[6] + m[13] * matrix.m[7];
		ret.m[6] = m[2] * matrix.m[4] + m[6] * matrix.m[5] + m[10] * matrix.m[6] + m[14] * matrix.m[7];
		ret.m[7] = m[3] * matrix.m[4] + m[7] * matrix.m[5] + m[11] * matrix.m[6] + m[15] * matrix.m[7];
		ret.m[8] = m[0] * matrix.m[8] + m[4] * matrix.m[9] + m[8] * matrix.m[10] + m[12] * matrix.m[11];
		ret.m[9] = m[1] * matrix.m[8] + m[5] * matrix.m[9] + m[9] * matrix.m[10] + m[13] * matrix.m[11];
		ret.m[10] = m[2] * matrix.m[8] + m[6] * matrix.m[9] + m[10] * matrix.m[10] + m[14] * matrix.m[11];
		ret.m[11] = m[3] * matrix.m[8] + m[7] * matrix.m[9] + m[11] * matrix.m[10] + m[15] * matrix.m[11];
		ret.m[12] = m[0] * matrix.m[12] + m[4] * matrix.m[13] + m[8] * matrix.m[14] + m[12] * matrix.m[15];
		ret.m[13] = m[1] * matrix.m[12] + m[5] * matrix.m[13] + m[9] * matrix.m[14] + m[13] * matrix.m[15];
		ret.m[14] = m[2] * matrix.m[12] + m[6] * matrix.m[13] + m[10] * matrix.m[14] + m[14] * matrix.m[15];
		ret.m[15] = m[3] * matrix.m[12] + m[7] * matrix.m[13] + m[11] * matrix.m[14] + m[15] * matrix.m[15];
		return ret;
	}
	Matrix operator*(const Matrix& matrix)
	{
		return mul(matrix);
	}
	//���з�ת
	Matrix Flip() {
		Matrix temp;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				temp.a[i][j] = a[j][i];
			}
		}
		return temp;
	}
	//����
	Matrix invert()
	{
		Matrix inv;
		inv.m[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv.m[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv.m[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv.m[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv.m[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv.m[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv.m[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv.m[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv.m[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv.m[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv.m[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv.m[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv.m[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv.m[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv.m[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv.m[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		float det = m[0] * inv.m[0] + m[1] * inv.m[4] + m[2] * inv.m[8] + m[3] * inv.m[12];
		if (det == 0) {
			// Handle this case
		}
		det = 1.0 / det;
		for (int i = 0; i < 16; i++) {
			inv.m[i] = inv.m[i] * det;
		}
		return inv;
	}

};

class Colour {
public:
	union {
		float v[3];
		struct { float r, g, b; };
	};
	Colour() {
		v[0] = 0;
		v[1] = 0;
		v[2] = 0;
	}
	Colour(float _a,float _b,float _c) {
		v[0] = _a;
		v[1] = _b;
		v[2] = _c;
	}
};


class Quaternion {
public:
		union {
			struct {
				float a;
				float b;
				float c;
				float d;
			};
			float q[4];
		};

	// Ĭ�Ϲ��캯��
	Quaternion() : a(1), b(0), c(0), d(0) {}

	// ���������캯��
	Quaternion(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d) {}

	// ��Ԫ���ӷ�
	Quaternion operator+(const Quaternion& q) const {
		return Quaternion(a + q.a, b + q.b, c + q.c, d + q.d);
	}

	// ��Ԫ������
	Quaternion operator-(const Quaternion& q) const {
		return Quaternion(a - q.a, b - q.b, c - q.c, d - q.d);
	}

	// ��Ԫ���˷�
	Quaternion operator*(const Quaternion& q) const {
		return Quaternion(
			a * q.a - b * q.b - c * q.c - d * q.d,
			a * q.b + b * q.a + c * q.d - d * q.c,
			a * q.c - b * q.d + c * q.a + d * q.b,
			a * q.d + b * q.c - c * q.b + d * q.a
		);
	}

	// �����˷�
	Quaternion operator*(float scalar) const {
		return Quaternion(a * scalar, b * scalar, c * scalar, d * scalar);
	}

	// ���
	static float dot(const Quaternion& q1, const Quaternion& q2) {
		return q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
	}

	// ��һ��
	void normalize() {
		float magnitude = std::sqrt(a * a + b * b + c * c + d * d);
		if (magnitude > 0.0f) {
			a /= magnitude;
			b /= magnitude;
			c /= magnitude;
			d /= magnitude;
		}
	}

	// ���Բ�ֵ (LERP)
	static Quaternion lerp(const Quaternion& q1, const Quaternion& q2, float t) {
		return (q1 * (1 - t) + q2 * t).normalized();
	}

	// ���β�ֵ (SLERP)
	static Quaternion slerp(const Quaternion& q1, const Quaternion& q2, float t) {
		float dot = Quaternion::dot(q1, q2);
		if (dot < 0.0f) {
			// ��תһ����Ԫ����ѡ��϶�·��
			return slerp(q1, Quaternion(-q2.a, -q2.b, -q2.c, -q2.d), t);
		}
		dot = std::fmax(-1.0f, std::fmin(1.0f, dot));
		float theta = std::acos(dot);
		if (theta < 1e-6) {
			return lerp(q1, q2, t); // ������Ԫ���ӽ�ʱ��ʹ�����Բ�ֵ
		}
		float sinTheta = std::sin(theta);
		float weight1 = std::sin((1 - t) * theta) / sinTheta;
		float weight2 = std::sin(t * theta) / sinTheta;
		return (q1 * weight1 + q2 * weight2).normalized();
	}

	// ת��Ϊ��ת���� (�����ȴ洢)
	Matrix toMatrix() const {
		float xx = q[0] * q[0];
		float xy = q[0] * q[1];
		float xz = q[0] * q[2];
		float yy = q[1] * q[1];
		float zz = q[2] * q[2];
		float yz = q[1] * q[2];
		float wx = q[3] * q[0];
		float wy = q[3] * q[1];
		float wz = q[3] * q[2];
		Matrix matrix;
		matrix.m[0] = 1.0f - 2.0f * (yy + zz);
		matrix.m[1] = 2.0f * (xy - wz);
		matrix.m[2] = 2.0f * (xz + wy);
		matrix.m[3] = 0.0;
		matrix.m[4] = 2.0f * (xy + wz);
		matrix.m[5] = 1.0f - 2.0f * (xx + zz);
		matrix.m[6] = 2.0f * (yz - wx);
		matrix.m[7] = 0.0;
		matrix.m[8] = 2.0f * (xz - wy);
		matrix.m[9] = 2.0f * (yz + wx);
		matrix.m[10] = 1.0f - 2.0f * (xx + yy);
		matrix.m[11] = 0.0;
		matrix.m[12] = 0;
		matrix.m[13] = 0;
		matrix.m[14] = 0;
		matrix.m[15] = 1;
		return matrix;
	}

	// ��һ����Ŀ���
	Quaternion normalized() const {
		Quaternion q(*this);
		q.normalize();
		return q;
	}

	// �����Ԫ��
	friend std::ostream& operator<<(std::ostream& os, const Quaternion& q) {
		os << "(" << q.a << ", " << q.b << ", " << q.c << ", " << q.d << ")";
		return os;
	}
};

//�����������н� ���ص��ǻ���
float anglebetween(vec3 u, vec3 v) {
	float dotProduct = u.Dot(v);
	float magnitudeU = u.Length();
	float magnitudeV = v.Length();

	float cosTheta = dotProduct / (magnitudeU * magnitudeV);
	float result = std::acos(cosTheta);

	//Ҫע�������صĽǶȲ���0-360Ҫ�ò���жϷ���
	vec3 up = v.Cross(u);
	if (up.y < 0) {
		result= -result;
	}

	return result; // ���ػ���ֵ
}

//����������������ע�ⲻ�Ǵ��������Ǹ������������������нϴ��xyz���������
vec3 Max(const vec3& v1, const vec3& v2)
{
	return vec3(max(v1.v[0], v2.v[0]),
		max(v1.v[1], v2.v[1]),
		max(v1.v[2], v2.v[2]));
}
//����������������ע�ⲻ�Ǵ��������Ǹ������������������н�С��xyz���������
vec3 Min(const vec3& v1, const vec3& v2)
{
	return vec3(min(v1.v[0], v2.v[0]),
		min(v1.v[1], v2.v[1]),
		min(v1.v[2], v2.v[2]));
}