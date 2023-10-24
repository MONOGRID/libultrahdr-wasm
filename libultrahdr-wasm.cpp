#include <emscripten/bind.h>
#include <iostream>
// #include <fstream>
// #include "ultrahdr/ultrahdrcommon.h"
// #include "ultrahdr/gainmapmath.h"
#include "ultrahdr/jpegr.h"
#include "ultrahdr/jpegrutils.h"

using namespace emscripten;
using namespace ultrahdr;

// Wrapper to describe the input type
typedef enum
{
  YCbCr_p010 = 0,
  YCbCr_420 = 1,
} UhdrInputFormat;

/**
 * Wrapper class for raw resource
 * Sample usage:
 *   UhdrUnCompressedStructWrapper rawImg(width, height, YCbCr_p010);
 *   rawImg.setImageColorGamut(colorGamut));
 *   rawImg.setImageStride(strideLuma, strideChroma); // optional
 *   rawImg.setChromaMode(false); // optional
 *   rawImg.allocateMemory();
 *   rawImg.loadRawResource(kYCbCrP010FileName);
 */
class UhdrUnCompressedStructWrapper
{
public:
  UhdrUnCompressedStructWrapper(size_t width, size_t height, UhdrInputFormat format);
  ~UhdrUnCompressedStructWrapper() = default;

  bool setChromaMode(bool isChromaContiguous);
  bool setImageStride(size_t lumaStride, size_t chromaStride);
  bool setImageColorGamut(ultrahdr_color_gamut colorGamut);
  bool allocateMemory();
  // bool loadRawResource(const char* fileName);
  jr_uncompressed_ptr getImageHandle();

private:
  std::unique_ptr<uint8_t[]> mLumaData;
  std::unique_ptr<uint8_t[]> mChromaData;
  jpegr_uncompressed_struct mImg;
  UhdrInputFormat mFormat;
  bool mIsChromaContiguous;
};

// UhdrUnCompressedStructWrapper::UhdrUnCompressedStructWrapper(size_t width, size_t height,
//                                                              UhdrInputFormat format)
// {
//   mImg.data = nullptr;
//   mImg.width = width;
//   mImg.height = height;
//   mImg.colorGamut = ULTRAHDR_COLORGAMUT_UNSPECIFIED;
//   mImg.chroma_data = nullptr;
//   mImg.luma_stride = 0;
//   mImg.chroma_stride = 0;
//   mFormat = format;
//   mIsChromaContiguous = true;
// }

// bool UhdrUnCompressedStructWrapper::setChromaMode(bool isChromaContiguous)
// {
//   if (mLumaData.get() != nullptr)
//   {
//     std::cerr << "Object has sailed, no further modifications are allowed" << std::endl;
//     return false;
//   }
//   mIsChromaContiguous = isChromaContiguous;
//   return true;
// }

// bool UhdrUnCompressedStructWrapper::setImageStride(size_t lumaStride, size_t chromaStride)
// {
//   if (mLumaData.get() != nullptr)
//   {
//     std::cerr << "Object has sailed, no further modifications are allowed" << std::endl;
//     return false;
//   }
//   if (lumaStride != 0)
//   {
//     if (lumaStride < mImg.width)
//     {
//       std::cerr << "Bad luma stride received" << std::endl;
//       return false;
//     }
//     mImg.luma_stride = lumaStride;
//   }
//   if (chromaStride != 0)
//   {
//     if (mFormat == YCbCr_p010 && chromaStride < mImg.width)
//     {
//       std::cerr << "Bad chroma stride received for format YCbCrP010" << std::endl;
//       return false;
//     }
//     if (mFormat == YCbCr_420 && chromaStride < (mImg.width >> 1))
//     {
//       std::cerr << "Bad chroma stride received for format YCbCr420" << std::endl;
//       return false;
//     }
//     mImg.chroma_stride = chromaStride;
//   }
//   return true;
// }

// bool UhdrUnCompressedStructWrapper::setImageColorGamut(ultrahdr_color_gamut colorGamut)
// {
//   if (mLumaData.get() != nullptr)
//   {
//     std::cerr << "Object has sailed, no further modifications are allowed" << std::endl;
//     return false;
//   }
//   mImg.colorGamut = colorGamut;
//   return true;
// }

