//
//  Cinder-DDS.h
//  Cinder-DDS
//
//  Created by Jean-Pierre Mouilleseaux on 10 Feb 2015.
//  Copyright (c) 2015 Chorded Constructions. All rights reserved.
//

#pragma once

#include "cinder/Cinder.h"

namespace Cinder { namespace DDS {

enum class CompressionType {DXT1, DXT5};

//! Retuns an owned pointer to the DXT compressed data and provides its length in \a length
const unsigned char* DXTCompress(const ci::Surface8uRef& surface, CompressionType type, int* length);

//! Retuns a buffer containing the DDS file header and DXT compressed data
const ci::Buffer DDSConvert(const ci::Surface8uRef& surface, CompressionType type);

}}
