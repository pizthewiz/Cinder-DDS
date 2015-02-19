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
}

using namespace ci;

namespace Cinder { namespace DDS {

const ci::Buffer ddsConvert(const ci::Surface8uRef surface, CompressionType type) {
    SurfaceChannelOrder channelOrder = SurfaceChannelOrder::UNSPECIFIED;
    switch (type) {
        case CompressionType::DXT1:
            channelOrder = SurfaceChannelOrder::RGB;
            break;
        case CompressionType::DXT5:
            channelOrder = SurfaceChannelOrder::RGBA;
            break;
        default:
            assert(false);
    }

    // create intermediate surface if the channels are not in the necessary order
    Surface8uRef intermediateSurface = nullptr;
    if (surface->getChannelOrder().getCode() != channelOrder.getCode()) {
        int32_t rowBytes = 0;
        switch (type) {
            case CompressionType::DXT1:
                rowBytes = 3;
                break;
            case CompressionType::DXT5:
                rowBytes = 4;
                break;
            default:
                assert(false);
        }

        int32_t width = surface->getWidth();
        int32_t height = surface->getHeight();
        size_t dataSize = width * height * rowBytes;
        unsigned char* intermediate = (unsigned char*)malloc(dataSize);
        if (!intermediate) {
            return NULL;
        }
        size_t offset = 0;

        Surface::Iter iter = surface->getIter();
        while (iter.line()) {
            while (iter.pixel()) {
                intermediate[offset++] = iter.r();
                intermediate[offset++] = iter.g();
                intermediate[offset++] = iter.b();
                if (type == CompressionType::DXT5) {
                    intermediate[offset++] = surface->hasAlpha() ? iter.a() : 255;
                }
            }
        }

        intermediateSurface = Surface::create(intermediate, width, height, rowBytes, channelOrder);
    }

    Surface8uRef s = intermediateSurface ? intermediateSurface : surface;
    unsigned char* source = s->getData();
    int width = s->getWidth();
    int height = s->getHeight();
    unsigned char* destination = NULL;
    int dataSize = 0;

    // compress
    switch (type) {
        case CompressionType::DXT1:
            destination = convert_image_to_DXT1(source, width, height, 3, &dataSize);
            break;
        case CompressionType::DXT5:
            destination = convert_image_to_DXT5(source, width, height, 4, &dataSize);
            break;
        default:
            assert(false);
    }

    // release buffer backing intermediate surface
    if (intermediateSurface) {
        free(intermediateSurface->getData());
        intermediateSurface = nullptr;
    }

    if (!destination) {
        return NULL;
    }

    DDS_header* header = (DDS_header*)calloc(1, sizeof(DDS_header));
    if (!header) {
        return NULL;
    }
    header->dwMagic = ('D' << 0) | ('D' << 8) | ('S' << 16) | (' ' << 24);
    header->dwSize = 124;
    header->dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_LINEARSIZE;
    header->dwHeight = width;
    header->dwWidth = height;
    header->dwPitchOrLinearSize = dataSize;
    header->sPixelFormat.dwSize = 32;
    header->sPixelFormat.dwFlags = DDPF_FOURCC;
    switch (type) {
        case CompressionType::DXT1:
            header->sPixelFormat.dwFourCC = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('1' << 24);
            break;
        case CompressionType::DXT5:
            header->sPixelFormat.dwFourCC = ('D' << 0) | ('X' << 8) | ('T' << 16) | ('5' << 24);
            break;
        default:
            assert(false);
    }
    header->sCaps.dwCaps1 = DDSCAPS_TEXTURE;

    Buffer buffer(sizeof(DDS_header) + dataSize);
    memcpy((unsigned char*)buffer.getData() + 0, header, sizeof(DDS_header));
    memcpy((unsigned char*)buffer.getData() + sizeof(DDS_header), destination, dataSize);

    free(destination);
    destination = NULL;

    return buffer;
}

}}
