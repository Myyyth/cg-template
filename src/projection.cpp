#include "projection.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>


Projection::Projection(unsigned short width, unsigned short height, std::string obj_file) : ReadObj(width, height, obj_file)
{
}

Projection::~Projection()
{
}

void Projection::DrawScene()
{
	parser->Parse();

	float rotangle = 0.0f * M_PI / 180.0;
	float4x4 rotate{
		{1, 0, 0, 0},
		{0, cos(rotangle), sin(rotangle), 0},
		{0, -sin(rotangle), cos(rotangle), 0},
		{0, 0, 0, 1},
	};

	float3 eye{ 0, 0, 0 };
	float3 look_at{ 0, 0, 1 };
	float3 up{ 0, 1, 0 };

	float3 zaxis = normalize(eye - look_at);
	float3 xaxis = normalize(cross(up, zaxis));
	float3 yaxis = cross(xaxis, zaxis);

	float aspect = static_cast<float>(width) / static_cast<float>(height);
	float fovy = 60.0 * M_PI / 180.0;
	float yscale = 1.0 / tan(fovy / 2.0);
	float xscale = yscale / aspect;
	float z_far = 10;
	float z_near = 1;

	float4x4 view{
		{xaxis.x, xaxis.y, xaxis.z, -dot(xaxis, eye)},
		{yaxis.x, yaxis.y, yaxis.z, -dot(yaxis, eye)},
		{zaxis.x, zaxis.y, zaxis.z, -dot(zaxis, eye)},
		{0, 0, 0, 1},
	};

	float4x4 translate{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 2, 1},
	};

	float4x4 projection{
		{xscale, 0, 0, 0},
		{0, yscale, 0, 0},
		{0, 0, z_far / (z_far - z_near), -z_near * z_far / (z_far - z_near)},
		{0, 0, 1, 0},
	};

	float4x4 world = mul(translate, rotate);

	float4x4 translate_matrix = mul(projection, view, world);

	for (auto face : parser->GetFaces()) {
		float4 translated[3];
		for (int i = 0; i < 3; i++) {
			translated[i] = mul(translate_matrix, face.vertexes[i]);
			translated[i] /= translated[i].w;
		}
		DrawTriangle(translated, color(255, 255, 255));
	}
}

void Projection::DrawTriangle(float4 triangle[3], color color)
{
	float x_center = width / 2;
	float y_center = height / 2;

	DrawLine(static_cast<unsigned short>(x_center + x_center * triangle[0].x),
		static_cast<unsigned short>(y_center - y_center * triangle[0].y),
		static_cast<unsigned short>(x_center + x_center * triangle[1].x),
		static_cast<unsigned short>(y_center - y_center * triangle[1].y), color);

	DrawLine(static_cast<unsigned short>(x_center + x_center * triangle[1].x),
		static_cast<unsigned short>(y_center - y_center * triangle[1].y),
		static_cast<unsigned short>(x_center + x_center * triangle[2].x),
		static_cast<unsigned short>(y_center - y_center * triangle[2].y), color);

	DrawLine(static_cast<unsigned short>(x_center + x_center * triangle[2].x),
		static_cast<unsigned short>(y_center - y_center * triangle[2].y),
		static_cast<unsigned short>(x_center + x_center * triangle[0].x),
		static_cast<unsigned short>(y_center - y_center * triangle[0].y), color);
}