// bool UhdrUnCompressedStructWrapper::allocateMemory()
// {
//   if (mImg.width == 0 || (mImg.width % 2 != 0) || mImg.height == 0 || (mImg.height % 2 != 0) ||
//       (mFormat != YCbCr_p010 && mFormat != YCbCr_420))
//   {
//     std::cerr << "Object in bad state, mem alloc failed" << std::endl;
//     return false;
//   }
//   int lumaStride = mImg.luma_stride == 0 ? mImg.width : mImg.luma_stride;
//   int lumaSize = lumaStride * mImg.height * (mFormat == YCbCr_p010 ? 2 : 1);
//   int chromaSize = (mImg.height >> 1) * (mFormat == YCbCr_p010 ? 2 : 1);
//   if (mIsChromaContiguous)
//   {
//     chromaSize *= lumaStride;
//   }
//   else
//   {
//     if (mImg.chroma_stride == 0)
//     {
//       std::cerr << "Object in bad state, mem alloc failed" << std::endl;
//       return false;
//     }
//     if (mFormat == YCbCr_p010)
//     {
//       chromaSize *= mImg.chroma_stride;
//     }
//     else
//     {
//       chromaSize *= (mImg.chroma_stride * 2);
//     }
//   }
//   if (mIsChromaContiguous)
//   {
//     mLumaData = std::make_unique<uint8_t[]>(lumaSize + chromaSize);
//     mImg.data = mLumaData.get();
//     mImg.chroma_data = nullptr;
//   }
//   else
//   {
//     mLumaData = std::make_unique<uint8_t[]>(lumaSize);
//     mImg.data = mLumaData.get();
//     mChromaData = std::make_unique<uint8_t[]>(chromaSize);
//     mImg.chroma_data = mChromaData.get();
//   }
//   return true;
// }

// bool UhdrUnCompressedStructWrapper::loadRawResource(const char* fileName) {
//   if (!mImg.data) {
//     std::cerr << "memory is not allocated, read not possible" << std::endl;
//     return false;
//   }
//   std::ifstream ifd(fileName, std::ios::binary | std::ios::ate);
//   if (ifd.good()) {
//     int bpp = mFormat == YCbCr_p010 ? 2 : 1;
//     int size = ifd.tellg();
//     int length = mImg.width * mImg.height * bpp * 3 / 2; // 2x2 subsampling
//     if (size < length) {
//       std::cerr << "requested to read " << length << " bytes from file : " << fileName
//                 << ", file contains only " << length << " bytes" << std::endl;
//       return false;
//     }
//     ifd.seekg(0, std::ios::beg);
//     size_t lumaStride = mImg.luma_stride == 0 ? mImg.width : mImg.luma_stride;
//     char* mem = static_cast<char*>(mImg.data);
//     for (size_t i = 0; i < mImg.height; i++) {
//       ifd.read(mem, mImg.width * bpp);
//       mem += lumaStride * bpp;
//     }
//     if (!mIsChromaContiguous) {
//       mem = static_cast<char*>(mImg.chroma_data);
//     }
//     size_t chromaStride;
//     if (mIsChromaContiguous) {
//       chromaStride = mFormat == YCbCr_p010 ? lumaStride : lumaStride / 2;
//     } else {
//       if (mFormat == YCbCr_p010) {
//         chromaStride = mImg.chroma_stride == 0 ? lumaStride : mImg.chroma_stride;
//       } else {
//         chromaStride = mImg.chroma_stride == 0 ? (lumaStride / 2) : mImg.chroma_stride;
//       }
//     }
//     if (mFormat == YCbCr_p010) {
//       for (size_t i = 0; i < mImg.height / 2; i++) {
//         ifd.read(mem, mImg.width * 2);
//         mem += chromaStride * 2;
//       }
//     } else {
//       for (size_t i = 0; i < mImg.height / 2; i++) {
//         ifd.read(mem, (mImg.width / 2));
//         mem += chromaStride;
//       }
//       for (size_t i = 0; i < mImg.height / 2; i++) {
//         ifd.read(mem, (mImg.width / 2));
//         mem += chromaStride;
//       }
//     }
//     return true;
//   }
//   std::cerr << "unable to open file : " << fileName << std::endl;
//   return false;
// }

// jr_uncompressed_ptr UhdrUnCompressedStructWrapper::getImageHandle()
// {
//   return &mImg;
// }

/**
 * Wrapper class for compressed resource
 * Sample usage:
 *   UhdrCompressedStructWrapper jpgImg(width, height);
 *   rawImg.allocateMemory();
 */
class UhdrCompressedStructWrapper
{
public:
  UhdrCompressedStructWrapper(size_t width, size_t height);
  ~UhdrCompressedStructWrapper() = default;

  bool allocateMemory();
  jr_compressed_ptr getImageHandle();

private:
  std::unique_ptr<uint8_t[]> mData;
  jpegr_compressed_struct mImg{};
  size_t mWidth;
  size_t mHeight;
};

