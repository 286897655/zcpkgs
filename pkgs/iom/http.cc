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

#include "http.h"
#include <string.h>
#include <zpkg/strings.h>
#include <zpkg/base64.h>
#include <zpkg/hash/hash.h>
#include <zpkg/randoms.h>
#include <llhttp/llhttp.h>

namespace iom { 
/////////////////////http_utility////////////////////////////////
std::string http_take_header(const std::string& key,const std::string& header_content){
    if(key.empty() || header_content.empty()){
        // empty
        return zpkg::kstrings::kEmpty;
    }

    struct parse_context {
        std::string cur_key;
        std::string cur_value;
        HttpHeader headers;
    } ctx;
    llhttp_settings_t settings;
    llhttp_settings_init(&settings);
    settings.on_header_field = [](llhttp_t* parser, const char* at, size_t length) -> int {
        // header field
        struct parse_context *ctx = static_cast<struct parse_context*>(parser->data);
        ctx->cur_key.append(at, length);
        return 0;
    };
    settings.on_header_field_complete = [](llhttp_t* parser) -> int {
        // header field complete
        return 0;
    };
    settings.on_header_value = [](llhttp_t* parser, const char* at, size_t length) -> int {
        // header value
        struct parse_context *ctx = static_cast<struct parse_context*>(parser->data);
        ctx->cur_value.append(at, length);
        return 0;
    };
    settings.on_header_value_complete = [](llhttp_t* parser) -> int {
        // header value complete
        struct parse_context *ctx = static_cast<struct parse_context*>(parser->data);
        if(!ctx->cur_key.empty()){
            ctx->headers.insert(std::make_pair(ctx->cur_key, ctx->cur_value));
        }
        ctx->cur_key.clear();
        ctx->cur_value.clear();
        return 0;
    };
    settings.on_headers_complete = [](llhttp_t* parser) -> int {
        // header complete
        return 0;
    };
    llhttp_t parser;
    llhttp_init(&parser, HTTP_BOTH, &settings);
    parser.data = &ctx;
    llhttp_errno_t error = llhttp_execute(&parser, header_content.c_str(), header_content.size());
    if(error != HPE_OK){
        // error
        return zpkg::kstrings::kEmpty;
    }
    return ctx.headers[key];
}

std::string http_url_path(const std::string& url){
    size_t pos = url.find("://");
    if(pos == std::string::npos) {
        // not with protocol
        return url;
    }
    // with protocol,skip protocol
    pos = url.find("/", pos + 3);
    if(pos == std::string::npos) {
        // root path
        return "/";
    }
    // /xxx/yyy/zzz
    return url.substr(pos);
}

const char* http_method_name(http_method method){
    static constexpr const char* names[] = {
        "DELETE",
        "GET",
        "HEAD",
        "POST",
        "PUT",
        "CONNECT",
        "OPTIONS",
        "TRACE",
        "COPY",
        "LOCK",
        "MKCOL",
        "MOVE",
        "PROPFIND",
        "PROPPATCH",
        "SEARCH",
        "UNLOCK",
        "BIND",
        "REBIND",
        "UNBIND",
        "ACL",
        "REPORT",
        "MKACTIVITY",
        "CHECKOUT",
        "MERGE",
        "MSEARCH",
        "NOTIFY",
        "SUBSCRIBE",
        "UNSUBSCRIBE",
        "PATCH",
        "PURGE",
        "MKCALENDAR",
        "LINK",
        "UNLINK",
        "SOURCE",
        "PRI",
        "DESCRIBE",
        "ANNOUNCE",
        "SETUP",
        "PLAY",
        "PAUSE",
        "TEARDOWN",
        "GET_PARAMETER",
        "SET_PARAMETER",
        "REDIRECT",
        "RECORD",
        "FLUSH",
        "QUERY"
    };
    return names[static_cast<int>(method)];
}

/////////////////////http_utility////////////////////////////////
#define DIGEST_MAX_KEY_LENGTH           256
#define DIGEST_MAX_CONTENT_LENGTH         1024

namespace{
// from curl-digest.c
static bool rfc2617_decode_pair(const char *str, char *key, char *content,
                                    const char **endptr){
    int c;
    bool starts_with_quote = false;
    bool escape = false;
    // write key until '=' or max length
    for(c = DIGEST_MAX_KEY_LENGTH - 1; (*str && (*str != '=') && c--);)
        *key++ = *str++;
    *key = 0;

    if('=' != *str++){
        // next is not '=' decode fail
        return false;
    }
    
    if('\"' == *str) {
        // This starts with a quote so it must end with one as well!
        str++;
        starts_with_quote = true;
    }

    for(c = DIGEST_MAX_CONTENT_LENGTH - 1; *str && c--; str++) {
        switch(*str) {
        case '\\': // with '\\' start
            if(!escape) {
                /* possibly the start of an escaped quote */
                escape = true;
                *content++ = '\\'; /* Even though this is an escape character, we still
                                    store it as-is in the target buffer */
                continue;
            }
            break;

        case ',':
            if(!starts_with_quote) {
                /* This signals the end of the content if we didn't get a starting
                quote and then we do "sloppy" parsing */
                c = 0; /* the end */
                continue;
            }
            break;

        case '\r':
        case '\n':
            /* end of string */
            c = 0;
            continue;

        case '\"':
            if(!escape && starts_with_quote) {
                /* end of string */
                c = 0;
                continue;
            }
            break;
        }

        escape = false;
        *content++ = *str;
    }

    *content = 0;
    *endptr = str;

    return true;
}

static bool rfc2617_decode_params(const char *str, RFC2617::DigestParams* params){
    for(;;) {
        char key[DIGEST_MAX_KEY_LENGTH];
        char content[DIGEST_MAX_CONTENT_LENGTH];

        // Pass all additional spaces here
        while(*str && std::isspace(*str))
            str++;

        // Extract a key=content pair
        if(rfc2617_decode_pair(str, key, content, &str)) {
            // decode pair success
            if(::strcasecmp(key, RFC2617::kAuthNonce) == 0) {
                params->nonce = content;
            }else if(::strcasecmp(key, RFC2617::kAuthStale) == 0) {
                if(::strcasecmp(content, "true") == 0) {
                    params->stale = true;
                    params->nc = 1;
                }
            }else if(::strcasecmp(key, RFC2617::kAuthRealm) == 0) {
                params->realm = content;
            }else if(::strcasecmp(key,RFC2617::kAuthOpaque) == 0) {
                params->opaque = content;
            }else if(::strcasecmp(key,RFC2617::kAuthQop) == 0) {
                params->qopstr = content;
                // qop to check auth/auth-int
                std::string qop = zpkg::strings::tolower(content);
                if(qop.find("auth-int") != std::string::npos){
                    params->qop = RFC2617::DigestQop::AUTH_INT;
                }else if(qop.find("auth") != std::string::npos){
                    params->qop = RFC2617::DigestQop::AUTH;
                }
            }else if(::strcasecmp(key, RFC2617::kAuthAlgorithm) == 0) {
                // algorithm to check md5/md5-sess/sha256/sha256-sess/sha512-256/sha512-256-sess
                std::string algorithm = zpkg::strings::tolower(content);
                if(algorithm.find("md5-sess") != std::string::npos){
                    params->algo = RFC2617::MD5_SESS;
                }else if(algorithm.find("md5") != std::string::npos){
                    params->algo = RFC2617::MD5;
                }else if(algorithm.find("sha256-sess") != std::string::npos){
                    params->algo = RFC2617::SHA256_SESS;
                }else if(algorithm.find("sha256") != std::string::npos){
                    params->algo = RFC2617::SHA256;
                }else if(algorithm.find("sha512-256-sess") != std::string::npos){
                    params->algo = RFC2617::SHA512_256_SESS;
                }else if(algorithm.find("sha512-256") != std::string::npos){
                    params->algo = RFC2617::SHA512_256;
                }
            }else if(::strcasecmp(key, "userhash") == 0) {
                if(::strcasecmp(content, "true") == 0){
                    params->userhash = true;
                }
            }else{
                /* Unknown specifier, ignore it! */
            }
        }else{// decode no key-value content
            break; /* We're done here */
        }
      

        /* Pass all additional spaces here */
        while(*str && std::isspace(*str))
            str++;

        /* Allow the list to be comma-separated */
        if(',' == *str)
            str++;
    }

    /* We had a nonce since before, and we got another one now without
     'stale=true'. This means we provided bad credentials in the previous
     request */
    // if(had_nonce && !params->stale){
    //     return false;
    // }
    /* We got this header without a nonce, that's a bad Digest line! */
    if(params->nonce.empty()){
        return false;
    }
    return true;
}
}

namespace RFC2617{ 

void DigestParams::Clear(){
    stale = false;
    nc = 1;
    qop = RFC2617::DigestQop::NONE;
    //default not auth
    type = RFC2617::Authentication::NONE;
    // defualt use md5
    algo = RFC2617::MD5;
    userhash = false;
    realm.clear();
    nonce.clear();
    qopstr.clear();
    opaque.clear();
    cnonce.clear();
}
std::ostream& operator<<(std::ostream& os, const DigestParams& params){
    static constexpr const char* kAuthenticationType[]={"NONE","BASIC","DIGEST"};
    static constexpr const char* kDigestAlgo[]={"MD5","MD5_SESS","SHA256","SHA256_SESS","SHA512_256","SHA512_256_SESS"};
    static constexpr const char* kQopType[] = {"NONE","AUTH","AUTH-INT"};
    os << "DigestParams:"
        << "  type:"  << kAuthenticationType[params.type]
        << "  algo:"  << kDigestAlgo[params.algo]
        << "  nc:"    << params.nc
        << "  qop:"   << kQopType[static_cast<int>(params.qop)]
        << "  qops:"  << params.qopstr
        << "  realm:" << params.realm
        << "  nonce:" << params.nonce
        << "  opaque:" << params.opaque
        << std::endl;
    return os;
}
}

HttpAuthenticator::HttpAuthenticator(const std::string& user,const std::string& pass){
    parames_.Clear();
    parames_.username = user;
    parames_.password = pass;
}
HttpAuthenticator::~HttpAuthenticator(){
    
}
RFC2617::DigestParams& HttpAuthenticator::ParseChallenge(const std::string& challenge){
    if(challenge.empty())
        return parames_;
    
    parames_.Clear();
    
    // check Basic or Digest
    if(challenge.find(RFC2617::kAuthBasic) != std::string::npos){
        parames_.type = RFC2617::BASIC;
        return parames_;
    }else if(challenge.find(RFC2617::kAuthDigest) != std::string::npos){
        parames_.type = RFC2617::DIGEST;
    }else{
        // not use authenticate 
        parames_.type = RFC2617::Authentication::NONE;
        return parames_;
    }
    // decode parameters
    std::string digest = challenge.substr(sizeof(RFC2617::kAuthDigest) - 1);
    if(!rfc2617_decode_params(digest.c_str(), &parames_)){
        parames_.Clear();
    }
    return parames_;
}

std::string HttpAuthenticator::AuthorizationBasic(){
    if(parames_.type != RFC2617::Authentication::BASIC){
        return zpkg::kstrings::kEmpty;
    }
    // Basic 认证：username:password  Base64 编码
    return "Basic " + zpkg::base64_encode(parames_.username + ":" + parames_.password);
}

std::string HttpAuthenticator::AuthorizationDigest(http_method method,const std::string& uri_path){
    if(parames_.type != RFC2617::Authentication::DIGEST){
        return zpkg::kstrings::kEmpty;
    }
    return "Digest " + digest_auth(method,uri_path);
}

static std::string md5_hash_cb(const std::string& str){
    return zpkg::hash::hash_hex(zpkg::hash::MD5,str);
}

static std::string sha256_hash_cb(const std::string& str){
    return zpkg::hash::hash_hex(zpkg::hash::SHA256,str);
}
std::string HttpAuthenticator::digest_auth(http_method method,const std::string& uri_path){
    // Digest 认证核心逻辑（RFC 2617 第 3.2.2 节）
    // generate conce
    if(parames_.qop != RFC2617::DigestQop::NONE){
        parames_.cnonce = zpkg::Random::TLSInstance().RandomHexString(16);
    }
    
    // realm and nonce must be set
    if(parames_.realm.empty() || parames_.nonce.empty()){
        return zpkg::kstrings::kEmpty;
    }
    std::string (*hash_cb)(const std::string&) = &md5_hash_cb;
    if(parames_.algo == RFC2617::DigestAlgo::SHA256 
        || parames_.algo == RFC2617::DigestAlgo::SHA512_256
        || parames_.algo == RFC2617::DigestAlgo::SHA256_SESS 
        || parames_.algo == RFC2617::DigestAlgo::SHA512_256_SESS){
        hash_cb = &sha256_hash_cb;
    }
    
    // 1. HA1
    std::string ha1;
    if(parames_.algo == RFC2617::DigestAlgo::MD5 || parames_.algo == RFC2617::DigestAlgo::SHA256
        || parames_.algo == RFC2617::DigestAlgo::SHA512_256){
        ha1 = hash_cb(parames_.username + ":" + parames_.realm + ":" + parames_.password);
    }else{
        // sess
        std::string tmp = hash_cb(parames_.username + ":" + parames_.realm + ":" + parames_.password);
        ha1 = hash_cb(tmp + ":" + parames_.nonce + ":" + parames_.cnonce);
    }
    // 2. HA2
    std::string ha2 = std::string(http_method_name(method)) + ":" + uri_path;
    if(parames_.qop == RFC2617::DigestQop::AUTH_INT){
        // auth-int should contains body entity
        ha2 += ":" + hash_cb("");
    }
    ha2 = hash_cb(ha2);
    // 3. response
    char nc[9] = {0};
    std::string response;
    if(parames_.qop == RFC2617::DigestQop::NONE){
        response = hash_cb(ha1 + ":" + parames_.nonce + ":" + ha2);
    }else{
        sprintf(nc,"%08x",parames_.nc);
        // nonce count should add
        parames_.nc++;

        response = hash_cb(ha1 + ":" + parames_.nonce + ":" + std::string(nc) + ":" + parames_.cnonce + ":" + parames_.qopstr + ":" + ha2);
    }
    // 4 build
    std::ostringstream oss;
    oss << "username=\"" << parames_.username << "\","
             << "realm=\"" << parames_.realm << "\","
             << "nonce=\"" << parames_.nonce << "\","
             << "uri=\"" << uri_path << "\","
             << "response=\"" << response << "\"";

    if (parames_.qop != RFC2617::DigestQop::NONE) {
        oss << ","
            << "qop=\"" << parames_.qopstr << "\","
            << "cnonce=\"" << parames_.cnonce << "\","
            << "nc=\"" << nc << "\"";
    }

    if (!parames_.opaque.empty()) {
        oss << ","
            << "opaque=\"" << parames_.opaque << "\"";
    }

    return oss.str();
}

};//!namespace iom