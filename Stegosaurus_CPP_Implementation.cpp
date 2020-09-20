// Stegosaurus_CPP_Implementation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iterator>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/numeric/sampler.hpp>
#include <boost/gil/extension/numeric/resample.hpp>
#include <boost/coroutine2/all.hpp>
#include "interleaved_ptr.hpp"

#include <bitset>

namespace gil = boost::gil;

/// <summary>
/// This method will take a string and convert it into a vector of binary characters.
/// </summary>
/// <param name="data"></param>
/// <returns>
/// Vector of binary equivelent strings for each character.
/// </returns>
std::vector<std::string> GenerateData(std::string data) {

    std::vector<std::string> generatedData;

    for (char& _char : data) {
        generatedData.push_back(std::bitset<8>(_char).to_string());
    }

    return generatedData;
}

int OddEven(int bitValue, int pixelValue) {

    int newValue;

    if (bitValue == '0' && (pixelValue % 2 != 0)) {
        newValue = pixelValue - 1;
        return (newValue);
    }
    else if (bitValue == '1' && pixelValue % 2 == 0) {
        newValue = pixelValue - 1;
        return (newValue);
    }
    else {
        newValue = pixelValue;
        return newValue;
    }
}

gil::rgb8_image_t EncodeImage(gil::rgb8_image_t image, std::string message) {

    std::vector<std::string> binaryMessage = GenerateData(message);
    std::vector<std::string>::iterator characterIterator;

    std::string binaryString;
    std::string::iterator binaryStringIterator;

    for (characterIterator = binaryMessage.begin(); characterIterator != binaryMessage.end(); characterIterator++) {
        binaryString.append(*characterIterator);
    }

    std::cout << binaryString << std::endl;

    binaryStringIterator = binaryString.begin();

    // Creating a view of the image
    gil::rgb8_view_t imageView = view(image);
    auto v = gil::view(image);
    auto b = v.begin();

    // Boolean to check for when the message has been iterated through
    bool messageDone = false;

    while (b != v.end()) {
        gil::rgb8_pixel_t pixel = *b;
        //std::cout << (int)pixel[0] << ", " << (int)pixel[1] << ", " << (int)pixel[2] << std::endl;
        
        if (binaryStringIterator == binaryString.end() - 2) {
            // If True the loop will break out
            messageDone = true;

            pixel[0] = OddEven(*binaryStringIterator, (int)pixel[0]);
            std::cout << (int)pixel[0] << std::endl;
            std::advance(binaryStringIterator, 1);

            pixel[1] = OddEven(*binaryStringIterator, (int)pixel[1]);
            std::cout << (int)pixel[1] << std::endl;
            std::advance(binaryStringIterator, 1);

            if ((int)pixel[2] % 2 == 0) {
                pixel[2] = (int)pixel[2] - 1;
            }

            break;
        }

        else if (!messageDone) {
            if ((b - v.begin()) % 3 == 0 && b != v.begin()) {

                pixel[0] = OddEven(*binaryStringIterator, (int)pixel[0]);
                std::cout << (int)pixel[0] << std::endl;
                std::advance(binaryStringIterator, 1);
                
                pixel[1] = OddEven(*binaryStringIterator, (int)pixel[1]);
                std::cout << (int)pixel[1] << std::endl;
                std::advance(binaryStringIterator, 1);

                if ((int)pixel[2] % 2 != 0) {
                    std::cout << "Moving to next 3 pixels" << std::endl;
                    pixel[2] = (int)pixel[2] - 1;
                }

            }
            else {


                for (int i = 0; i < 3; i++) {
                    pixel[i] = OddEven(*binaryStringIterator, (int)pixel[i]);
                    std::cout << (int)pixel[i] << std::endl;
                    std::advance(binaryStringIterator, 1);
                }
            }
        }

        *b = pixel;

        b++;
    }
    
    return image;
}

void Encode() {

    std::string imgpath;

    // Test image: C:\Users\mwsco\source\repos\Stegosaurus_CPP_Implementation\test.png
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

    gil::rgb8_image_t encodedImage =  EncodeImage(image, message);

    gil::write_view("encode_out.png", view(encodedImage), gil::png_tag{});
}


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