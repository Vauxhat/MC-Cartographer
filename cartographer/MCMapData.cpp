#include "MCMapData.h"
#include <fstream>

using namespace Cartographer;
using namespace std;

MCMapData::MCMapData(const int& width, const int& height) : width_(width), height_(height)
{
    // Reserve space in colour vector.
    colourID_.reserve(width_ * height_);

    for (int i = 0; i < width_ * height_; i++)
    {
        // Initialise colour vector.
        colourID_.push_back(0);
    }
}
MCMapData::MCMapData(const char* filename, const int& width, const int& height)
{
    // Load from file.
    LoadFromFile(filename, width, height);
}
MCMapData::~MCMapData()
{
    // Default destructor.
}

// Returns width of map.
const int& MCMapData::GetWidth() const
{
    return width_;
}
// Returns height of map.
const int& MCMapData::GetHeight() const
{
    return height_;
}

// Returns a colour ID from position (x, y) in the map.
const int& MCMapData::Get(const int& x, const int& y) const
{
    return colourID_[x + y * width_];
}
// Sets a colour ID at position (x, y) in the map
void MCMapData::Set(const int& x, const int& y, const int& val)
{
    colourID_[x + y * width_] = val;
}

// Loads map data from a binary map file.
const bool MCMapData::LoadFromFile(const char* filename, const int& width, const int& height)
{
    // Open map file.
    ifstream inputData;
    inputData.open(filename, ios::in | ios::binary | ios::ate);

    // Check if file was succesfully opened.
    if (inputData.is_open())
    {
        // Update local width and height.
        width_ = width;
        height_ = height;

        // Initialise colour ID vector, reserve memory.
        vector<int> colourID;
        colourID.reserve(width_ * height_);

        streampos size = sizeof(unsigned char);
        unsigned char* memblock = new unsigned char[size];

        // Set seek position.
        inputData.seekg(0, ios::beg);

        for (int i = 0; i < width_ * height_; i++)
        {
            // Read current memory block (break if out of data).
            if (!inputData.read(reinterpret_cast<char*>(memblock), size)) break;

            // Convert memory block from binary to integer, store in colour array.
            colourID.push_back((unsigned int)*memblock);
        }

        // Close map file, delete memory block.
        inputData.close();

        // Update colourID with new data.
        colourID_ = colourID;
    }
    else
    {
        // Failed to open file.
        return false;
    }

    // File loaded successfully.
    return true;
}
// Saves map data to a binary file.
const bool MCMapData::SaveToFile(const char* filename) const
{
    // Open output file.
    ofstream outputData;
    outputData.open(filename, ios::out | ios::binary | ios::ate);

    // Check if file was succesfully opened.
    if (outputData.is_open())
    {
        // Loop through each colour in vector.
        for (int i = 0; i < width_ * height_; i++)
        {
            // Convert colour ID to char, store in map file.
            outputData << (char)colourID_[i];
        }

        // Close output file.
        outputData.close();
    }
    else
    {
        // Could not open file.
        return false;
    }

    // File loaded successfully.
    return true;
}