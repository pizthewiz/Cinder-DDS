# Cinder-DDS
`Cinder-DDS` is a [CinderBlock](http://libcinder.org/docs/welcome/CinderBlocks.html) to compress images with the lossy DXT1 and DXT5 formats and wrap them in a DDS file container.

[DirectX Texture Compression](http://en.wikipedia.org/wiki/S3_Texture_Compression) (DXTc or more properly S3 Texture Compression, S3TC) is a fixed-rate lossy image compression format supported by many GPUs via the [`GL_EXT_texture_compression_s3tc`](https://www.opengl.org/registry/specs/EXT/texture_compression_s3tc.txt) extension - this allows the compressed image to be transferred directly to the GPU and then decompressed in hardware rather than the more costly transfer of a decompressed image from CPU to GPU.

[DirectDraw Surface](http://en.wikipedia.org/wiki/DirectDraw_Surface) (DDS) is an image file container often used for DXT compressed images. It was created by Microsoft, is well supported on Windows in both creation and loading though importantly Cinder can create a texture from a DDS file on non-Windows platforms.

*NOTE* - `Cinder-DDS` is to be used with the heir apparent [glNext](https://github.com/cinder/Cinder/tree/glNext) Cinder branch.

### USAGE
Grab the project and its dependencies:
```sh
$ git clone --recursive git@github.com:pizthewiz/Cinder-DDS.git
```

```C++
// stuff
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
