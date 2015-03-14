//
//  Cinder-DDS.cpp
//  Cinder-DDS
//
//  Created by Jean-Pierre Mouilleseaux on 10 Feb 2015.
//  Copyright (c) 2015 Chorded Constructions. All rights reserved.
//

#include "Cinder-DDS.h"

extern "C" {
    #include "image_DXT.h"
    #include "image_helper.h"
}

using namespace ci;

namespace Cinder { namespace DDS {

const unsigned char* DXTCompress(const ci::Surface8uRef& surface, CompressionFormat format, int* length) {
    unsigned char* source = surface->getData();
    int width = surface->getWidth();
    int height = surface->getHeight();
    int channels = surface->getPixelInc();
    int channelOrder = surface->getChannelOrder().getCode();
    unsigned char* destination = NULL;

    // compress
    switch (format) {
        case CompressionFormat::DXT1:
            destination = convert_image_to_DXT1(source, width, height, channels, length);
            break;
        case CompressionFormat::DXT5:
            destination = convert_image_to_DXT5(source, width, height, channels, length);
            break;
        case CompressionFormat::YCoCg_DXT5:
            // NB - requires RGB or RGBA
            if (channelOrder != SurfaceChannelOrder::RGB && channelOrder != SurfaceChannelOrder::RGBA) {
                return NULL;
            }
            convert_RGB_to_YCoCg(source, width, height, channels);
            destination = convert_image_to_DXT5(source, width, height, channels, length);
            break;
    }

    return destination;
}

const ci::Buffer DDSConvert(const ci::Surface8uRef& surface, CompressionFormat format) {
    int length = 0;
    const unsigned char* data = DXTCompress(surface, format, &length);
    if (!data) {
        return NULL;
    }

    // see https://msdn.microsoft.com/en-us/library/bb943982(v=vs.85).aspx
    DDS_header* header = (DDS_header*)calloc(1, sizeof(DDS_header));
    if (!header) {
        return NULL;
    }
    header->dwMagic = ('D' << 0) | ('D' << 8) | ('S' << 16) | (' ' << 24);
    header->dwSize = 124;
    header->dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_LINEARSIZE;
    header->dwHeight = surface->getHeight();
    header->dwWidth = surface->getWidth();
    header->dwPitchOrLinearSize = length;
    header->sPixelFormat.dwSize = 32;
    header->sPixelFormat.dwFlags = DDPF_FOURCC;
    switch (format) {
        case CompressionFormat::DXT1:
            header->sPixelFormat.dwFourCC = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('1' << 24);
            break;
        case CompressionFormat::DXT5:
        case CompressionFormat::YCoCg_DXT5:
            header->sPixelFormat.dwFourCC = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('5' << 24);
            break;
    }
    header->sCaps.dwCaps1 = DDSCAPS_TEXTURE;

    Buffer buffer(sizeof(DDS_header) + length);
    memcpy((unsigned char*)buffer.getData() + 0, header, sizeof(DDS_header));
    memcpy((unsigned char*)buffer.getData() + sizeof(DDS_header), data, length);

    free((void*)data);
    data = NULL;

    return buffer;
}

}}