UhdrCompressedStructWrapper::UhdrCompressedStructWrapper(size_t width, size_t height)
{
  mWidth = width;
  mHeight = height;
}

bool UhdrCompressedStructWrapper::allocateMemory()
{
  if (mWidth == 0 || (mWidth % 2 != 0) || mHeight == 0 || (mHeight % 2 != 0))
  {
    std::cerr << "Object in bad state, mem alloc failed" << std::endl;
    return false;
  }
  int maxLength = std::max(8 * 1024 /* min size 8kb */, (int)(mWidth * mHeight * 3 * 2));
  mData = std::make_unique<uint8_t[]>(maxLength);
  mImg.data = mData.get();
  mImg.length = 0;
  mImg.maxLength = maxLength;
  return true;
}

jr_compressed_ptr UhdrCompressedStructWrapper::getImageHandle()
{
  return &mImg;
}

/*
 * appendGainMap

 * @param width width of the final image
 * @param height height of the final image
 */
auto appendGainMap(int width, int height,
                   std::string sdr, int sdrSize,
                   std::string gainmap, int gainmapSize,
                   float maxContentBoost, float minContentBoost,
                   float gamma, float offsetSdr, float offsetHdr,
                   float hdrCapacityMin, float hdrCapacityMax)
{
  const uint8_t *sdrPtr = reinterpret_cast<const uint8_t *>(sdr.c_str());
  const uint8_t *gainmapPtr = reinterpret_cast<const uint8_t *>(gainmap.c_str());

  jpegr_compressed_struct jpgSdr = {
      .data = (void *)sdrPtr,
      .length = sdrSize,
      .maxLength = sdrSize,
      .colorGamut = ULTRAHDR_COLORGAMUT_UNSPECIFIED};

  jpegr_compressed_struct jpgGainmap = {
      .data = (void *)gainmapPtr,
      .length = gainmapSize,
      .maxLength = gainmapSize,
      .colorGamut = ULTRAHDR_COLORGAMUT_UNSPECIFIED};

  ultrahdr_metadata_struct meta = {
      .version = "1.0",
      .maxContentBoost = exp2(maxContentBoost),
      .minContentBoost = exp2(minContentBoost),
      .gamma = gamma,
      .offsetSdr = offsetSdr,
      .offsetHdr = offsetHdr,
      .hdrCapacityMin = exp2(hdrCapacityMin),
      .hdrCapacityMax = exp2(hdrCapacityMax)};

  UhdrCompressedStructWrapper outJpeg(width, height);
  outJpeg.allocateMemory();

  JpegR jpeg = JpegR();

  jpeg.appendGainMap(
      &jpgSdr,                 // jpeg
      &jpgGainmap,             // gainmap
      nullptr,                 // exif
      nullptr,                 // icc
      0,                       // icc size
      &meta,                   // metadata
      outJpeg.getImageHandle() // out
  );

  // jpeg.encodeJPEGR(
  //     rawImgP010.getImageHandle(),                 // hdr
  //     ultrahdr_transfer_function::ULTRAHDR_TF_HLG, // trasfer function
  //     outJpeg.getImageHandle(),                    // dest
  //     90,                                          // quality
  //     nullptr                                      // exif
  // );

  // printf("STATUS\n");
  // std::cout << status;
  // printf("\n");

  jr_compressed_ptr handle = outJpeg.getImageHandle();
  return val(typed_memory_view(handle->length, (uint8_t *)handle->data));
}

int main()
{
  // printf("HELLO WORLD\n");
}

jpegr_info_struct getJPEGRInfo(std::string jpeg, int jpegSize)
{
  const uint8_t *jpegPtr = reinterpret_cast<const uint8_t *>(jpeg.c_str());

  jpegr_compressed_struct jpegrPtr{
      .data = (void *)jpegPtr,
      .length = jpegSize,
      .maxLength = jpegSize,
      .colorGamut = ULTRAHDR_COLORGAMUT_UNSPECIFIED};

  jpegr_info_struct info{};
  // auto size = info.exifData->size();

  JpegR().getJPEGRInfo(&jpegrPtr, &info);

  return info;
}

struct ultrahdr_unpacked
{
  std::string metadata;
  val sdr;
  val gainMap;
  bool success;
  val errorMessage;
};

