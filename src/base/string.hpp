/**************************************************************************
 * Copyright 2017-2018 NextCash, LLC                                      *
 * Contributors :                                                         *
 *   Curtis Ellis <curtis@nextcash.tech>                                  *
 * Distributed under the MIT software license, see the accompanying       *
 * file license.txt or http://www.opensource.org/licenses/mit-license.php *
 **************************************************************************/
#ifndef NEXTCASH_STRING_HPP
#define NEXTCASH_STRING_HPP

#include "math.hpp"

#include <cstring>
#include <cstdarg>

#define NEXTCASH_STRING_LOG_NAME "String"


namespace NextCash
{
#ifdef _WIN32
    static const char *PATH_SEPARATOR __attribute__ ((unused)) = "\\";
#else
    static const char *PATH_SEPARATOR __attribute__ ((unused)) = "/";
#endif

    class String
    {
    public:

        String() { mData = NULL; }
        String(const String &pCopy)
        {
            mData = NULL;
            *this = pCopy;
        }
        String(const char *pText)
        {
            mData = NULL;
            *this = pText; // Call = operator
        }
        ~String()
        {
            if(mData != NULL)
                delete[] mData;
        }

        size_t memorySize()
        {
            return sizeof(char *) + length() + 1;
        }

        /*******************************************************************************************
         * Basic functions
         ******************************************************************************************/
        const char *text() const
        {
            if(mData != NULL)
                return mData;
            else
                return "";
        }

        unsigned int length() const
        {
            if(mData == NULL)
                return 0;
            else
                return std::strlen(mData);
        }

        char &operator[] (unsigned int pOffset)
        {
            return mData[pOffset];
        }

        void clear()
        {
            if(mData != NULL)
            {
                delete[] mData;
                mData = NULL;
            }
        }

        // Double wipe data
        void clean()
        {
            if(mData != NULL)
            {
                unsigned int theLength = length();
                std::memset(mData, 0x00, theLength);
                std::memset(mData, 0xff, theLength);
                std::memset(mData, 0x00, theLength);
                delete[] mData;
                mData = NULL;
            }
        }

        // Allocate memory for a string of specific length and return it to be written into.
        char *writeAddress(unsigned int pLength);

        // Write binary data to string in hex text format.
        void writeHex(const void *pData, unsigned int pSize);
        void writeReverseHex(const void *pData, unsigned int pSize);

        // Read hex text into binary data.
        //   pData must have at least half as many bytes allocated as the string's length.
        //   Returns number of bytes written.
        unsigned int readHex(uint8_t *pData);
        unsigned int readReverseHex(uint8_t *pData);

        // Write binary data to string in base 58 text format.
        void writeBase58(const void *pData, unsigned int pSize);

        // Write binary data to string in base 32 text format.
        void writeBase32(const void *pData, unsigned int pSize);

        // Append a file system path separator and then path part to the string.
        String &pathAppend(const char *pPart)
        {
            if(length() > 0 && mData[length()-1] != *PATH_SEPARATOR)
                *this += PATH_SEPARATOR;
            *this += pPart;
            return *this;
        }

        // Uses local time.
        void writeFormattedTime(time_t pTime, bool pLocal = true, const char *pFormat = "%F %T"); // YYYY-MM-DD HH:MM::SS

        // Write with printf formatting codes.
        bool writeFormatted(const char *pFormatting, ...);
        bool writeFormattedList(const char *pFormatting, va_list &pList);

        /*******************************************************************************************
         * Compare/Evaluate Operators
         ******************************************************************************************/
        bool operator !() const { return mData == NULL; }
        operator bool() const { return mData != NULL; }

        operator const char *() const { return text(); }

        bool operator == (const char *pRight) const
        {
            if(mData == NULL)
                return pRight == NULL || *pRight == 0;
            else if(pRight == NULL || *pRight == 0)
                return false;

            return std::strcmp(mData, pRight) == 0;
        }
        bool operator == (const String &pRight) const { return *this == pRight.text(); }

        bool operator != (const char *pRight) const { return !(*this == pRight); }
        bool operator != (const String &pRight) const { return !(*this == pRight.text()); }

        int compare(const char *pRight) const
        {
            // This is empty
            if(mData == NULL)
            {
                if(pRight == NULL || *pRight == 0)
                    return 0; // Right is empty
                else
                    return -1; // Right is not empty
            }

            if(pRight == NULL || *pRight == 0)
                return 1; // Right is empty

            return std::strcmp(mData, pRight);
        }

        bool operator < (const char *pRight) const
        {
            if(mData == NULL)
                return pRight != NULL && *pRight != 0;
            else if(pRight == NULL || *pRight == 0)
                return false;

            return std::strcmp(mData, pRight) < 0;
        }
        bool operator < (const String &pRight) const { return *this < pRight.text(); }

        bool operator <= (const char *pRight) const { return *this < pRight || *this == pRight; }
        bool operator <= (const String &pRight) const { return *this < pRight || *this == pRight; }

        bool operator > (const char *pRight) const
        {
            if(mData == NULL)
                return false;
            else if(pRight == NULL || *pRight == 0)
                return true;

            return std::strcmp(mData, pRight) > 0;
        }
        bool operator > (const String &pRight) const { return *this > pRight.text(); }

        bool operator >= (const char *pRight) const { return *this > pRight || *this == pRight; }
        bool operator >= (const String &pRight) const { return *this > pRight || *this == pRight; }

        /*******************************************************************************************
         * Modify Operators
         ******************************************************************************************/
        String &operator = (const char *pRight);
        String &operator = (const String &pRight);

        void operator += (const char *pRight);
        void operator += (const String &pRight) { *this += pRight.text(); }
        void operator += (char pRight);

        String operator + (const char *pRight) const
        {
            String result(*this);
            result += pRight;
            return result;
        }
        String operator + (const String &pRight) const
        {
            String result(*this);
            result += pRight;
            return result;
        }

        static bool test();

    private:

        char *mData;

    };

    inline bool isLetter(char pChar)
    {
        return (pChar >= 'a' && pChar <= 'z') || (pChar >= 'A' && pChar <= 'Z');
    }

    inline bool isWhiteSpace(char pChar)
    {
        return pChar == ' ' || pChar == '\t' || pChar == '\r' || pChar == '\n';
    }

    inline bool isInt(char pChar)
    {
        return pChar >= '0' && pChar <= '9';
    }

    inline bool isUpper(char pChar)
    {
        return pChar >= 'A' && pChar <= 'Z';
    }

    inline bool isLower(char pChar)
    {
        return pChar >= 'a' && pChar <= 'z';
    }

    inline bool isASCII(char pChar)
    {
        return pChar >= 32 && pChar <= 126;
    }

    inline char upper(char pChar)
    {
        if(isLower(pChar))
            return pChar - 32;
        else
            return pChar;
    }

    inline char lower(char pChar)
    {
        if(isUpper(pChar))
            return pChar + 32;
        else
            return pChar;
    }

    // RFC 3986 Uniform Resource Identifier (URI) encoding
    String uriEncode(const char *pString);

    // RFC 3986 Uniform Resource Identifier (URI) decoding
    String uriDecode(const char *pString);
}

#endif
