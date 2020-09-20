// Stegosaurus_CPP_Implementation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iterator>
#include <sstream>
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
/// 
/// <param name="data">
/// The typed message.
/// </param>
/// 
/// <returns>
/// A binary representation of the typed message.
/// </returns>
std::string EncodeMessage(std::string data) {
    std::vector<std::string>::iterator characterIterator;
    std::vector<std::string> generatedData;
    std::string binaryString;

    // Loops through the data to convert to a vector of 8 bit binary codes.
    for (char& _char : data) {
        generatedData.push_back(std::bitset<8>(_char).to_string());
    }

    // Loops through the vector to convert binary into string.
    for (characterIterator = generatedData.begin(); characterIterator != generatedData.end(); characterIterator++) {
        binaryString.append(*characterIterator);
    }

    // Returns the binary string
    return binaryString;
}

/// <summary>
/// Takes a binary string and converts it to ASCII text.
/// </summary>
/// 
/// <param name="data">
/// The constructed binary string extracted from the pixel values.
/// </param>
/// 
/// <returns>
/// Decoded message as string.
/// </returns>
std::string DecodeMessage(std::string data) {

    std::stringstream sstream(data);
    std::string message;

    while (sstream.good())
    {
        std::bitset<8> bits;

        // Shifts bits into the bitset.
        sstream >> bits;

        // Converts the bits to ASCII characters.
        char c = char(bits.to_ulong());

        // Constructs the characters into a message.
        message += c;
    }

    // Returns the full message.
    return message;

}

/// <summary>
/// Will change the pixel component value that has been passed to the desired
/// odd(representing binary value of '1') or even(representing binary value of '0') value.
/// </summary>
/// 
/// <param name="bitValue">
/// Is a '1' or '0' based on the binaryString index value.
/// </param>
/// 
/// <param name="pixelValue">
/// The original component value of the pixel.
/// </param>
/// 
/// <returns>
/// The new odd or even value based on the bitValue.
/// </returns>

int OddEven(int bitValue, int pixelValue) {

    int newValue;

    // If bit value is 0 and the pixel value is odd then decrement pixel component value by 1.
    if (bitValue == '0' && (pixelValue % 2 != 0)) {
        newValue = pixelValue - 1;
        return (newValue);
    }

    // If bit value is 1 and the pixel value is even then decrement pixel component value by 1.
    else if (bitValue == '1' && pixelValue % 2 == 0) {
        newValue = pixelValue - 1;
        return (newValue);
    }

    // If nothing needs to be done then return the same value.
    else {
        newValue = pixelValue;
        return newValue;
    }
}

/// <summary>
/// Will encode an rgb8_image_t pixels with the message, using the blue component as a control.
/// </summary>
/// 
/// <param name="image">
/// The original image that has been opened and passed into the rgb8_image_t variable.
/// </param>
/// 
/// <param name="message">
/// The typed message that the user wants to encode.
/// </param>
/// 
/// <returns>
/// A modified image with the message encoded within the pixel components.
/// </returns>

gil::rgb8_image_t EncodeImage(gil::rgb8_image_t image, std::string message) {

    // Gets the binary string.
    std::string binaryString = EncodeMessage(message);
    std::string::iterator binaryStringIterator;

    // See what the binary string is.
    std::cout << binaryString << std::endl;

    // Set the iterator to the beginning of the string.
    binaryStringIterator = binaryString.begin();

    // Creating a view of the image.
    gil::rgb8_view_t imageView = view(image);
    auto v = gil::view(image);
    auto b = v.begin();

    // Boolean to check for when the message has been iterated through.
    bool messageDone = false;

    while (b != v.end()) {    // While there are still pixels in the image.

        // Set the current pixel to a new pixel object.
        gil::rgb8_pixel_t pixel = *b;       
        // Set the R value appropriately.
        pixel[0] = OddEven(*binaryStringIterator, (int)pixel[0]);
        // See the binary value that was just set.
        std::cout << (int)pixel[0] % 2 << std::endl;
        // Move onto the next binary value in the string.
        std::advance(binaryStringIterator, 1);

        pixel[1] = OddEven(*binaryStringIterator, (int)pixel[1]);
        std::cout << (int)pixel[1] % 2 << std::endl;
        std::advance(binaryStringIterator, 1);

        // If the end of the message has been reached.
        if (binaryStringIterator == binaryString.end()) {

            // If the G value is even, change it to odd to indicate message is done.
            if ((int)pixel[2] % 2 == 0) {
                pixel[2] = (int)pixel[2] - 1;
                std::cout << (int)pixel[2] % 2 << std::endl;
                // Place the pixel into the image.
                *b = pixel;
                // Break out of loops.
                break;
            }
        }
        else {

            // Otherwise check if the value is odd, if it is make it even
            // to indicate that the message is not complete.
            if ((int)pixel[2] % 2 != 0) {
                std::cout << (int)pixel[2] % 2 << std::endl;
                pixel[2] = (int)pixel[2] - 1;
            }
        }

        // Place the modified pixel into the image.
        *b = pixel;
        // Increment to the next pixel.
        b++;
    }
    
    // Return the modified image.
    return image;
}

