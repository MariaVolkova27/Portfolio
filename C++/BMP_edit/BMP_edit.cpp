#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdint>

using namespace std;

#pragma pack(push, 1) // отключает выравнивание в структуре
struct BMPHeader { // структура для информации  файла BMP формата
    uint16_t signature; // формат файла
    uint32_t fileSize; // размер всего файла в байтах
    uint32_t reserved; // зарезервировано
    uint32_t dataOffset; // смещение, с которого начинаются пиксельные данные
};

struct DIBHeader { // структура с информацией об изображении (Device-Independent Bitmap Header)
    uint32_t headerSize; //  размер самой структуры
    int32_t width; // ширина изображения в пикселях
    int32_t height; // высота изображения в пикселях
    uint16_t planes; // число цветовых плоскостей (всегда 1)
    uint16_t bitsPerPixel; // бит на пиксель (24 = RGB, 32 = RGBA)
    uint32_t compression; // тип сжатия (0 = без сжатия)
    uint32_t imageSize; // размер пиксельных данных в байтах
    int32_t xPixelsPerMeter; // горизонтальное разрешения (пикселей на метр, редко используется)
    int32_t yPixelsPerMeter; // вертикальное разрешение (пикселей на метр)
    uint32_t colorsUsed; // число используемых цветов (0 = все)
    uint32_t importantColors; // число "важных" цветов (0 = все)
};
#pragma pack(pop) // включает обратно выравнивание

class BMPProcessor {
private:
    BMPHeader bmpHeader;
    DIBHeader dibHeader;
    vector<uint8_t> pixelData;
    vector<vector<char>> consoleImage;
    string inputFilename;
    string outputFilename;

    void readBMP(const string& filename) {
        ifstream file(filename, ios::binary);
        if (!file) {
            throw runtime_error("Failed to open file: " + filename);
        }

        file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader));
        file.read(reinterpret_cast<char*>(&dibHeader), sizeof(DIBHeader));

        if (bmpHeader.signature != 0x4D42) {
            throw runtime_error("Not a valid BMP file");
        }

        if (dibHeader.bitsPerPixel != 24 && dibHeader.bitsPerPixel != 32) {
            throw runtime_error("Only 24-bit and 32-bit BMP files are supported");
        }

        file.seekg(bmpHeader.dataOffset, ios::beg);

        int rowSize = ((dibHeader.bitsPerPixel * dibHeader.width + 31) / 32) * 4; // переводит биты в DWORD-блоки, потом обратно в байты
        int pixelDataSize = rowSize * abs(dibHeader.height);
        pixelData.resize(pixelDataSize);
        file.read(reinterpret_cast<char*>(pixelData.data()), pixelDataSize);

        file.close();
    }

    void processPixelsToConsole() {
        int width = dibHeader.width;
        int height = abs(dibHeader.height);
        int bytesPerPixel = dibHeader.bitsPerPixel / 8;
        int rowSize = ((dibHeader.bitsPerPixel * width + 31) / 32) * 4;

        consoleImage.resize(height, vector<char>(width, ' '));

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int offset = y * rowSize + x * bytesPerPixel;

                uint8_t blue = pixelData[offset];
                uint8_t green = pixelData[offset + 1];
                uint8_t red = pixelData[offset + 2];

                if (red == 0 && green == 0 && blue == 0) {
                    consoleImage[height - 1 - y][x] = '#'; // Black pixel
                }
                else if (red == 255 && green == 255 && blue == 255) {
                    consoleImage[height - 1 - y][x] = '.'; // White pixel
                }
                else {
                    throw runtime_error("Image contains colors other than black and white");
                }
            }
        }
    }

    void drawLine(int x1, int y1, int x2, int y2) {
        int width = dibHeader.width;
        int height = abs(dibHeader.height);
        int bytesPerPixel = dibHeader.bitsPerPixel / 8; // перевод в байты на пиксель
        int rowSize = ((dibHeader.bitsPerPixel * width + 31) / 32) * 4;

        // Convert coordinates to image space (BMP stores rows bottom-to-top)
        y1 = height - 1 - y1;
        y2 = height - 1 - y2;

        // Bresenham's line algorithm
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            // Set pixel to black
            int offset = y1 * rowSize + x1 * bytesPerPixel;
            pixelData[offset] = 0;     // Blue
            pixelData[offset + 1] = 0; // Green
            pixelData[offset + 2] = 0; // Red

            if (x1 == x2 && y1 == y2) break;

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }

public:
    void run() {
        try {
            // Get input filename
            cout << ">> Enter input BMP file name: ";
            cin >> inputFilename;

            // Read and process the BMP file
            readBMP(inputFilename);
            processPixelsToConsole();

            // Display original image
            cout << "\nOriginal Image:\n";
            for (const auto& row : consoleImage) {
                for (char pixel : row) {
                    cout << pixel;
                }
                cout << '\n';
            }

            // Draw X (cross)
            int width = dibHeader.width;
            int height = abs(dibHeader.height);
            drawLine(0, 0, width - 1, height - 1);      // Diagonal from top-left to bottom-right
            drawLine(0, height - 1, width - 1, 0);      // Diagonal from bottom-left to top-right

            // Process modified image for console display
            processPixelsToConsole();

            // Display modified image
            cout << "\nModified Image (with X):\n";
            for (const auto& row : consoleImage) {
                for (char pixel : row) {
                    cout << pixel;
                }
                cout << '\n';
            }

            // Get output filename and save
            cout << "\n>> Enter output BMP file name: ";
            cin >> outputFilename;

            ofstream outFile(outputFilename, ios::binary);
            if (!outFile) {
                throw runtime_error("Failed to create output file");
            }

            // Write headers
            outFile.write(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHeader));
            outFile.write(reinterpret_cast<char*>(&dibHeader), sizeof(DIBHeader));

            // Write pixel data
            outFile.seekp(bmpHeader.dataOffset, ios::beg);
            outFile.write(reinterpret_cast<char*>(pixelData.data()), pixelData.size());

            outFile.close();

            cout << "Modified image saved to " << outputFilename << endl;
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
};

int main() {
    BMPProcessor processor;
    processor.run();
    return 0;
}
