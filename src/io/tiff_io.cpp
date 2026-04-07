#include "chiefray/io/tiff_io.hpp"
#include <tiffio.h>
#include <cstring>
#include <memory>
#include <type_traits>
#include <stdexcept>

namespace chiefray {

namespace {

struct TiffDeleter {
    void operator()(TIFF* tif) const { if (tif) TIFFClose(tif); }
};
using TiffPtr = std::unique_ptr<TIFF, TiffDeleter>;

TiffPtr openTiff(const std::string& path, const char* mode) {
    TiffPtr tif(TIFFOpen(path.c_str(), mode));
    if (!tif) throw std::runtime_error("Failed to open TIFF: " + path);
    return tif;
}

template <typename T>
constexpr uint16_t sampleFormat() {
    if constexpr (std::is_floating_point_v<T>) return SAMPLEFORMAT_IEEEFP;
    else if constexpr (std::is_unsigned_v<T>)  return SAMPLEFORMAT_UINT;
    else return SAMPLEFORMAT_INT;
}

uint16_t mapCompression(TiffCompression comp) {
    switch(comp) {
        case TiffCompression::Lzw:     return COMPRESSION_LZW;
        case TiffCompression::Deflate: return COMPRESSION_ADOBE_DEFLATE;
        default:                       return COMPRESSION_NONE;
    }
}

template <typename T>
void convertScanline(const uint8_t* src, T* dst, uint32_t width,
                     uint16_t bps, uint16_t fmt) {
    for (uint32_t i = 0; i < width; ++i) {
        if (fmt == SAMPLEFORMAT_IEEEFP) {
            if (bps == 32) {
                float v; std::memcpy(&v, src + i * sizeof(float), sizeof(float));
                dst[i] = static_cast<T>(v);
            } else if (bps == 64) {
                double v; std::memcpy(&v, src + i * sizeof(double), sizeof(double));
                dst[i] = static_cast<T>(v);
            }
        } else {
            if (bps == 8) {
                dst[i] = static_cast<T>(src[i]);
            } else if (bps == 16) {
                uint16_t v; std::memcpy(&v, src + i * sizeof(uint16_t), sizeof(uint16_t));
                dst[i] = static_cast<T>(v);
            } else if (bps == 32) {
                uint32_t v; std::memcpy(&v, src + i * sizeof(uint32_t), sizeof(uint32_t));
                dst[i] = static_cast<T>(v);
            }
        }
    }
}

struct TiffPageInfo {
    uint32_t width;
    uint32_t height;
    uint16_t bps;
    uint16_t spp;
    uint16_t fmt;
};

TiffPageInfo getPageInfo(TIFF* tif) {
    TiffPageInfo info{};
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &info.width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &info.height);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &info.bps);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &info.spp);
    TIFFGetFieldDefaulted(tif, TIFFTAG_SAMPLEFORMAT, &info.fmt);

    if (info.spp != 1)
        throw std::runtime_error("Only single-channel (grayscale) TIFFs are supported.");

    return info;
}

template <typename T>
void readPageInto(TIFF* tif, T* dst, Eigen::Index rows, Eigen::Index cols) {
    auto info = getPageInfo(tif);
    std::vector<uint8_t> buf(static_cast<size_t>(TIFFScanlineSize(tif)));

    for (uint32_t row = 0; row < info.height; ++row) {
        if (TIFFReadScanline(tif, buf.data(), row) < 0)
            throw std::runtime_error("Failed to read scanline " + std::to_string(row));
        convertScanline<T>(buf.data(), dst + static_cast<Eigen::Index>(row) * cols,
                           info.width, info.bps, info.fmt);
    }
}

