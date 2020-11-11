#pragma once
#include "NetCommon.h"

namespace netcafe
{
    template <typename T>
    struct MessageHeader
    {
        T id{};
        uint32_t size = 0;
    };

    template <typename T>
    struct Message
    {
        MessageHeader<T> header{};
        std::vector<uint8_t> body;

        size_t size() const
        {
            return sizeof(MessageHeader<T>) + body.size();
        }

        friend std::ostream& operator<<(std::ostream& os,
                                        const Message<T>& message)
        {
            os << "ID:" << static_cast<int>(message.header.id)
               << " Size:" << message.header.size;
            return os;
        }

        template <typename DataType>
        friend Message<T>& operator<<(Message<T>& message,
                                      const DataType& data)
        {
            static_assert(std::is_standard_layout<DataType>::value,
                          "Data is too complex to be pushed");

            size_t size = message.body.size();
            message.body.resize(message.body.size() + sizeof(DataType));
            std::memcpy(message.body.data() + size, &data, sizeof(DataType));
            message.header.size = message.size();
            return message;
        }
    };
}
