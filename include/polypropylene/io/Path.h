//
// Created by Paul on 06.01.2018.
//

#ifndef POLYPROPYLENE_PATH_H
#define POLYPROPYLENE_PATH_H

#include <string>
#include <polypropylene/stdutils/StringUtils.h>
#include <polypropylene/definitions/OSDetection.h>

namespace PAX {
    class Path {
    public:
        static constexpr const char * EmptyPath = ".";

        static constexpr char PathSeparator_Win = '\\';
        static constexpr char PathSeparator_Unix = '/';
        static constexpr char PathSeparator =
#ifdef PAX_OS_WIN
                PathSeparator_Win;
#else
                PathSeparator_Unix;
#endif

        static bool isAbsolute(const std::string & path);

    private:
        std::string _path;

    public:
        Path();
        PAX_IMPLICIT Path(const char* path);
        PAX_IMPLICIT Path(const std::string& path);
        Path(const Path& other);

        PAX_NODISCARD bool isEmpty() const;

        /**
         * A Path is considered to be a file, if it contains a dot, indicating a file ending, after the last slash/backslash.
         * @return True, iff this file points to a file.
         */
        PAX_NODISCARD bool isFile() const;

        /**
         * Inverse of isFile.
         * @return True, iff this file points to a directory.
         */
        PAX_NODISCARD bool isDirectory() const;
        PAX_NODISCARD bool isAbsolute() const;
        PAX_NODISCARD bool isRelative() const;

        /**
         * @return The directoy of this path.
         *         If this path points to a file, this will return the directory, the file is located in.
         *         If this path points to a directory, returns a copy of this path.
         */
        PAX_NODISCARD Path getDirectory() const;

        /**
         * Converts this path to a relative directory with respect to the given root directory.
         * Example:
         *     Path("/usr/bin/blob.txt").toRelative("/usr") == Path("/bin/blob.txt")
         * Returns a copy of this path if this path is already relative (discarding the given root directory).
         * @param root Directory from which this path should be made relative.
         * @return This path as a relative directory.
         */
        PAX_NODISCARD Path toRelative(const Path & root) const;
        PAX_NODISCARD Path toAbsolute() const;

        PAX_NODISCARD std::string getFileEnding() const;
        void dropFileEnding();
        PAX_NODISCARD Path withoutFileEnding() const;

        /**
         * Converts all slashes to the current platform's separator.
         * For Windows, this will be `\` and on unix this will be `/`.
         */
        void convertToCurrentPlatform();
        void convertToWin();
        void convertToUnix();
        PAX_NODISCARD Path convertedToCurrentPlatform() const;
        PAX_NODISCARD Path convertedToWin() const;
        PAX_NODISCARD Path convertedToUnix() const;

        void simplify();

        static void convertToCurrentPlatform(std::string & path);
        static void convertToWin(std::string & path);
        static void convertToUnix(std::string & path);
        static void simplify(std::string & path);
        static std::string toAbsolute(const std::string & path);

        PAX_NODISCARD const char* c_str() const;
        PAX_NODISCARD const std::string& toString() const;

        bool operator==(const Path& other) const;
        bool operator<(const Path& other) const;
        bool operator>(const Path& other) const;
        bool operator!=(const Path& other) const;

        PAX_IMPLICIT operator const char*() const;
        PAX_IMPLICIT operator std::string() const;

        Path& operator=(const char* path);
        Path& operator=(const std::string& path);
        Path& operator=(const Path& other);
        Path operator+(const char* path) const;
        Path operator+(const std::string& path) const;
        Path& operator+=(const char* path);
        Path& operator+=(const std::string& path);

        Path operator+(const Path& other) const;
        Path& operator+=(const Path& other);
    };
}

std::ostream& operator<<(std::ostream& os, const PAX::Path & p);

#endif //POLYPROPYLENE_PATH_H
