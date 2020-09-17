// Stegosaurus_CPP_Implementation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/numeric/sampler.hpp>
#include <boost/gil/extension/numeric/resample.hpp>
#include <boost/coroutine2/all.hpp>

#include <bitset>

//#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

namespace gil = boost::gil;

std::vector<std::string> GenerateData(std::string data) {

    std::vector<std::string> generatedData;

    for (char& _char : data) {
        generatedData.push_back(std::bitset<8>(_char).to_string());
    }

    return generatedData;
}

void Encode() {

    std::string imgpath;

    std::cout << "Please enter the full filepath, with extension:" << std::endl;
    std::cin >> imgpath;

    gil::rgb8_image_t image;
    gil::read_image(imgpath, image, gil::png_tag{});

    std::string message;
    std::cout << "Please type a message to be converted into binary code" << std::endl;
    std::cin >> message;

    

    if (message.empty()) {
        throw std::invalid_argument("The message can't be nothing.");
    }


    
}

gil::rgb8_image_t EncodeImage(gil::rgb8_image_t image, std::string message) {
    //Not Implemented yet
}

std::tuple<gil::rgb8_pixel_t, gil::rgb8_pixel_t, gil::rgb8_pixel_t> ModifyPixels() {

}


int main()
{
    //gil::rgb8_image_t img;
    //gil::read_image("POP_Cover.png", img, gil::png_tag{});
    //gil::rgb8_image_t square250x250(250, 250);
    //gil::resize_view(gil::const_view(img), gil::view(square250x250), gil::bilinear_sampler{});
    //gil::write_view("out-resize.png", gil::const_view(square250x250), gil::png_tag{});

    int a;
    std::cout << ":: Welcome to Stegosaurus C++ Edition ::" << std::endl << "1. Encode \n2. Decode" << std::endl;

    std::cin >> a;

    if (a == 1) {
        std::cout << "You picked Encode!";
        Encode();
        return 0;
    } 
    else if (a == 2){
        std::cout << "You picked Decode!";
        return 0;
    }
    else {
        std::cout << "Invalid Number!";
        return 0;
    }

}