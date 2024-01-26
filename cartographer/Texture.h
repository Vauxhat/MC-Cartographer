#ifndef TEXTURE_2D_H_
#define TEXTURE_2D_H_

#include "Vector4.h"
#include "Vector2.h"

#include <vector>

namespace Vaux
{
	class Texture2D
	{
	public:
		enum class Sampling
		{
			POINT,
			BILINEAR
		};

		enum class Wrapping
		{
			CLAMP,
			REPEAT
		};

	private:
		int channels_;
		int width_, height_;
		std::vector<Vector4i> pixel_;

	public:
		Texture2D(const int& width = 0, const int& height = 0);
		~Texture2D();

		// Size functions.
		const Vector2i& GetSize() const;
		const int& GetWidth() const;
		const int& GetHeight() const;

		// Get functions.
		const Vector4i& Get(const int& x, const int& y) const;
		const Vector4i& Get(const Vector2i& uv) const;

		// Set functions.
		void Set(const int& x, const int& y, const Vector4i& colour);
		void Set(const Vector2i& uv, const Vector4i& colour);

		void SetA(const int& x, const int& y, const int& colour);
		void SetA(const Vector2i& uv, const int& colour);

		// Sampler function.
		const Vector4i Sample(const float& x, const float& y, const Sampling& sampling = Sampling::POINT, const Wrapping& wrapping = Wrapping::CLAMP) const;
		const Vector4i Sample(const Vector2f& uv, const Sampling& sampling = Sampling::POINT, const Wrapping& wrapping = Wrapping::CLAMP) const;

		// Resize function.
		void Resize(const int& width, const int& height);
		void ResizeCanvas(const int& width, const int& height);

		// File functions.
		const bool LoadFromFile(const char* filename);
		const bool SaveToFile(const char* filename) const;

	private:
		const bool SaveToPPM(const char* filename) const;
	};
}

#endif //TEXTURE_2D_H_
