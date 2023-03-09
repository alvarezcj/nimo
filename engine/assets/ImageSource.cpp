#include "ImageSource.h"

#include "stb_image.h"

nimo::ImageSource::ImageSource(const std::string& file)
{
    stbi_set_flip_vertically_on_load(1);
    data = stbi_load(file.c_str(), &width, &height, &channels, 0);
}
nimo::ImageSource::~ImageSource(){
    if(data)
        stbi_image_free(data);
}