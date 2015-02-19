# Cinder-DDS
`Cinder-DDS` is a [CinderBlock](http://libcinder.org/docs/welcome/CinderBlocks.html) to compress images with the lossy DXT1 and DXT5 formats and create DDS files.

[S3 Texture Compression](http://en.wikipedia.org/wiki/S3_Texture_Compression) (of which DXT1 and DXT5 are two of several variations) is a lossy compression format which many desktop GPUs support via the [`GL_EXT_texture_compression_s3tc`](https://www.opengl.org/registry/specs/EXT/texture_compression_s3tc.txt) extension - this allows the compressed image to be transferred directly to the GPU and then decompressed in hardware rather than the more costly transfer of a decompressed image from CPU to GPU.

Does *not* currently support mipmap generation, DXT2, DXT3, DXT4 and many other things.

*NOTE* - `Cinder-DDS` is to be used with the heir apparent [glNext](https://github.com/cinder/Cinder/tree/glNext) Cinder branch.

Several third-party libraries do the actual work:
- [RygsDXTc](https://github.com/Cyan4973/RygsDXTc)
- [SOIL2](https://bitbucket.org/SpartanJ/soil2)
- [gimp-dds](https://code.google.com/p/gimp-dds/)

### USAGE
Grab the project and its dependencies:
```sh
$ git clone --recursive git@github.com:pizthewiz/Cinder-DDS.git
```

```C++
// stuff
```
