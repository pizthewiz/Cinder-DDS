# Cinder-DDS
`Cinder-DDS` is a [CinderBlock](http://libcinder.org/docs/welcome/CinderBlocks.html) to compress images with the lossy DXT1, DXT5 and YCoCg-DXT5 formats and wrap them in a DDS file container.

[S3 Texture Compression](http://en.wikipedia.org/wiki/S3_Texture_Compression) (S3TC or sometimes referred to as _DirectX Texture Compression_, DXTC) is a fixed-rate lossy image compression format supported by many GPUs through the [`GL_EXT_texture_compression_s3tc`](https://www.opengl.org/registry/specs/EXT/texture_compression_s3tc.txt) OpenGL extension - this allows the compressed image to be transferred directly to the GPU and then decompressed in hardware rather than a more costly decompression on the CPU followed by a higher bandwidth transfer from CPU to GPU.

The YCoCg-DXT5 compression format DXT5 compresses image data in the [YCoCg color space](http://en.wikipedia.org/wiki/YCgCo), which generally yields higher quality than RGB - the format was proposed by J.M.P. van Waveren and Ignacio Castaño in the paper [_Real-Time YCoCg-DXT Compression_](http://www.nvidia.com/object/real-time-ycocg-dxt-compression.html). It is worth noting that the compressed artifact remains in the YCoCg color space, so a fragment shader might be necessary to bring it back into RGB.

[DirectDraw Surface](http://en.wikipedia.org/wiki/DirectDraw_Surface) (DDS) is an image file container often used for DXT compressed images. It was created by Microsoft, is well supported on Windows in both creation and loading though importantly Cinder can create a texture from a DDS file on non-Windows platforms.

*NOTE* - `Cinder-DDS` is to be used with the upcoming 0.9.0 Cinder release.

### USAGE
Convert PNG to DXT5 compressed DDS file and create texture from it
```C++
auto surface = Surface::create(loadImage(loadAsset("pulsar.png")));
auto buffer = DDSConvert(surface, CompressionFormat::DXT5);
auto texture = gl::Texture2d::createFromDds(DataSourceBuffer::create(buffer));
```

### LIBRARIES
There are quite a few third-party libraries that offer DXT compression:
- [RygsDXTc](https://github.com/Cyan4973/RygsDXTc)
- [SOIL2](https://bitbucket.org/SpartanJ/soil2)
- [gimp-dds](https://code.google.com/p/gimp-dds/)
- [NVIDIA Texture Tools](https://code.google.com/p/nvidia-texture-tools/) (NVTT)
- [libsquish](https://code.google.com/p/libsquish/)
- [crunch](https://code.google.com/p/crunch/)

Some support numerous format variations and even perform additional encoding and compression to further reduce the in-memory size - the GDC 2012 talk [DXT is NOT ENOUGH! Advanced texture compression for games](https://www.youtube.com/watch?v=7bJ-D1xXEeg) goes into more detail for those interested. `Cinder-DDS` uses [SOIL2](https://bitbucket.org/SpartanJ/soil2).
