#pragma once

#include "types.h"
#include "platform.h"

#include <spdlog/spdlog.h>

#include <vector>
#include <filesystem>

namespace svl
{
    /**
     * a files open mode
     */
    namespace mode
    {
        constexpr u8 read = (1 << 0); /// read only file
        constexpr u8 write = (1 << 1); /// write file, implies read as well
    }

    /**
     * an abstract base class for all file handles
     */
    struct file_handle 
    {
        /**
         * virtual destructor 
         */
        virtual ~file_handle() {}

        /**
         * seek to an absolute offset in the file
         * @param pos the position to seek from relative to 0
         */
        virtual void seek(u64 pos) {}

        /**
         * get the current cursor position in the file
         * @return the current cursor position
         */
        virtual u64 tell() const { return 0; }

        /**
         * get the size of the file in bytes
         * @return length of file data in bytes
         */
        virtual u64 size() const { return 0; }

        /**
         * read binary data into a pointer
         * @param out the pointer to write to
         * @param num the amount of bytes to read
         */
        virtual void read(void* out, u64 num) {}

        /**
         * write binary data from a pointer into the file
         * @param in the binary data to read from
         * @param num the amount of bytes to read
         */
        virtual void write(const void* in, u64 num) {}
    };

    /**
     * represents a file stream
     * can be either an in memory stream or a disk backed file
     */
    struct file
    {
        /**
         * seek to a point in the file
         * @param pos the point to seek to
         */
        void seek(u64 pos)
        {
            handle->seek(pos);
        }

        /**
         * get the location of the cursor in the file
         * @return the position of the cursor
         */
        u64 tell() const
        {
            return handle->tell();
        }

        /**
         * get the size of the file
         * @return size in bytes
         */
        u64 size() const 
        {
            return handle->size();
        }

        /**
         * read a POD type in from the stream
         * @tparam the type to read in
         * @return the read in data
         */
        template<typename T>
        T read()
        {
            static_assert(std::is_standard_layout<T>::value && std::is_trivial<T>::value);
            
            T val;
            handle->read(&val, sizeof(T));
            return val;
        }

        /**
         * @brief read in N amount of POD type
         * @tparam the type to read in
         * @param num the amount of data to read in
         * @return the read in data in a struct
         */
        template<typename T>
        std::vector<T> read(u64 num)
        {
            std::vector<T> vec(num);
            handle->read(vec.data(), num * sizeof(T));
            return vec;
        }
        
        /**
         * @brief read n bytes into a void*
         * 
         * @param out the pointer to write to
         * @param num the amount of bytes to read
         */
        void read(void* out, u64 num)
        {
            handle->read(out, num);
        }

        /**
         * @brief write a POD type into the stream
         * 
         * @tparam T the type to write
         * @param val the value to write
         */
        template<typename T>
        void write(const T& val)
        {
            static_assert(std::is_standard_layout<T>::value && std::is_trivial<T>::value);

            handle->write(&val, sizeof(T));
        }

        /**
         * @brief write an array of POD types into the stream
         * 
         * @tparam T the pod type to write
         * @param vec the data to write
         */
        template<typename T>
        void write(const std::vector<T>& vec)
        {
            static_assert(std::is_standard_layout<T>::value && std::is_trivial<T>::value);

            handle->write(vec.data(), vec.size() * sizeof(T));
        }

        /**
         * @brief write data from a void* into the stream
         * 
         * @param ptr the data to read from
         * @param num the amount of bytes to write
         */
        void write(const void* ptr, u64 num)
        {
            handle->write(ptr, num);
        }

        /**
         * @brief check if the file is valid
         * 
         * @return true the file is valid
         * @return false the file is invalid
         */
        bool valid() const
        {
            return !!handle;
        }

        file()
            : handle(nullptr)
        {}

        file(file_handle* ptr)
            : handle(ptr)
        {}

        file(const file& f) : handle(f.handle) {}

    private:

        std::shared_ptr<file_handle> handle;
    };

    file open(const std::filesystem::path& path, u8 mo);
    
    file from(const std::vector<svl::byte>& vec);
}