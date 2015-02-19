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

const unsigned char* dxtCompress(const ci::Surface8uRef surface, CompressionType type, int* size);

}}
