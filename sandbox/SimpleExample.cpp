#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include <boost/system/error_code.hpp>

#include "sandbox.h"

using namespace boost;


std::vector<char> buffer(20 * 1024);                                            // глобальный буфер для чтения данных
size_t package_counter;                                                         // счетчик пакетов

void GrabSomeData(asio::ip::tcp::socket& socket)                                // функция для приема данных через сокет
{
    socket.async_read_some(asio::buffer(buffer.data(), buffer.size()),          // асинхронно (в потоке контекста) читаем данные в буфер 
        [&](std::error_code ec, size_t length)                                  // асинхронный обработчик завершения операции
        {
            if (!ec)                                                            // если ошибок нет
            {            
                std::cout << "\n\nRead " << length << " bytes\n\n";             // выводим размер полученного пакета

                for (size_t i = 0; i < length; i++)                             // выводим содержимое пакета
                {
                    std::cout << buffer[i];
                } 

                ++package_counter;                                              // инкрементируем счетчик пакетов
                GrabSomeData(socket);                                           // снова считываем данные (предполагаем, что считали не все)
            }
            else                                                                // иначе
            {
                std::cout << "ERROR CODE: " << ec.message() << std::endl        // выводим содержимое сообщения об ошибке
                          << "PACKAGE COUNTER: " << package_counter             // и значение счетчика пакетов
                          << std::endl;
            }
        }
    );
}       

int SimpleExample()
{
    system::error_code ec;                                                      // переменная для информации об ошибках

    asio::io_context context;                                                   // создаем "контекст" - основной платформозависимый интерфейс 
    asio::io_context::work idleWork(context);                                   // создаем "бесполезную" работу для контекста
    std::thread contextThread = std::thread([&]() { context.run(); });          // создаем дополнительный поток для запуска контекста

    auto address = asio::ip::make_address("51.38.81.49", ec);                   // получаем адрес местоназначения
    asio::ip::tcp::endpoint endpoint(address, 80);                              // создаем точку назначения
    
    asio::ip::tcp::socket socket(context);                                      // создаем сокет
    socket.connect(endpoint, ec);                                               // производим попытку соединения сокета и точки назначения

    if (!ec)                                                                    // проверка на ошибки
    {
        std::cout << "Connected!" << std::endl;
    }
    else
    {
        std::cout << "Failed to connect to address:\n"
                  << ec.message() << std::endl;
    }

    if (socket.is_open())                                                       // в случае успешного открытия сокета
    {
        GrabSomeData(socket);                                                   // читаем все, что приходит

        std::string request = 
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";

        socket.write_some(asio::buffer(request.data(), request.size()), ec);    // отправляем запросы на endpoint

        using namespace std::chrono_literals;                                   
        std::this_thread::sleep_for(2s);                                        // дожидаемся завершения асинхронного чтения

        context.stop();                                                         // предполагаем, что чтение закончилось, и завершаем
        if (contextThread.joinable())                                           // "бесполезную" работу контекста
        {                                                                       // безопасно завершаем работу потока контекста
            contextThread.join();
        }
    }
    
    return 0;
}