//
//  ImageCompressorApp.cpp
//  ImageCompressor
//
//  Created by Jean-Pierre Mouilleseaux on 16 Mar 2015.
//  Copyright 2015 Chorded Constructions. All rights reserved.
//

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "Cinder-DDS.h"

using namespace ci;
using namespace ci::app;
using namespace Cinder::DDS;
using namespace std;

static const std::string sVertexShaderPassThrough = R"(
    #version 150
    uniform mat4 ciModelViewProjection;

    in vec4 ciPosition;
    in vec2	ciTexCoord0;

    out vec2 vTexCoord0;

    void main() {
        vTexCoord0 = ciTexCoord0;
        gl_Position = ciModelViewProjection * ciPosition;
    }
)";

static const std::string sFragmentShaderColorSpaceConversion = R"(
    #version 150
    uniform sampler2D image;
    uniform bool hasAlpha;

    in vec2 vTexCoord0;

    out vec4 oFragColor;

    void main() {
        vec4 color = texture(image, vTexCoord0);

        // component encoding and order differs on presence of alpha
        float y, co, cg, a;
        if (hasAlpha) {
            co = color.x - (0.5 * 256.0 / 255.0);
            cg = color.y - (0.5 * 256.0 / 255.0);
            a = color.z;
            y = color.w;
        } else {
            co = color.x - (0.5 * 256.0 / 255.0);
            y = color.y;
            cg = color.z - (0.5 * 256.0 / 255.0);
            a = 1.0;
        }
        float r = y + co - cg;
        float g = y + cg;
        float b = y - co - cg;
        vec4 result = vec4(r, g, b, a);

        oFragColor = result;
    }
)";

class ImageCompressorApp : public App {
public:
	void setup() override;
	void draw() override;

private:
    Surface8uRef mSurface;
    gl::Texture2dRef mSourceTexture;
    gl::Texture2dRef mResultTexture;
    ci::gl::FboRef mFBO;
    ci::gl::GlslProgRef mShader;
};

void ImageCompressorApp::setup() {
    // grab the source
    try {
        mSurface = Surface::create(loadImage(loadAsset("grad-cutout.png")));
    } catch (ci::Exception& e) {
        CI_LOG_EXCEPTION("unable to create surface", e);
        quit();
    }
    mSourceTexture = gl::Texture::create(*mSurface);

    // convert to a DXT compressed DDS file buffer
    CompressionFormat format = CompressionFormat::YCoCg_DXT5;
    const Buffer buffer = DDSConvert(mSurface, format);

    // create a texture from DDS file buffer
    try {
        mResultTexture = gl::Texture2d::createFromDds(DataSourceBuffer::create(buffer));
    } catch (ci::Exception& e) {
        CI_LOG_EXCEPTION("failed to create texture from DDS file", e);
        quit();
    }

    // handle color space conversion if using YCoCg_DXT5
    if (format == CompressionFormat::YCoCg_DXT5) {
        // create FBO
        mFBO = gl::Fbo::create(mResultTexture->getWidth(), mResultTexture->getHeight());

        // create color space conversion shader
        try {
            mShader = gl::GlslProg::create(sVertexShaderPassThrough, sFragmentShaderColorSpaceConversion);
        } catch (gl::GlslProgCompileExc& e) {
            CI_LOG_EXCEPTION("failed to compile shader", e);
            quit();
        } catch (ci::Exception& e) {
            CI_LOG_EXCEPTION("failed to create shader", e);
            quit();
        }

        // run texture through shader into FBO
        gl::ScopedMatrices matricies;
        gl::ScopedViewport viewport(ivec2(0), mFBO->getSize());
        gl::ScopedFramebuffer fbo(mFBO);

        gl::setMatricesWindow(mFBO->getSize());
        gl::color(Color::white());

        gl::ScopedTextureBind texture(mResultTexture);
        gl::ScopedGlslProg shader(mShader);

        mShader->uniform("image", 0);
        mShader->uniform("hasAlpha", mSurface->hasAlpha());
        gl::drawSolidRect(mResultTexture->getBounds());

        // pull texture out of the FBO
        mResultTexture = mFBO->getColorTexture();
    }

    // denote the texture as vertically flipped
    mResultTexture->setTopDown();

    if (mSurface->hasAlpha()) {
        gl::enableAlphaBlending();
    }
}

void ImageCompressorApp::draw() {
    gl::clear();

    if (!mSourceTexture) {
        return;
    }
    gl::draw(mSourceTexture);

    if (!mResultTexture) {
        return;
    }
    gl::pushMatrices(); {
        gl::translate(mSourceTexture->getWidth(), 0);
        gl::draw(mResultTexture);
    } gl::popMatrices();
}

CINDER_APP(ImageCompressorApp, RendererGl, [](App::Settings* settings) {
    settings->setHighDensityDisplayEnabled();
    settings->prepareWindow(Window::Format().size(1024, 512));
})
