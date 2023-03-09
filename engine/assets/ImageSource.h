#pragma once
#include <string>

namespace nimo
{
    struct ImageSpecification{

    };
    class ImageSource
    {
    public:
        ImageSource(const std::string& file);
        ~ImageSource();
        unsigned char* data;
        int width, height, channels;
    };
};