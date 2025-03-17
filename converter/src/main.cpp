#include "stb_image_write.h"
#include "textureApi.h"
#include "formats/dds/ddsSupport.h"
#include "texture.h"
#include "logging.h"
#include <filesystem>
#include <sstream>
#include <argparse/argparse.hpp>
#include <string>

namespace fs = std::filesystem;

std::string getAllPixelFormats() {
    std::stringstream ss;
    ss << "Supported pixel formats: ";
    for (uint16_t i = ePixelFormat::RGBA32; i < ePixelFormat::RGBA5551; ++i) {
        ss << getPixelFormatName(ePixelFormat(i)) << " ";
    }
    return ss.str();
}

int main(int argc, char **argv) {
    argparse::ArgumentParser program("converter");

    program.add_argument("input_filepath")
            .required()
            .help("Specify the input file path.");

    // Optional output file path (short version: -o)
    program.add_argument("-o", "--output")
            .default_value(std::string(""))
            .help("Specify the output file path (without dot). \n"
                  "If not specified input filepath will be used with .png format, if input was png, suffix will be added.\n"
                  "If not specified `--output-format` will replace output file extension.");

    // Optional input format overwrite (short version: -f)
    program.add_argument("-if", "--input-format")
            .default_value(std::string(""))
            .help("Specify the input format to overwrite. Eg you have .tga, but inside it's png file.");

    // Optional input format overwrite (short version: -f)
    program.add_argument("-of", "--output-format")
            .default_value(std::string(""))
            .help("Specify the output format to overwrite. Eg you have .tga, but but you want to write png into it.");

    // Optional pixel format (short version: -p)
    program.add_argument("-p", "--pixel-format")
            .default_value(std::string("RGBA8888"))
            .help(getAllPixelFormats());

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    auto input_filepath_arg = program.get<std::string>("input_filepath");
    auto output_filepath_arg = program.get<std::string>("--output");
    auto input_format_arg = program.get<std::string>("--input-format");
    auto output_format_arg = program.get<std::string>("--output-format");
    auto pixel_format_arg = program.get<std::string>("--pixel-format");

    fs::path output_filepath;
    std::filesystem::path input_filepath(input_filepath_arg);
    if (output_filepath_arg.empty()) {
        std::filesystem::path inputPath(input_filepath);

        if (!output_format_arg.empty()) {
            output_filepath = inputPath.replace_extension("." + output_format_arg).string();
        } else if (inputPath.extension() == ".png") {
            output_filepath = inputPath.replace_filename(inputPath.stem().string() + "_modified.png");
        } else {
            output_filepath = inputPath.replace_extension(".png");
        }
    } else {
        output_filepath = fs::path(output_filepath_arg);
    }
    if (output_format_arg.empty())
        output_format_arg = "png";

    if (input_format_arg.empty())
        input_format_arg = input_filepath.extension().string().substr(1);


    if (output_format_arg != "png" &&
        output_format_arg != "tga" &&
        output_format_arg != "hdr" &&
        output_format_arg != "dds") {
        logger(eLogLevel::ERROR, std::format("Output format \"{}\" is not supported", output_format_arg).c_str());
        return -1;
    }
    if (input_format_arg != "png" &&
        input_format_arg != "hdr" &&
        input_format_arg != "dds") {
        logger(eLogLevel::ERROR, std::format("Input format \"{}\" is not supported", input_format_arg).c_str());
        return -1;
    }

    sTexture texture;
    if (!loadDDS(input_filepath.c_str(), &texture)) {
        return -1;
    }

    sTexture cTexture;
    cTexture.m_height = texture.m_height;
    cTexture.m_width = texture.m_width;
    cTexture.m_pixelFormat = ePixelFormat::RGB16F;

    if (convertTexture(&texture, &cTexture))
        stbi_write_tga(output_filepath.string().c_str(), (int) texture.m_width, (int) texture.m_height, 4,
                       (float *) cTexture.m_rawPixelData.data());
    else
        return -1;
}