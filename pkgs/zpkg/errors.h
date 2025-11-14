#pragma once

/** 
 * @copyright Copyright © 2020-2025 code by zhaoj
 * 
 * LICENSE
 * 
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

 /**
 * @author zhaoj 286897655@qq.com
 * @brief 
 */

#ifndef ZPKG_ERRORS_H_
#define ZPKG_ERRORS_H_

#include <string>

/**
 * Status and Result implementation
*/
class Status_t{
public:
    enum StatusCode{
        // Status Ok
        kOK = 0,
        // Status_t InvalidArgument indicates the caller
        // specified an invalid argument
        kInvalidArgument = 1,
        // Status_t NotFound indicates some requested entity (such as
        // a file or directory) was not found.
        kNotFound = 2,
        // Status_t AlreadyExists indicates that the entity a
        // caller attempted to create (such as a file or directory) is already
        // present.
        kAlreadyExist = 3,
        kOutOfRange = 4,
        kNotSupported = 5,
        kFail = 6
    };
public:
    Status_t() noexcept : status_code_(StatusCode::kOK){}
    ~Status_t(){}
    Status_t(const Status_t& rhs) : status_code_(rhs.status_code_){}
    Status_t& operator=(const Status_t& rhs) { status_code_ = rhs.status_code_; return *this;}

    Status_t(Status_t&& rhs) noexcept : status_code_(rhs.status_code_) {}
    Status_t& operator=(Status_t&& rhs) noexcept { status_code_ = rhs.status_code_; return *this;}

    static Status_t OK() { return  Status_t(); }
    static Status_t InvalidArgument() { return StatusCode::kInvalidArgument; }
    static Status_t NotFound() { return StatusCode::kNotFound; }
    static Status_t AlreadyExist(){ return StatusCode::kAlreadyExist; }
    static Status_t OutOfRange(){ return StatusCode::kOutOfRange; }
    static Status_t NotSupported(){ return StatusCode::kNotSupported; }
    static Status_t Fail() { return StatusCode::kFail; }

    // Returns status if indicates success.
    bool ok() const{ return (status_code_ == StatusCode::kOK);}
    template<StatusCode scode>
    bool is() const{ return (status_code_ == scode);}
    bool isInvalidArgument() const { return (status_code_ == StatusCode::kInvalidArgument);}
    bool isNotFound() const { return (status_code_ == StatusCode::kNotFound);}
    bool isAlreadyExist() const { return (status_code_ == StatusCode::kAlreadyExist);}
    bool isOutOfRange() const { return (status_code_ == StatusCode::kOutOfRange); }
    bool isNotSupported() const { return (status_code_ == StatusCode::kNotSupported);}
    bool isFail() const { return (status_code_ == StatusCode::kFail);}

    const StatusCode code() const{ return status_code_;}

    bool operator==(StatusCode code) const{ return (status_code_ == code);}
    bool operator==(const Status_t& status) const { return (status_code_ == status.status_code_); }

    const std::string message() const{
        static constexpr const char* kStatusMessage[] = {"OK","InvalidArgument","NotFound","AlreadyExist","OutOfRange","NotSupported","Fail"};
        return kStatusMessage[static_cast<int>(status_code_)];
    }
private:
    Status_t(StatusCode code) noexcept : status_code_(code){}
private:
    StatusCode status_code_;
};

template<typename RType>
class Result_t{

};


#endif //!ZPKG_ERRORS_H_