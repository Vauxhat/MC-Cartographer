#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>
#include <cmath>

#include "Vector3.h"
#include "Texture.h"
#include "MCMapData.h"

using namespace std;
using namespace Vaux;
using namespace Cartographer;

const int bayerWidth = 16;
const int bayerHeight = 16;

const float bayerMatrix[]
{
    0, 0.5, 0.125, 0.625, 0.03125, 0.53125, 0.15625, 0.65625, 0.0078125, 0.507812, 0.132812, 0.632812, 0.0390625, 0.539062, 0.164062, 0.664062,
    0.75, 0.25, 0.875, 0.375, 0.78125, 0.28125, 0.90625, 0.40625, 0.757812, 0.257812, 0.882812, 0.382812, 0.789062, 0.289062, 0.914062, 0.414062,
    0.1875, 0.6875, 0.0625, 0.5625, 0.21875, 0.71875, 0.09375, 0.59375, 0.195312, 0.695312, 0.0703125, 0.570312, 0.226562, 0.726562, 0.101562, 0.601562,
    0.9375, 0.4375, 0.8125, 0.3125, 0.96875, 0.46875, 0.84375, 0.34375, 0.945312, 0.445312, 0.820312, 0.320312, 0.976562, 0.476562, 0.851562, 0.351562,
    0.046875, 0.546875, 0.171875, 0.671875, 0.015625, 0.515625, 0.140625, 0.640625, 0.0546875, 0.554688, 0.179688, 0.679688, 0.0234375, 0.523438, 0.148438, 0.648438,
    0.796875, 0.296875, 0.921875, 0.421875, 0.765625, 0.265625, 0.890625, 0.390625, 0.804688, 0.304688, 0.929688, 0.429688, 0.773438, 0.273438, 0.898438, 0.398438,
    0.234375, 0.734375, 0.109375, 0.609375, 0.203125, 0.703125, 0.078125, 0.578125, 0.242188, 0.742188, 0.117188, 0.617188, 0.210938, 0.710938, 0.0859375, 0.585938,
    0.984375, 0.484375, 0.859375, 0.359375, 0.953125, 0.453125, 0.828125, 0.328125, 0.992188, 0.492188, 0.867188, 0.367188, 0.960938, 0.460938, 0.835938, 0.335938,
    0.0117188, 0.511719, 0.136719, 0.636719, 0.0429688, 0.542969, 0.167969, 0.667969, 0.00390625, 0.503906, 0.128906, 0.628906, 0.0351562, 0.535156, 0.160156, 0.660156,
    0.761719, 0.261719, 0.886719, 0.386719, 0.792969, 0.292969, 0.917969, 0.417969, 0.753906, 0.253906, 0.878906, 0.378906, 0.785156, 0.285156, 0.910156, 0.410156,
    0.199219, 0.699219, 0.0742188, 0.574219, 0.230469, 0.730469, 0.105469, 0.605469, 0.191406, 0.691406, 0.0664062, 0.566406, 0.222656, 0.722656, 0.0976562, 0.597656,
    0.949219, 0.449219, 0.824219, 0.324219, 0.980469, 0.480469, 0.855469, 0.355469, 0.941406, 0.441406, 0.816406, 0.316406, 0.972656, 0.472656, 0.847656, 0.347656,
    0.0585938, 0.558594, 0.183594, 0.683594, 0.0273438, 0.527344, 0.152344, 0.652344, 0.0507812, 0.550781, 0.175781, 0.675781, 0.0195312, 0.519531, 0.144531, 0.644531,
    0.808594, 0.308594, 0.933594, 0.433594, 0.777344, 0.277344, 0.902344, 0.402344, 0.800781, 0.300781, 0.925781, 0.425781, 0.769531, 0.269531, 0.894531, 0.394531,
    0.246094, 0.746094, 0.121094, 0.621094, 0.214844, 0.714844, 0.0898438, 0.589844, 0.238281, 0.738281, 0.113281, 0.613281, 0.207031, 0.707031, 0.0820312, 0.582031,
    0.996094, 0.496094, 0.871094, 0.371094, 0.964844, 0.464844, 0.839844, 0.339844, 0.988281, 0.488281, 0.863281, 0.363281, 0.957031, 0.457031, 0.832031, 0.332031
};

