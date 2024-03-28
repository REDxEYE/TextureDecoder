#pragma once

#include <cstdint>
#include <cstdlib>
#include "enums.h"

// Original structs come from https://github.com/panzi/VTFLib/blob/master/src/VTFFormat.h

#ifdef _MSC_VER
#	define PYLIB_ALIGN16 __declspec(align(16))
#else
#	define PYLIB_ALIGN16 __attribute__((aligned(16)))
#endif

struct VTFFileHeader {
    char ident[4];                    //!< "Magic number" identifier- "VTF\0".
    uint32_t version[2];                        //!< Version[0].version[1] (currently 7.2)
    uint32_t header_size;                        //!< Size of the header struct (currently 80 bytes)
};

#pragma pack (push, 1)
struct VTFHeader_70 : public VTFFileHeader {
    uint16_t width;                            //!< Width of the largest image
    uint16_t height;                            //!< Height of the largest image
    uint32_t flags;                            //!< Flags for the image
    uint16_t frames;                            //!< Number of frames if animated (1 for no animation)
    uint16_t start_frame;                        //!< Start frame (always 0)
    uint32_t padding0;                    //!< Reflectivity padding (16 byte alignment)
    float reflectivity[3];                //!< Reflectivity vector
    uint32_t padding1;                    //!< Reflectivity padding (8 byte packing)
    float bump_scale;                        //!< Bump map scale
    VTFImageFormat image_format;                    //!< Image format index
    uint8_t mip_count;                        //!< Number of MIP levels (including the largest image)
    VTFImageFormat low_res_image_format;                //!< Image format of the thumbnail image
    uint8_t low_res_image_width;                //!< Thumbnail image width
    uint8_t low_res_image_height;                //!< Thumbnail image height
};

struct PYLIB_ALIGN16 VTFHeader_70A : public VTFHeader_70 {
};

struct VTFHeader_72 : public VTFHeader_70 {
    uint16_t depth;                            //!< Depth of the largest image
};

struct PYLIB_ALIGN16 VTFHeader_72A : public VTFHeader_72 {
};

struct VTFHeader_73 : public VTFHeader_72 {
    uint8_t padding2[3];
    uint32_t resource_count;                            //!< Number of image resources
};

struct PYLIB_ALIGN16 VTFHeader_73A : public VTFHeader_73 {
};

struct VTFResource {
    union {
        uint32_t type;
        struct {
            uint8_t ident[3];    //!< Unique resource ID
            uint8_t flags;    //!< Resource flags
        } S;                //!< ISO C++ requires struct name
    };
    uint32_t data;    //!< Resource data (e.g. for a  CRC) or offset from start of the file
};
#pragma pack(pop)

class VTFFile {
    uint8_t *m_data;
    size_t m_data_size;
public:
    explicit VTFFile() = delete;

    explicit VTFFile(uint8_t *data, size_t data_size);

    bool valid() {
        return m_data != nullptr && m_data_size > 12;
    }

    uint32_t version_major() {
        return header()->version[0];
    }

    uint32_t version_minor() {
        return header()->version[1];
    }

    uint32_t width() {
        if (version_major() == 7 && version_minor() >= 0)
            return header70()->width;
        return 0;
    }

    VTFImageFormat image_format() {
        if (version_major() == 7 && version_minor() >= 0)
            return header70()->image_format;
        return VTFImageFormat::NONE;
    }

    uint32_t height() {
        if (version_major() == 7 && version_minor() >= 0)
            return header70()->height;
        return 0;
    }

    uint16_t depth() {
        if (version_major() == 7 && version_minor() >= 2)
            return header72()->depth;
        return 1;
    }

    uint32_t resource_count() {
        if (version_major() == 7 && version_minor() >= 3)
            return header73()->resource_count;
        return 0;
    }

    VTFResource *resource(int32_t resource_id);

    VTFResource *get_resource(uint32_t resource_ident);

    uint8_t *get_highres_image(size_t &buffer_size);

    VTFFileHeader *header() {
        return (VTFFileHeader *) m_data;
    }

    VTFHeader_70 *header70() {
        return (VTFHeader_70 *) m_data;
    }

    VTFHeader_72 *header72() {
        return (VTFHeader_72 *) m_data;
    }

    VTFHeader_73 *header73() {
        return (VTFHeader_73 *) m_data;
    }


};