/// <summary>
/// Is the initiator method to encode the image with a message.
/// </summary>

void Encode() {

    std::string imgpath;

    // Test image: C:\Users\mwsco\source\repos\Stegosaurus_CPP_Implementation\test.png
    std::cout << "Please enter the full filepath, with extension:" << std::endl;
    // Set the image path.
    std::cin >> imgpath;

    gil::rgb8_image_t image;
    // Read the image into an image object.
    gil::read_image(imgpath, image, gil::png_tag{});

    std::string message;
    std::cout << "Please type a message to be converted into binary code" << std::endl;
    // Set the message that the user wants to encode.
    std::cin >> message;

    // If the message is empty throw error.
    if (message.empty()) {
        throw std::invalid_argument("The message can't be nothing.");
    }

    // Creates a new image object by calling the EncodeImage method to modify the pixels.
    gil::rgb8_image_t encodedImage =  EncodeImage(image, message);

    // Save modified image.
    gil::write_view("encode_out.png", view(encodedImage), gil::png_tag{});
}

/// <summary>
/// Will scan through the supplied image and construct a binary string
/// this string will then get passed to the DecodeMessage to be parsed into ASCII
/// </summary>

void Decode() {
    std::string imgpath;
    std::string data;

    // Test image: C:\Users\mwsco\source\repos\Stegosaurus_CPP_Implementation\encode_out.png
    std::cout << "Please enter the full filepath, with extension:" << std::endl;
    std::cin >> imgpath;

    gil::rgb8_image_t image;
    // Reads the image.
    gil::read_image(imgpath, image, gil::png_tag{});

    // Views the image.
    gil::rgb8_view_t imageView = view(image);
    auto v = gil::view(image);
    auto b = v.begin();

    bool messageDone = false;

    // While there are still pixels to iterate.
    while (b != v.end()) {
        // Set the current pixel to a pixel object.
        gil::rgb8_pixel_t pixel = *b;
        
        // For every R and G value.
        for (int i = 0; i < 2; i++) {
            // Determine the binary value based on whether odd('1') or even ('0')
            if ((int)pixel[i] % 2 == 0) {
                data.push_back('0');
            }
            else {
                data.push_back('1');
            }
        }

        // If the B value is odd then the message is complete.
        if ((int)pixel[2] % 2 != 0) {
            break;
        }

        // Increment to the next pixel.
        b++;
    }

    // Display the message and the binary that has been harvested from the pixel components.
    std::cout << DecodeMessage(data) << std::endl;
    std::cout << data << std::endl;
}

int main()
{

    int a;
    std::cout << ":: Welcome to Stegosaurus C++ Edition ::" << std::endl << "1. Encode \n2. Decode" << std::endl;
    // Get the option from user.
    std::cin >> a;

    // Pass off to Encode method.
    if (a == 1) {
        std::cout << "You picked Encode!" << std::endl;
        Encode();
        return 0;
    } 
    // Pass off to Decode method.
    else if (a == 2){
        std::cout << "You picked Decode!" << std::endl;
        Decode();
        return 0;
    }
    // If the user types an invalid option then end the program.
    else {
        std::cout << "Invalid Number!" << std::endl;
        return 0;
    }

}