auto extractJpegR(std::string jpeg, int jpegSize)
{
  const uint8_t *jpegPtr = reinterpret_cast<const uint8_t *>(jpeg.c_str());

  jpegr_compressed_struct jpegr_image_ptr{
      .data = (void *)jpegPtr,
      .length = jpegSize,
      .maxLength = jpegSize,
      .colorGamut = ULTRAHDR_COLORGAMUT_UNSPECIFIED};

  jpegr_compressed_struct primary_jpeg_image, gainmap_jpeg_image;

  ultrahdr_unpacked unpacked;

  auto jpegr = JpegR();

  auto status = jpegr.extractPrimaryImageAndGainMap(&jpegr_image_ptr, &primary_jpeg_image, &gainmap_jpeg_image);
  if (status != NO_ERROR)
  {
    if (status == ERROR_JPEGR_GAIN_MAP_IMAGE_NOT_FOUND)
    {
      unpacked.success = false;
      unpacked.errorMessage = val("Gainmap Image not found");
      return unpacked;
    }

    unpacked.success = false;
    unpacked.errorMessage = val("Error Extracting gainmap, status: " + std::to_string(status));
    return unpacked;
  }

  JpegDecoderHelper jpeg_dec_obj_gm;

  if (!jpeg_dec_obj_gm.decompressMetadata(gainmap_jpeg_image.data, gainmap_jpeg_image.length))
  {
    unpacked.success = false;
    unpacked.errorMessage = val("Decode Metadata Error");
    return unpacked;
  }

  auto str = getRawMetadataFromXMP(
      static_cast<uint8_t *>(jpeg_dec_obj_gm.getXMPPtr()),
      jpeg_dec_obj_gm.getXMPSize());

  // ultrahdr_metadata_struct uhdr_metadata;
  // if (!getMetadataFromXMP(static_cast<uint8_t *>(jpeg_dec_obj_gm.getXMPPtr()),
  //                         jpeg_dec_obj_gm.getXMPSize(), &uhdr_metadata))
  // {
  //   unpacked.success = false;
  //   unpacked.errorMessage = val("Cannot Extract Metadata");
  //   return unpacked;
  // }

  // uhdr_metadata.hdrCapacityMax = log2(uhdr_metadata.hdrCapacityMax);
  // uhdr_metadata.hdrCapacityMin = log2(uhdr_metadata.hdrCapacityMin);
  // uhdr_metadata.maxContentBoost = log2(uhdr_metadata.maxContentBoost);
  // uhdr_metadata.minContentBoost = log2(uhdr_metadata.minContentBoost);

  unpacked.success = true;
  unpacked.metadata = str;
  unpacked.sdr = val(typed_memory_view(primary_jpeg_image.length, static_cast<uint8_t *>(primary_jpeg_image.data)));
  unpacked.gainMap = val(typed_memory_view(gainmap_jpeg_image.length, static_cast<uint8_t *>(gainmap_jpeg_image.data)));
  return unpacked;
}

std::string getExceptionMessage(intptr_t exceptionPtr)
{
  return std::string(reinterpret_cast<std::exception *>(exceptionPtr)->what());
}

EMSCRIPTEN_BINDINGS(module)
{

  value_object<ultrahdr_metadata_struct>("UltraHDRMetadata")
      .field("version", &ultrahdr_metadata_struct::version)
      .field("gamma", &ultrahdr_metadata_struct::gamma)
      .field("hdrCapacityMax", &ultrahdr_metadata_struct::hdrCapacityMax)
      .field("hdrCapacityMin", &ultrahdr_metadata_struct::hdrCapacityMin)
      .field("maxContentBoost", &ultrahdr_metadata_struct::maxContentBoost)
      .field("minContentBoost", &ultrahdr_metadata_struct::minContentBoost)
      .field("offsetHdr", &ultrahdr_metadata_struct::offsetHdr)
      .field("offsetSdr", &ultrahdr_metadata_struct::offsetSdr);

  value_object<ultrahdr_unpacked>("UltraHDRUnpacked")
      .field("metadata", &ultrahdr_unpacked::metadata)
      .field("success", &ultrahdr_unpacked::success)
      .field("errorMessage", &ultrahdr_unpacked::errorMessage)
      .field("sdr", &ultrahdr_unpacked::sdr)
      .field("gainMap", &ultrahdr_unpacked::gainMap);

  value_object<jpegr_info_struct>("JPEGRInfo")
      .field("width", &jpegr_info_struct::width)
      .field("height", &jpegr_info_struct::height)
      // .field("iccData", &jpegr_info_struct::iccData) // TODO: include this
      // .field("exifData", &jpegr_info_struct::exifData) // TODO: include this
      ;
  function("getExceptionMessage", &getExceptionMessage);
  function("appendGainMap", &appendGainMap);
  function("getJPEGRInfo", &getJPEGRInfo);
  function("extractJpegR", &extractJpegR);
}
