#include "Texture.h"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define __STDC_LIB_EXT1__
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

using namespace Vaux;
using namespace std;

Texture2D::Texture2D(const int& width, const int& height) : width_(width), height_(height), channels_(4)
{
	// Reserve space in vector.
	pixel_.reserve(width_ * height_);

	// Initialise pixel vector.
	for (int i = 0; i < width_ * height_; i++)
	{
		pixel_.push_back(Vector4i(0, 0, 0, 255));
	}
}
Texture2D::~Texture2D()
{
	// Default deconstructor.
}

// Returns texture dimensions (width, height).
const Vector2i& Texture2D::GetSize() const
{
	return Vector2i(width_, height_);
}
// Returns texture width.
const int& Texture2D::GetWidth() const
{
	return width_;
}
// Returns texture height.
const int& Texture2D::GetHeight() const
{
	return height_;
}

// Returns colour of current pixel.
const Vector4i& Texture2D::Get(const int& x, const int& y) const
{
	return pixel_[x + y * width_];
}
// Returns colour of current pixel.
const Vector4i& Texture2D::Get(const Vector2i& uv) const
{
	return Get(uv.x, uv.y);
}

// Sets colour of current pixel.
void Texture2D::Set(const int& x, const int& y, const Vector4i& colour)
{
	pixel_[x + y * width_] = colour;
}
// Sets colour of current pixel.
void Texture2D::Set(const Vector2i& uv, const Vector4i& colour)
{
	Set(uv.x, uv.y, colour);
}

// Returns a sample from the texture using a given interpolation method.
const Vector4i Texture2D::Sample(const float& x, const float& y, const Sampling& sampling, const Wrapping& wrapping) const
{
	// Store local sample coordinates.
	float sampleX = x;
	float sampleY = y;

	// Store local height variables.
	float fwidth = static_cast<float>(width_);
	float fheight = static_cast<float>(height_);

	// Update sample position based on wrapping mode.
	switch (wrapping)
	{
	case Wrapping::REPEAT:
	{
		// Loop x and y coordinates within texture range.
		sampleX = fmodf(sampleX, fwidth);
		sampleY = fmodf(sampleY, fheight);

		// If sample is negative, loop back to upper bounds.
		if (sampleX < 0.f) sampleX += fwidth;
		if (sampleY < 0.f) sampleY += fheight;

		break;
	}
	default: // Wrapping::CLAMP
	{
		// Clamp texture by default.
		sampleX = clamp(sampleX, 0.f, fwidth - 1.f);
		sampleY = clamp(sampleY, 0.f, fheight - 1.f);

		break;
	}
	}

	// Sample texture based on sampling mode.
	switch (sampling)
	{
	case Sampling::BILINEAR:
	{
		// X interpolation values.
		int minX = static_cast<int>(sampleX);
		int maxX = minX + 1;
		float lerpX = sampleX - static_cast<float>(minX);
		
		// Y interpolation values.
		int minY = static_cast<int>(sampleY);
		int maxY = minY + 1;
		float lerpY = sampleY - static_cast<float>(minY);

		// Clamp max X.
		if (maxX >= width_)
			maxX = width_ - 1;

		// Clamp max Y.
		if (maxY >= height_)
			maxY = height_ - 1;

		// Interpolate between samples of x.
		Vector4i y1 = Vector4i::Lerp(Get(minX, minY), Get(maxX, minY), lerpX);
		Vector4i y2 = Vector4i::Lerp(Get(minX, maxY), Get(maxX, maxY), lerpX);

		// Interpolate between samples of y, return value.
		return Vector4i::Lerp(y1, y2, lerpY);

		break;
	}
	default: // Sampling::POINT
	{
		// Return sample at rounded texture coodinates
		return Get(int(sampleX), int(sampleY)); break;
	}
	}
}
// Returns a sample from the texture using a given interpolation method.
const Vector4i Texture2D::Sample(const Vector2f& uv, const Sampling& sampling, const Wrapping& wrapping) const
{
	return Sample(uv.x, uv.y, sampling, wrapping);
}