template <typename T>
void writePageFrom(TIFF* tif, const T* src, uint32_t height, uint32_t width,
                   bool multiPage, TiffCompression comp) {
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, static_cast<uint16_t>(sizeof(T) * 8));
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, static_cast<uint16_t>(1));
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, sampleFormat<T>());
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, height);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Setup Compression
    uint16_t tiffComp = mapCompression(comp);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, tiffComp);
    
    // Set predictor for better compression ratios on compressed data
    if (tiffComp == COMPRESSION_LZW || tiffComp == COMPRESSION_DEFLATE) {
        if constexpr (std::is_integral_v<T>) {
            TIFFSetField(tif, TIFFTAG_PREDICTOR, PREDICTOR_HORIZONTAL);
        } else {
            TIFFSetField(tif, TIFFTAG_PREDICTOR, PREDICTOR_FLOATINGPOINT);
        }
    }

    if (multiPage)
        TIFFSetField(tif, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);

    for (uint32_t row = 0; row < height; ++row) {
        if (TIFFWriteScanline(tif, const_cast<T*>(src + row * width), row) < 0)
            throw std::runtime_error("Failed to write scanline " + std::to_string(row));
    }
}

} // anonymous namespace

// --- Single image ---

template <typename T>
Image<T> readTiff(const std::string& path) {
    auto tif = openTiff(path, "r");
    auto info = getPageInfo(tif.get());
    Image<T> image(info.height, info.width);
    readPageInto<T>(tif.get(), image.data(), image.rows(), image.cols());
    return image;
}

template <typename T>
void writeTiff(const std::string& path, const Image<T>& image, TiffCompression comp) {
    auto tif = openTiff(path, "w");
    writePageFrom<T>(tif.get(), image.data(),
                     static_cast<uint32_t>(image.rows()),
                     static_cast<uint32_t>(image.cols()), false, comp);
}

// --- Image stack ---

template <typename T>
ImageStack<T> readTiffStack(const std::string& path) {
    Eigen::Index pageCount = 0;
    Eigen::Index rows = 0;
    Eigen::Index cols = 0;

    // Pass 1: Light-weight sequential read to grab dimensions and count pages
    {
        auto tif = openTiff(path, "r");
        auto info = getPageInfo(tif.get());
        rows = static_cast<Eigen::Index>(info.height);
        cols = static_cast<Eigen::Index>(info.width);

        do { ++pageCount; } while (TIFFReadDirectory(tif.get()));
    }

    // Allocate the contiguous memory block once
    ImageStack<T> stack(pageCount, rows, cols);

    // Parallel thread pool opening individual file handles
    #pragma omp parallel for schedule(dynamic)
    for (Eigen::Index z = 0; z < pageCount; ++z) {
        auto localTif = openTiff(path, "r");
        TIFFSetDirectory(localTif.get(), static_cast<uint16_t>(z));
        readPageInto<T>(localTif.get(), stack.data() + z * stack.stride(), rows, cols);
    }

    return stack;
}

template <typename T>
void writeTiffStack(const std::string& path, const ImageStack<T>& stack, TiffCompression comp) {
    if (stack.empty())
        throw std::runtime_error("Cannot write empty stack");

    // "w8" forces BigTIFF creation
    auto tif = openTiff(path, "w8");
    auto height = static_cast<uint32_t>(stack.rows());
    auto width = static_cast<uint32_t>(stack.cols());

    for (Eigen::Index z = 0; z < stack.depth(); ++z) {
        writePageFrom<T>(tif.get(), stack.data() + z * stack.stride(),
                         height, width, true, comp);
        TIFFWriteDirectory(tif.get());
    }
}

// Explicit instantiations
template Image<uint8_t> readTiff(const std::string&);
template Image<uint16_t> readTiff(const std::string&);
template Image<float> readTiff(const std::string&);
template Image<double> readTiff(const std::string&);

template ImageStack<uint8_t> readTiffStack(const std::string&);
template ImageStack<uint16_t> readTiffStack(const std::string&);
template ImageStack<float> readTiffStack(const std::string&);
template ImageStack<double> readTiffStack(const std::string&);

template void writeTiff(const std::string&, const Image<uint8_t>&, TiffCompression);
template void writeTiff(const std::string&, const Image<uint16_t>&, TiffCompression);
template void writeTiff(const std::string&, const Image<float>&, TiffCompression);
template void writeTiff(const std::string&, const Image<double>&, TiffCompression);

template void writeTiffStack(const std::string&, const ImageStack<uint8_t>&, TiffCompression);
template void writeTiffStack(const std::string&, const ImageStack<uint16_t>&, TiffCompression);
template void writeTiffStack(const std::string&, const ImageStack<float>&, TiffCompression);
template void writeTiffStack(const std::string&, const ImageStack<double>&, TiffCompression);

} // namespace chiefray