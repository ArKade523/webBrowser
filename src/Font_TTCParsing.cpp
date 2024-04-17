#include "Font_TTCParsing.h"

#define NDEBUG

void printFontStyles(const char* filePath) {
    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        std::cerr << "Could not initialize FreeType library." << std::endl;
        return;
    }

    FT_Face face;
    int face_index = 0;

    while (true) {
        // Load a face from the file. In TTC files, face_index selects the face.
        error = FT_New_Face(library, filePath, face_index, &face);
        if (error == FT_Err_Unknown_File_Format) {
            std::cerr << "The file format is not supported." << std::endl;
            break;
        } else if (error) {
            std::cerr << "The font could not be opened or read, or is broken." << std::endl;
            break;
        }

        // Print the face's style name (usually indicates bold, italic, etc.)
        std::cout << "Face " << face_index << ": " << face->style_name << std::endl;

        // Check if the style matches what you're looking for
        std::string styleName = face->style_name;
        if (styleName.find("Bold") != std::string::npos) {  // Example condition
            std::cout << "Found 'Bold' style at index: " << face_index << std::endl;
            // You can exit if you're looking for a specific style
            // break;
        }

        // Increment face_index to load the next face in the next iteration
        face_index++;

        FT_Done_Face(face);  // Clean up the face object
    }

    FT_Done_FreeType(library);  // Clean up the FreeType library
}

int findFontStyleIndex(const char* filePath, const char* styleName) {
    FT_Library library;
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        std::cerr << "Could not initialize FreeType library." << std::endl;
        return -1;
    }

    FT_Face face;
    int face_index = 0;

    while (true) {
        // Load a face from the file. In TTC files, face_index selects the face.
        error = FT_New_Face(library, filePath, face_index, &face);
        if (error == FT_Err_Unknown_File_Format) {
            std::cerr << "The file format is not supported." << std::endl;
            break;
        } else if (error) {
            std::cerr << "The font could not be opened or read, or is broken." << std::endl;
            break;
        }

        // Check if the style matches what you're looking for
        std::string style = face->style_name;
        if (style.find(styleName) != std::string::npos) {
            #ifndef NDEBUG
                std::cout << "Found '" << styleName << "' style at index: " << face_index << std::endl;
            #endif

            FT_Done_Face(face);  // Clean up the face object
            FT_Done_FreeType(library);  // Clean up the FreeType library
            return face_index;
        }

        // Increment face_index to load the next face in the next iteration
        face_index++;

        FT_Done_Face(face);  // Clean up the face object
    }

    FT_Done_FreeType(library);  // Clean up the FreeType library
    return -1;
}