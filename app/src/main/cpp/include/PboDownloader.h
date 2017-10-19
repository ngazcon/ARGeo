#pragma once

#include "ReadPixelBuffer.h"
#include "TextureHelper.h"
#include "FrameState.h"

#include <vector>
#include <memory>

namespace argeo {
    class PboDownloader
    {

    public:
        PboDownloader();
        ~PboDownloader();

        void update(
                FrameState *frame_state,
                const unsigned int& width,
                const unsigned int& height,
                const ImageFormat& format,
                const ImageDataType& data_type);


        unsigned int download(
                FrameBuffer* frame_buffer,
                const unsigned int& x_offset,
                const unsigned int& y_offset,
                const unsigned int& width,
                const unsigned int& height);

        void read_pixels(
                const unsigned int& index,
                const std::size_t& offset_in_bytes,
                const std::size_t& size_in_bytes,
                void* pixels);

        template<class T>
        inline std::vector<T> read_pixels(
                const unsigned int& index,
                const std::size_t& offset_in_bytes,
                const std::size_t& size_in_bytes)
        {
            std::vector<T> data;
            data.resize(size_in_bytes / sizeof(T));

            read_pixels(
                    index,
                    offset_in_bytes,
                    size_in_bytes,
                    data.data());

            return data;
        }

        template<class T>
        inline std::vector<T> read_pixels(
                const unsigned int& index,
                const std::size_t& offset_in_bytes)
        {
            const std::size_t size_in_bytes = TextureHelper::required_size_in_bytes(
                    m_width,
                    m_height,
                    m_format,
                    m_data_type,
                    4);

            return read_pixels<T>(
                    index,
                    offset_in_bytes,
                    size_in_bytes);
        }

        template<class T>
        inline std::vector<T> read_pixels(const unsigned int& index)
        {
            return read_pixels<T>(
                    index,
                    0);
        }

    private:
        static const unsigned int NUM_PBOS;

        ImageFormat m_format;
        ImageDataType m_data_type;

        unsigned int m_last_pbo_index;
        unsigned int m_width;
        unsigned int m_height;

        std::vector<std::unique_ptr<ReadPixelBuffer>> m_pbos;
    };
}