// Resizes current texture to given dimensions. Texture will be interpolated using sampling.
void Texture2D::Resize(const int& width, const int& height)
{
	// Check if texture size is changed.
	if ((width != width_) || (height != height_))
	{
		// Create new vector for storing pixels.
		std::vector<Vector4i> pixel;
		pixel.reserve(width * height);

		// Loop through each pixel in texture.
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// Calculate relative sample position for current pixel.
				float sampleX = float(x) * float(width_) / float(width);
				float sampleY = float(y) * float(height_) / float(height);

				float offset = 1.f;

				// Take multiple samples from the image at offsets.
				Vector4i a = Sample(sampleX - offset, sampleY + offset, Sampling::BILINEAR, Wrapping::CLAMP);
				Vector4i b = Sample(sampleX + offset, sampleY + offset, Sampling::BILINEAR, Wrapping::CLAMP);
				Vector4i c = Sample(sampleX - offset, sampleY - offset, Sampling::BILINEAR, Wrapping::CLAMP);
				Vector4i d = Sample(sampleX - offset, sampleY - offset, Sampling::BILINEAR, Wrapping::CLAMP);

				// Average sampled colours, store in texture.
				pixel.push_back((a + b + c + d) / 4.f);
			}
		}

		// Update texture with new values.
		width_ = width;
		height_ = height;
		pixel_ = pixel;
	}
}
// Resizes current texture canvas to given dimensions. Dimensions are changed relative to the centre.
void Texture2D::ResizeCanvas(const int& width, const int& height)
{
	// Check if texture size is changed.
	if ((width != width_) || (height != height_))
	{
		// Create new vector for storing pixels.
		std::vector<Vector4i> pixel;
		pixel.reserve(width * height);

		// Loop through each pixel in texture.
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// Calculate local coordinates within source texture.
				Vector2i localCoords(x + (width_ - width) / 2, y + (height_ - height) / 2);

				// Check if local coordinates are within image bounds.
				if ((localCoords.x >= 0) && (localCoords.x < width_) && (localCoords.y >= 0) && (localCoords.y < height_))
				{
					// Sample source image.
					pixel.push_back(pixel_[localCoords.x + localCoords.y * width_]);
				}
				else
				{
					// Fill empty space with transparent pixels.
					pixel.push_back(Vector4i(0, 0, 0, 0));
				}
			}
		}

		// Update texture with new values.
		width_ = width;
		height_ = height;
		pixel_ = pixel;
	}
}

// Loads texture data from a file.
const bool Texture2D::LoadFromFile(const char* filename)
{
	const int desiredChannels = 4;

	int width, height, channels;
	unsigned char* data = stbi_load(filename, &width, &height, &channels, desiredChannels);

	// Check if data exists.
	if (data)
	{
		// Update width and height.
		channels_ = channels;
		width_ = width;
		height_ = height;

		// Initialise pixel vector.
		std::vector<Vector4i> pixel;
		pixel.reserve(width * height);

		// Loop thorugh each colour in image data.
		for (int i = 0; i < width * height * desiredChannels; i += desiredChannels)
		{
			Vector4i colour;
		
			// Update each channel of colour.
			colour.x = data[i];
			colour.y = data[i + 1];
			colour.z = data[i + 2];
			colour.w = data[i + 3];
		
			// Add colour to pixel vector.
			pixel.push_back(colour);
		}

		// Update pixel vector.
		pixel_ = pixel;

		// File loaded successfully.
		return true;
	}
	else
	{
		// Failed to load file.
		return false;
	}
}
// Saves texture data to a file. File type based on ending.
const bool Texture2D::SaveToFile(const char* filename) const
{
	// Get file path.
	filesystem::path filepath(filename);

	// Check if file has an extension.
	if (filepath.has_extension())
	{
		// Store file extensions.
		string extension = filepath.extension().string();

		// Check extension type.
		if (extension == ".ppm")
		{
			// Save as PMM.
			return SaveToPPM(filename);
		}
		else if (extension == ".png")
		{
			// Create new memory block.
			unsigned char* memblock = new unsigned char[width_ * height_ * 4];

			// Store raw colour data in memory block.
			for (int i = 0; i < width_ * height_; i++)
			{
				memblock[i * 4 + 0] = pixel_[i].x;
				memblock[i * 4 + 1] = pixel_[i].y;
				memblock[i * 4 + 2] = pixel_[i].z;
				memblock[i * 4 + 3] = pixel_[i].w;
			}

			// Write data to png.
			stbi_write_png(filename, width_, height_, channels_, memblock, 0);
		}
		else
		{
			// Extension type not supported.
			return false;
		}
	}
	else
	{
		// No extension found.
		return false;
	}

	// Save successfull.
	return true;
}

// Saves texture to a PPM file.
const bool Texture2D::SaveToPPM(const char* filename) const
{
	// Save to image.
	ofstream outputData;
	outputData.open(filename, ios::out | ios::ate);

	// Check if file was succesfully opened.
	if (outputData.is_open())
	{
		// Header
		outputData << "P3\n";
		outputData << width_ << " " << height_ << std::endl;
		outputData << 255 << std::endl;

		// Loop through each pixel in texture.
		for (int y = 0; y < height_; y++)
		{
			for (int x = 0; x < width_; x++)
			{
				int position = (x + y * width_) * channels_;

				// Send colour to file as RGB.
				Vector4i colour = pixel_[x + y * width_];
				outputData << colour.x << " " << colour.y << " " << colour.z << std::endl;
			}
		}

		// Close output file, return true.
		outputData.close();
		return true;
	}
	else
	{
		// Failed to output file.
		return false;
	}
}