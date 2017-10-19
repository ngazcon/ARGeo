#include "PboDownloader.h"

#include <Device.h>

namespace argeo
{
    const unsigned int PboDownloader::NUM_PBOS = 4;

    PboDownloader::PboDownloader()
    : m_last_pbo_index(0)
    { }


    PboDownloader::~PboDownloader()
    {}

    void PboDownloader::update(
            FrameState *frame_state,
            const unsigned int& width,
            const unsigned int& height,
            const ImageFormat& format,
            const ImageDataType& data_type)
    {
        if (format != m_format || data_type != m_data_type ||
            height != m_height || width != m_width)
        {
            m_pbos.clear();
            for (unsigned int i = 0; i < NUM_PBOS; i++)
            {
                m_pbos.push_back(Device::create_read_pixel_buffer(
                        PixelBufferHint::Stream,
                        TextureHelper::required_size_in_bytes(
                                width,
                                height,
                                format,
                                data_type,
                                4)));
            }

            m_format = format;
            m_data_type = data_type;
            m_width = width;
            m_height = height;
        }

        m_last_pbo_index = 0;
    }

    unsigned int PboDownloader::download(
            FrameBuffer *frame_buffer,
            const unsigned int& x_offset,
            const unsigned int& y_offset,
            const unsigned int& width,
            const unsigned int& height)
    {
        unsigned int index = m_last_pbo_index;
        m_last_pbo_index++;

        ReadPixelBuffer* pbo = m_pbos[index].get();

        pbo->transfer(
                frame_buffer,
                x_offset,
                y_offset,
                width,
                height);

        return index;
    }

    void PboDownloader::read_pixels(
            const unsigned int& index,
            const std::size_t& offset_in_bytes,
            const std::size_t& size_in_bytes,
            void* pixels)
    {
        ReadPixelBuffer* pbo = m_pbos[index].get();
        pbo->copy_from_device(
                pixels,
                offset_in_bytes,
                size_in_bytes);
    }


}