enum class DitherType
{
    ORDERED,
    FLOYD_STEINBERG
};

// Function pre declaration.
const bool LoadPaletteFromFile(const char* filename, vector<Vector3i>* output);
const bool ConvertImageToMap(const char* inputPath, const char* outputPath, const vector<Vector3i>& paletteData, DitherType dithering = DitherType::ORDERED);
const bool ConvertMapToImage(const char* inputPath, const char* outputPath, const vector<Vector3i>& paletteData);

int main(int argc, char* argv[])
{
    // Get path to exe and colour file.
    filesystem::path exeDirectory = filesystem::weakly_canonical(argv[0]).parent_path();
    filesystem::path colourPath = filesystem::path(exeDirectory.string() + "\\colours.csv");

    // Load palette data.
    vector<Vector3i> paletteData;
    if(!LoadPaletteFromFile(colourPath.string().c_str(), &paletteData))
        return 1;

    if (argc == 1)
    {
        // Output text.
        cout << "Please enter file name and location (e.g. C:/image.png):\n";

        // Get file path.
        string stringPath;
        getline(cin, stringPath);
        filesystem::path inputPath = stringPath.c_str();

        // Store file name, remove extension.
        string filename = inputPath.filename().string();
        filename.resize(filename.size() - inputPath.extension().string().size());

        // Check if input is a binary file (map data).
        if (inputPath.has_extension())
        {
            // Output text.
            cout << "Enter 0 for ordered dithering or 1 for Floyd-Steinberg dithering:\n";

            // Get dithering mode.
            int dithering;
            cin >> dithering;

            // Generate output path.
            string outputPath(exeDirectory.string() + "\\" + filename + "_map");

            // Input has a file type, attempt map conversion.
            MCMapData outputMap;
            if (!ConvertImageToMap(inputPath.string().c_str(), outputPath.c_str(), paletteData, DitherType(dithering)))
                return 1;
        }
        else
        {
            // Generate output path.
            string outputPath(exeDirectory.string() + "\\" + filename + ".png");

            // Input is binary, attempt image conversion.
            if (!ConvertMapToImage(inputPath.string().c_str(), outputPath.c_str(), paletteData))
                return 1;
        }
    }
    else
    {
        for (int i = 1; i < argc; i++)
        {
            // Store file path.
            filesystem::path inputPath(argv[i]);

            // Store file name, remove extension.
            string filename = inputPath.filename().string();
            filename.resize(filename.size() - inputPath.extension().string().size());

            // Check if input has an extension.
            if (inputPath.has_extension())
            {
                // Generate output path.
                string outputPath(inputPath.parent_path().string() + "\\" + filename + "_map");

                // Input has a file type, attempt map conversion.
                MCMapData outputMap;
                if (!ConvertImageToMap(argv[i], outputPath.c_str(), paletteData, DitherType::FLOYD_STEINBERG))
                    continue;
            }
            else
            {
                // Generate output path.
                string outputPath(inputPath.parent_path().string() + "\\" + filename + ".png");

                // Input is binary, attempt image conversion.
                if (!ConvertMapToImage(argv[i], outputPath.c_str(), paletteData))
                    continue;
            }
        }
    }

    // Program executed successfully.
    return 0;
}

const bool LoadPaletteFromFile(const char* filename, vector<Vector3i>* output)
{
    // Open colour file.
    ifstream inputData;
    inputData.open(filename, ios::in | ios::ate);

    // Check if file was succesfully opened.
    if (inputData.is_open())
    {
        // Clear output.
        output->clear();

        // Reset seek position.
        inputData.seekg(0, ios::beg);

        string line;

        // Loop through each line in the table.
        while (inputData >> std::quoted(line, '"'))
        {
            string x, y, z;
            stringstream ss(line);

            // Extract x, y, and z components from line.
            getline(ss, x, ',');
            getline(ss, y, ',');
            getline(ss, z, ',');

            // Create colour from file data.
            Vector3i colour = Vector3i(atoi(x.c_str()), atoi(y.c_str()), atoi(z.c_str()));

            // Add colours to colour index.
            output->push_back(colour * 0.71f);
            output->push_back(colour * 0.86f);
            output->push_back(colour);
            output->push_back(colour * 0.53f);
        }

        // Close colour file.
        inputData.close();
    }
    else
    {
        // Failed to open file.
        return false;
    }

    // File loaded successfully.
    return true;
}

