// Stegosaurus_CPP_Implementation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/numeric/sampler.hpp>
#include <boost/gil/extension/numeric/resample.hpp>
#include <boost/coroutine2/all.hpp>

#include <bitset>

namespace gil = boost::gil;

std::vector<std::string> GenerateData(std::string data) {

    std::vector<std::string> generatedData;

    for (char& _char : data) {
        generatedData.push_back(std::bitset<8>(_char).to_string());
    }

    return generatedData;
}

gil::rgb8_image_t EncodeImage(gil::rgb8_image_t image, std::string message) {

    std::vector<std::string> binaryMessage = GenerateData(message);

    std::vector<std::string>::iterator it;

    for (it = binaryMessage.begin(); it < binaryMessage.end(); it++) {
        std::cout << *it << std::endl;
    }

    return image;
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

    EncodeImage(image, message);
}



//std::tuple<gil::rgb8_pixel_t, gil::rgb8_pixel_t, gil::rgb8_pixel_t> ModifyPixels(gil::rgb8_image_t image, std::string message) {
    //Not Implemented Yet
//}


int main()
{
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