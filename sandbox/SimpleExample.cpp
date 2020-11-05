#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include <boost/system/error_code.hpp>

#include "sandbox.h"

using namespace boost;

int SimpleExample()
{
    system::error_code ec;

    asio::io_context context;                                                   // создаем "контекст" - основной платформозависимый интерфейс 
    
    auto address = asio::ip::make_address("93.184.216.34", ec);                 // получаем адрес местоназначения
    asio::ip::tcp::endpoint endpoint(address, 80);                              // создаем точку назначения
    
    asio::ip::tcp::socket socket(context);                                      // создаем сокет
    socket.connect(endpoint, ec);                                               // производим попытку соединения сокета и точки назначения

    if(!ec)
    {
        std::cout << "Connected!" << std::endl;
    }
    else
    {
        std::cout << "Failed to connect to address:\n"
                  << ec.message() << std::endl;
    }
    
    return 0;
}