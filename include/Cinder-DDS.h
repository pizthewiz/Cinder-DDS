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

//! Retuns a buffer containing the DDS file header and compressed data
const ci::Buffer ddsConvert(const ci::Surface8uRef surface, CompressionType type);

}}