const bool ConvertImageToMap(const char* inputFile, const char* outputFile, const vector<Vector3i>& paletteData, DitherType dithering)
{
    // Load image.
    Texture2D inputTexture;
    if (inputTexture.LoadFromFile(inputFile))
    {
        // Calculate individual x and y scales.
        float scaleX = float(MCMapData::defaultWidth) / float(inputTexture.GetWidth());
        float scaleY = float(MCMapData::defaultHeight) / float(inputTexture.GetHeight());

        // Find minimum scale between x and y dimensions.
        float scale = min(scaleX, scaleY);

        // Scale image to fit within map dimensions.
        inputTexture.Resize(static_cast<int>(inputTexture.GetWidth() * scale), static_cast<int>(inputTexture.GetHeight() * scale));

        // Resize image canvas to map dimensions.
        inputTexture.ResizeCanvas(MCMapData::defaultWidth, MCMapData::defaultHeight);
    }
    else
    {
        // Failed to load texture.
        return false;
    }

    // Create output map.
    MCMapData outputMap(inputTexture.GetWidth(), inputTexture.GetHeight());

    // Select dithering method.
    switch (dithering)
    {
    case DitherType::FLOYD_STEINBERG:
    {
        // Loop through each pixel in the texture.
        for (int y = 0; y < outputMap.GetHeight(); y++)
        {
            for (int x = 0; x < outputMap.GetWidth(); x++)
            {
                // Get current pixel from texture.
                Vector4i sampleColour = inputTexture.Get(x, y);

                // Calculate nearest alpha using 1 bit colour, determine quantisation error.
                int nearestAlpha = static_cast<int>(roundf((float)sampleColour.w / 255.f) * 255);
                int alphaQuantisation = sampleColour.w - nearestAlpha;

                // Spread quanisation error to surrounding pixels.
                inputTexture.SetA(x + 1, y, inputTexture.Get(x + 1, y).w + alphaQuantisation * 7.0f / 16.0f);
                inputTexture.SetA(x - 1, y + 1, inputTexture.Get(x - 1, y + 1).w + alphaQuantisation * 3.0f / 16.0f);
                inputTexture.SetA(x, y + 1, inputTexture.Get(x, y + 1).w + alphaQuantisation * 5.0f / 16.0f);
                inputTexture.SetA(x + 1, y + 1, inputTexture.Get(x + 1, y + 1).w + alphaQuantisation * 1.0f / 16.0f);

                // Check if pixel is transparent.
                if (nearestAlpha == 0)
                {
                    // Set colour ID to transparent.
                    outputMap.Set(x, y, 0);
                }
                else
                {
                    // Initialise nearest variables.
                    int nearest = 4;
                    int nearestDistance = Vector3i::LengthSqr(paletteData[nearest] - Vector3i(sampleColour.x, sampleColour.y, sampleColour.z));

                    // Loop through each colour in palette skipping the first five entries (0-3 is transprency, 4 was used above).
                    for (unsigned int i = 5; i < paletteData.size(); i++)
                    {
                        // Calculate distance to sample colour, avoid sqrt calculation.
                        int distance = Vector3i::LengthSqr(paletteData[i] - Vector3i(sampleColour.x, sampleColour.y, sampleColour.z));

                        // Check if colour is closer than nearest.
                        if (distance < nearestDistance)
                        {
                            // Update nearest colour.
                            nearest = i;
                            nearestDistance = distance;
                        }
                    }

                    // Store nearest ID in output map.
                    outputMap.Set(x, y, nearest);

                    // Calculate quantisation error.
                    Vector3i quantisation = Vector3i(sampleColour.x, sampleColour.y, sampleColour.z) - paletteData[nearest];

                    // Spread quanisation error to surrounding pixels.
                    inputTexture.Set(x + 1, y, inputTexture.Get(x + 1, y) + Vector4i(quantisation, 0) * 7.0f / 16.0f);
                    inputTexture.Set(x - 1, y + 1, inputTexture.Get(x - 1, y + 1) + Vector4i(quantisation, 0) * 3.0f / 16.0f);
                    inputTexture.Set(x, y + 1, inputTexture.Get(x, y + 1) + Vector4i(quantisation, 0) * 5.0f / 16.0f);
                    inputTexture.Set(x + 1, y + 1, inputTexture.Get(x + 1, y + 1) + Vector4i(quantisation, 0) * 1.0f / 16.0f);
                }
            }
        }

        break;
    }
    default:
    {
        // Loop through each pixel in the texture.
        for (int y = 0; y < outputMap.GetHeight(); y++)
        {
            for (int x = 0; x < outputMap.GetWidth(); x++)
            {
                // Get current pixel from texture.
                Vector4i sampleColour = inputTexture.Get(x, y);

                // Apply dither matrix to alpha using 1 bit colour.
                float alphaDither = 255.f * (bayerMatrix[x % bayerWidth + (y % bayerHeight) * bayerWidth] - 0.5f);
                int alpha = static_cast<int>(roundf(((float)sampleColour.w + alphaDither) / 255.f));

                // Check for transparency. Ordered sorting doesn't go to zero alpha, check sample colour as a workaround.
                if (sampleColour.w == 0 || alpha == 0)
                {
                    // Set colour ID to transparent.
                    outputMap.Set(x, y, 0);
                }
                else
                {
                    // Apply dither matrix to RGB using map colour.
                    int colourDither = static_cast<int>((255.f / 8.f) * (bayerMatrix[x % bayerWidth + (y % bayerHeight) * bayerWidth] - 0.5f));
                    sampleColour = sampleColour + Vector4i(colourDither, colourDither, colourDither, 0);

                    // Initialise nearest variables.
                    int nearest = 4;
                    int nearestDistance = Vector3i::LengthSqr(paletteData[nearest] - Vector3i(sampleColour.x, sampleColour.y, sampleColour.z));

                    // Loop through each colour in palette skipping the first five entries (0-3 is transprency, 4 was used above).
                    for (unsigned int i = 5; i < paletteData.size(); i++)
                    {
                        // Calculate distance to sample colour, avoid sqrt calculation.
                        int distance = Vector3i::LengthSqr(paletteData[i] - Vector3i(sampleColour.x, sampleColour.y, sampleColour.z));

                        // Check if colour is closer than nearest.
                        if (distance < nearestDistance)
                        {
                            // Update nearest colour.
                            nearest = i;
                            nearestDistance = distance;
                        }
                    }

                    // Store nearest ID in output map.
                    outputMap.Set(x, y, nearest);
                }
            }
        }

        break;
    }
    }

    // Save map data to file.
    if (!outputMap.SaveToFile(outputFile))
        return false;

    // Successfull conversion.
    return true;
}

const bool ConvertMapToImage(const char* inputFile, const char* outputFile, const vector<Vector3i>& paletteData)
{
    // Load map data from file.
    MCMapData inputMap;
    if (!inputMap.LoadFromFile(inputFile))
        return false;

    // Create output image.
    Texture2D outputTexture(inputMap.GetWidth(), inputMap.GetHeight());

    // Convert map data to image.
    for (int y = 0; y < outputTexture.GetHeight(); y++)
    {
        for (int x = 0; x < outputTexture.GetWidth(); x++)
        {
            // Get colour ID from map data.
            int colourID = inputMap.Get(x, y);

            // Check for transparency.
            if (colourID >= 0 && colourID < 4)
            {
                // Set pixel as transparent.
                outputTexture.Set(x, y, Vector4i(0, 0, 0, 0));
            }
            else
            {
                // Set pixel using colour from palette.
                outputTexture.Set(x, y, Vector4i(paletteData[colourID], 255));
            }
        }
    }

    // Save image to file.
    if (!outputTexture.SaveToFile(outputFile))
        return false;

    // Successfull conversion.
    return true;
}