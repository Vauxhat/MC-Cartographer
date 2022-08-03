#ifndef MC_MAP_DATA_H_
#define MC_MAP_DATA_H_

#include <vector>

namespace Cartographer
{
	class MCMapData
	{
	public:
		static const int defaultWidth = 128;
		static const int defaultHeight = 128;

	private:
		int width_, height_;
		std::vector<int> colourID_;

	public:
		// Constructors and Destructors.
		MCMapData(const int& width = defaultWidth, const int& height = defaultHeight);
		MCMapData(const char* filename, const int& width = defaultWidth, const int& height = defaultHeight);
		~MCMapData();

		// Size functions.
		const int& GetWidth() const;
		const int& GetHeight() const;
		
		// Getters and setters.
		const int& Get(const int& x, const int& y) const;
		void Set(const int& x, const int& y, const int& val);

		// File functions.
		const bool LoadFromFile(const char* filename, const int& width = defaultWidth, const int& height = defaultHeight);
		const bool SaveToFile(const char* filename) const;
	};
}

#endif //MC_MAP_DATA_H_
