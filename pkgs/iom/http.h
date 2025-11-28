#pragma once

/** 
 * @copyright Copyright © 2020-2025 zhaoj
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

#ifndef IOM_HTTP_H_
#define IOM_HTTP_H_

#include <string>
#include <cstring>
#include <map>

namespace iom{

enum http_method{
  HTTP_METHOD_DELETE = 0,
  HTTP_METHOD_GET = 1,
  HTTP_METHOD_HEAD = 2,
  HTTP_METHOD_POST = 3,
  HTTP_METHOD_PUT = 4,
  HTTP_METHOD_CONNECT = 5,
  HTTP_METHOD_OPTIONS = 6,
  HTTP_METHOD_TRACE = 7,
  HTTP_METHOD_COPY = 8,
  HTTP_METHOD_LOCK = 9,
  HTTP_METHOD_MKCOL = 10,
  HTTP_METHOD_MOVE = 11,
  HTTP_METHOD_PROPFIND = 12,
  HTTP_METHOD_PROPPATCH = 13,
  HTTP_METHOD_SEARCH = 14,
  HTTP_METHOD_UNLOCK = 15,
  HTTP_METHOD_BIND = 16,
  HTTP_METHOD_REBIND = 17,
  HTTP_METHOD_UNBIND = 18,
  HTTP_METHOD_ACL = 19,
  HTTP_METHOD_REPORT = 20,
  HTTP_METHOD_MKACTIVITY = 21,
  HTTP_METHOD_CHECKOUT = 22,
  HTTP_METHOD_MERGE = 23,
  HTTP_METHOD_MSEARCH = 24,
  HTTP_METHOD_NOTIFY = 25,
  HTTP_METHOD_SUBSCRIBE = 26,
  HTTP_METHOD_UNSUBSCRIBE = 27,
  HTTP_METHOD_PATCH = 28,
  HTTP_METHOD_PURGE = 29,
  HTTP_METHOD_MKCALENDAR = 30,
  HTTP_METHOD_LINK = 31,
  HTTP_METHOD_UNLINK = 32,
  HTTP_METHOD_SOURCE = 33,
  HTTP_METHOD_PRI = 34,
  HTTP_METHOD_DESCRIBE = 35,
  HTTP_METHOD_ANNOUNCE = 36,
  HTTP_METHOD_SETUP = 37,
  HTTP_METHOD_PLAY = 38,
  HTTP_METHOD_PAUSE = 39,
  HTTP_METHOD_TEARDOWN = 40,
  HTTP_METHOD_GET_PARAMETER = 41,
  HTTP_METHOD_SET_PARAMETER = 42,
  HTTP_METHOD_REDIRECT = 43,
  HTTP_METHOD_RECORD = 44,
  HTTP_METHOD_FLUSH = 45,
  HTTP_METHOD_QUERY = 46
};

enum http_status{
    HTTP_STATUS_CONTINUE = 100,
    HTTP_STATUS_SWITCHING_PROTOCOLS = 101,
    HTTP_STATUS_PROCESSING = 102,
    HTTP_STATUS_EARLY_HINTS = 103,
    HTTP_STATUS_RESPONSE_IS_STALE = 110,
    HTTP_STATUS_REVALIDATION_FAILED = 111,
    HTTP_STATUS_DISCONNECTED_OPERATION = 112,
    HTTP_STATUS_HEURISTIC_EXPIRATION = 113,
    HTTP_STATUS_MISCELLANEOUS_WARNING = 199,
    HTTP_STATUS_OK = 200,
    HTTP_STATUS_CREATED = 201,
    HTTP_STATUS_ACCEPTED = 202,
    HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION = 203,
    HTTP_STATUS_NO_CONTENT = 204,
    HTTP_STATUS_RESET_CONTENT = 205,
    HTTP_STATUS_PARTIAL_CONTENT = 206,
    HTTP_STATUS_MULTI_STATUS = 207,
    HTTP_STATUS_ALREADY_REPORTED = 208,
    HTTP_STATUS_TRANSFORMATION_APPLIED = 214,
    HTTP_STATUS_IM_USED = 226,
    HTTP_STATUS_MISCELLANEOUS_PERSISTENT_WARNING = 299,
    HTTP_STATUS_MULTIPLE_CHOICES = 300,
    HTTP_STATUS_MOVED_PERMANENTLY = 301,
    HTTP_STATUS_FOUND = 302,
    HTTP_STATUS_SEE_OTHER = 303,
    HTTP_STATUS_NOT_MODIFIED = 304,
    HTTP_STATUS_USE_PROXY = 305,
    HTTP_STATUS_SWITCH_PROXY = 306,
    HTTP_STATUS_TEMPORARY_REDIRECT = 307,
    HTTP_STATUS_PERMANENT_REDIRECT = 308,
    HTTP_STATUS_BAD_REQUEST = 400,
    HTTP_STATUS_UNAUTHORIZED = 401,
    HTTP_STATUS_PAYMENT_REQUIRED = 402,
    HTTP_STATUS_FORBIDDEN = 403,
    HTTP_STATUS_NOT_FOUND = 404,
    HTTP_STATUS_METHOD_NOT_ALLOWED = 405,
    HTTP_STATUS_NOT_ACCEPTABLE = 406,
    HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED = 407,
    HTTP_STATUS_REQUEST_TIMEOUT = 408,
    HTTP_STATUS_CONFLICT = 409,
    HTTP_STATUS_GONE = 410,
    HTTP_STATUS_LENGTH_REQUIRED = 411,
    HTTP_STATUS_PRECONDITION_FAILED = 412,
    HTTP_STATUS_PAYLOAD_TOO_LARGE = 413,
    HTTP_STATUS_URI_TOO_LONG = 414,
    HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE = 415,
    HTTP_STATUS_RANGE_NOT_SATISFIABLE = 416,
    HTTP_STATUS_EXPECTATION_FAILED = 417,
    HTTP_STATUS_IM_A_TEAPOT = 418,
    HTTP_STATUS_PAGE_EXPIRED = 419,
    HTTP_STATUS_ENHANCE_YOUR_CALM = 420,
    HTTP_STATUS_MISDIRECTED_REQUEST = 421,
    HTTP_STATUS_UNPROCESSABLE_ENTITY = 422,
    HTTP_STATUS_LOCKED = 423,
    HTTP_STATUS_FAILED_DEPENDENCY = 424,
    HTTP_STATUS_TOO_EARLY = 425,
    HTTP_STATUS_UPGRADE_REQUIRED = 426,
    HTTP_STATUS_PRECONDITION_REQUIRED = 428,
    HTTP_STATUS_TOO_MANY_REQUESTS = 429,
    HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE_UNOFFICIAL = 430,
    HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    HTTP_STATUS_LOGIN_TIMEOUT = 440,
    HTTP_STATUS_NO_RESPONSE = 444,
    HTTP_STATUS_RETRY_WITH = 449,
    HTTP_STATUS_BLOCKED_BY_PARENTAL_CONTROL = 450,
    HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS = 451,
    HTTP_STATUS_CLIENT_CLOSED_LOAD_BALANCED_REQUEST = 460,
    HTTP_STATUS_INVALID_X_FORWARDED_FOR = 463,
    HTTP_STATUS_REQUEST_HEADER_TOO_LARGE = 494,
    HTTP_STATUS_SSL_CERTIFICATE_ERROR = 495,
    HTTP_STATUS_SSL_CERTIFICATE_REQUIRED = 496,
    HTTP_STATUS_HTTP_REQUEST_SENT_TO_HTTPS_PORT = 497,
    HTTP_STATUS_INVALID_TOKEN = 498,
    HTTP_STATUS_CLIENT_CLOSED_REQUEST = 499,
    HTTP_STATUS_INTERNAL_SERVER_ERROR = 500,
    HTTP_STATUS_NOT_IMPLEMENTED = 501,
    HTTP_STATUS_BAD_GATEWAY = 502,
    HTTP_STATUS_SERVICE_UNAVAILABLE = 503,
    HTTP_STATUS_GATEWAY_TIMEOUT = 504,
    HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED = 505,
    HTTP_STATUS_VARIANT_ALSO_NEGOTIATES = 506,
    HTTP_STATUS_INSUFFICIENT_STORAGE = 507,
    HTTP_STATUS_LOOP_DETECTED = 508,
    HTTP_STATUS_BANDWIDTH_LIMIT_EXCEEDED = 509,
    HTTP_STATUS_NOT_EXTENDED = 510,
    HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED = 511,
    HTTP_STATUS_WEB_SERVER_UNKNOWN_ERROR = 520,
    HTTP_STATUS_WEB_SERVER_IS_DOWN = 521,
    HTTP_STATUS_CONNECTION_TIMEOUT = 522,
    HTTP_STATUS_ORIGIN_IS_UNREACHABLE = 523,
    HTTP_STATUS_TIMEOUT_OCCURED = 524,
    HTTP_STATUS_SSL_HANDSHAKE_FAILED = 525,
    HTTP_STATUS_INVALID_SSL_CERTIFICATE = 526,
    HTTP_STATUS_RAILGUN_ERROR = 527,
    HTTP_STATUS_SITE_IS_OVERLOADED = 529,
    HTTP_STATUS_SITE_IS_FROZEN = 530,
    HTTP_STATUS_IDENTITY_PROVIDER_AUTHENTICATION_ERROR = 561,
    HTTP_STATUS_NETWORK_READ_TIMEOUT = 598,
    HTTP_STATUS_NETWORK_CONNECT_TIMEOUT = 599
};

struct http_strcasecmp_operator {
    bool operator()(const std::string& a, const std::string& b) const {
        return ::strcasecmp(a.c_str(), b.c_str()) < 0;
    }
};
class HttpHeader : public std::multimap<std::string, std::string, http_strcasecmp_operator>{
public:
    using Map = multimap<std::string, std::string, http_strcasecmp_operator>;
    HttpHeader() = default;
    ~HttpHeader() = default;

    std::string &operator[](const std::string &key) {
        auto it = find(key);
        if (it == end()) {
            it = Map::emplace(key, "");
        }
        return it->second;
    }
    template<typename V>
    void emplace(const std::string &key, V &&v) {
        auto it = find(key);
        if (it != end()) {
            return;
        }
        Map::emplace(key, std::forward<V>(v));
    }

    template<typename V>
    void emplace_force(const std::string key, V &&v) {
        Map::emplace(key, std::forward<V>(v));
    }

};

/////////////////////http_utility////////////////////////////////
std::string http_take_header(const std::string& key,const std::string& header_content);
std::string http_url_path(const std::string& url);
const char* http_method_name(http_method method);
/////////////////////http_utility////////////////////////////////


///////////////////////////RFC-2617 HTTP Authentication: Basic and Digest Access Authentication///////////////
namespace RFC2617{ 
    static constexpr const char kAuthChallenge[] = "WWW-Authenticate";
    static constexpr const char kAuthResponse[] = "Authorization";
    static constexpr const char kAuthBasic[] = "Basic";
    static constexpr const char kAuthDigest[] = "Digest";
    static constexpr const char kAuthRealm[] = "realm";
    static constexpr const char kAuthNonce[] = "nonce";
    static constexpr const char kAuthOpaque[] = "opaque";
    static constexpr const char kAuthStale[] = "stale";
    static constexpr const char kAuthAlgorithm[] = "algorithm";
    static constexpr const char kAuthQop[] = "qop";
    static constexpr const char kAuthCnonce[] = "cnonce";
    static constexpr const char kAuthNc[] = "nc";

    enum Authentication{
        NONE,
        BASIC,
        DIGEST
    };
    enum DigestAlgo{
        MD5,
        MD5_SESS,
        SHA256,
        SHA256_SESS,
        SHA512_256,
        SHA512_256_SESS
    };
    enum class DigestQop{
        NONE = 0,
        AUTH,
        AUTH_INT
    };
    
    struct DigestParams{ 
        bool stale;
        Authentication type;
        DigestAlgo algo;
        // nonce count
        int nc;
        RFC2617::DigestQop qop;
        bool userhash;
        char ncstr[9];
        std::string qopstr;
        std::string realm;
        std::string nonce;
        std::string opaque;
        std::string cnonce;
        std::string username;
        std::string password;

        void Clear();

        friend std::ostream& operator<<(std::ostream& os, const DigestParams& params);
    };
};

class HttpAuthenticator{ 
public:
    explicit HttpAuthenticator(const std::string& user,const std::string& pass);
    ~HttpAuthenticator();
    
    RFC2617::DigestParams& GetParams(){
        return parames_;
    }
    /// @brief Parse "WWW-Authenticate" 's content
    /// @param challenge 
    /// @return 
    RFC2617::DigestParams& ParseChallenge(const std::string& challenge);

    /// @brief Authorization: Basic xxxx
    /// @return 
    std::string AuthorizationBasic();

    /// @brief Authorization: Digest xxxx
    /// @param method 
    /// @param uri_path 
    /// @return 
    std::string AuthorizationDigest(http_method method,const std::string& uri_path);
private:
    std::string digest_auth(http_method method,const std::string& uri_path);

    RFC2617::DigestParams parames_;
};

///////////////////////////RFC-2617 HTTP Authentication: Basic and Digest Access Authentication///////////////

};//!namespace iom

#endif//!IOM_HTTP_H_