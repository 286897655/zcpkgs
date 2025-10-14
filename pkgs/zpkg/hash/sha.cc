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

#include "hash.h"
#include "zpkg/utility.h"
#include <string.h>

namespace zpkg{
namespace hash{
/** SHA hash context */
typedef struct SHA_CTX{
    uint8_t  digest_len;  ///< digest length in 32-bit words
    uint64_t count;       ///< number of bytes in buffer
    uint8_t  buffer[64];  ///< 512-bit buffer of input values used in hash updating
    uint32_t state[8];    ///< current hash value
    /** function used to update hash for 512-bit input block */
    void     (*transform)(uint32_t *state, const uint8_t buffer[64]);
} SHA_CTX;


#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define blk0(i) (block[i] = Z_RBE32(buffer + 4 * (i)))
#define blk(i)  (block[i] = rol(block[(i)-3] ^ block[(i)-8] ^ block[(i)-14] ^ block[(i)-16], 1))

#define R0(v,w,x,y,z,i) z += (((w)&((x)^(y)))^(y))       + blk0(i) + 0x5A827999 + rol(v, 5); w = rol(w, 30);
#define R1(v,w,x,y,z,i) z += (((w)&((x)^(y)))^(y))       + blk (i) + 0x5A827999 + rol(v, 5); w = rol(w, 30);
#define R2(v,w,x,y,z,i) z += ( (w)^(x)       ^(y))       + blk (i) + 0x6ED9EBA1 + rol(v, 5); w = rol(w, 30);
#define R3(v,w,x,y,z,i) z += ((((w)|(x))&(y))|((w)&(x))) + blk (i) + 0x8F1BBCDC + rol(v, 5); w = rol(w, 30);
#define R4(v,w,x,y,z,i) z += ( (w)^(x)       ^(y))       + blk (i) + 0xCA62C1D6 + rol(v, 5); w = rol(w, 30);

/* Hash a single 512-bit block. This is the core of the algorithm. */
static void sha1_transform(uint32_t state[5], const uint8_t buffer[64])
{
    uint32_t block[80];
    unsigned int i, a, b, c, d, e;

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];

#define R1_0 \
    R0(a, b, c, d, e, 0 + i); \
    R0(e, a, b, c, d, 1 + i); \
    R0(d, e, a, b, c, 2 + i); \
    R0(c, d, e, a, b, 3 + i); \
    R0(b, c, d, e, a, 4 + i); \
    i += 5

    i = 0;
    R1_0; R1_0; R1_0;
    R0(a, b, c, d, e, 15);
    R1(e, a, b, c, d, 16);
    R1(d, e, a, b, c, 17);
    R1(c, d, e, a, b, 18);
    R1(b, c, d, e, a, 19);

#define R1_20 \
    R2(a, b, c, d, e, 0 + i); \
    R2(e, a, b, c, d, 1 + i); \
    R2(d, e, a, b, c, 2 + i); \
    R2(c, d, e, a, b, 3 + i); \
    R2(b, c, d, e, a, 4 + i); \
    i += 5

    i = 20;
    R1_20; R1_20; R1_20; R1_20;

#define R1_40 \
    R3(a, b, c, d, e, 0 + i); \
    R3(e, a, b, c, d, 1 + i); \
    R3(d, e, a, b, c, 2 + i); \
    R3(c, d, e, a, b, 3 + i); \
    R3(b, c, d, e, a, 4 + i); \
    i += 5

    R1_40; R1_40; R1_40; R1_40;

#define R1_60 \
    R4(a, b, c, d, e, 0 + i); \
    R4(e, a, b, c, d, 1 + i); \
    R4(d, e, a, b, c, 2 + i); \
    R4(c, d, e, a, b, 3 + i); \
    R4(b, c, d, e, a, 4 + i); \
    i += 5

    R1_60; R1_60; R1_60; R1_60;

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
}

static const uint32_t K256[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


#define Ch(x,y,z)   (((x) & ((y) ^ (z))) ^ (z))
#define Maj(z,y,x)  ((((x) | (y)) & (z)) | ((x) & (y)))

#define Sigma0_256(x)   (rol((x), 30) ^ rol((x), 19) ^ rol((x), 10))
#define Sigma1_256(x)   (rol((x), 26) ^ rol((x), 21) ^ rol((x),  7))
#define sigma0_256(x)   (rol((x), 25) ^ rol((x), 14) ^ ((x) >> 3))
#define sigma1_256(x)   (rol((x), 15) ^ rol((x), 13) ^ ((x) >> 10))

#undef blk
#define blk(i)  (block[i] = block[i - 16] + sigma0_256(block[i - 15]) + \
                            sigma1_256(block[i - 2]) + block[i - 7])

#define ROUND256(a,b,c,d,e,f,g,h)   \
    T1 += (h) + Sigma1_256(e) + Ch((e), (f), (g)) + K256[i]; \
    (d) += T1; \
    (h) = T1 + Sigma0_256(a) + Maj((a), (b), (c)); \
    i++

#define ROUND256_0_TO_15(a,b,c,d,e,f,g,h)   \
    T1 = blk0(i); \
    ROUND256(a,b,c,d,e,f,g,h)

#define ROUND256_16_TO_63(a,b,c,d,e,f,g,h)   \
    T1 = blk(i); \
    ROUND256(a,b,c,d,e,f,g,h)

static void sha256_transform(uint32_t *state, const uint8_t buffer[64])
{
    unsigned int i, a, b, c, d, e, f, g, h;
    uint32_t block[64];
    uint32_t T1;

    a = state[0];
    b = state[1];
    c = state[2];
    d = state[3];
    e = state[4];
    f = state[5];
    g = state[6];
    h = state[7];

    i = 0;
#define R256_0 \
    ROUND256_0_TO_15(a, b, c, d, e, f, g, h); \
    ROUND256_0_TO_15(h, a, b, c, d, e, f, g); \
    ROUND256_0_TO_15(g, h, a, b, c, d, e, f); \
    ROUND256_0_TO_15(f, g, h, a, b, c, d, e); \
    ROUND256_0_TO_15(e, f, g, h, a, b, c, d); \
    ROUND256_0_TO_15(d, e, f, g, h, a, b, c); \
    ROUND256_0_TO_15(c, d, e, f, g, h, a, b); \
    ROUND256_0_TO_15(b, c, d, e, f, g, h, a)

    R256_0; R256_0;

#define R256_16 \
    ROUND256_16_TO_63(a, b, c, d, e, f, g, h); \
    ROUND256_16_TO_63(h, a, b, c, d, e, f, g); \
    ROUND256_16_TO_63(g, h, a, b, c, d, e, f); \
    ROUND256_16_TO_63(f, g, h, a, b, c, d, e); \
    ROUND256_16_TO_63(e, f, g, h, a, b, c, d); \
    ROUND256_16_TO_63(d, e, f, g, h, a, b, c); \
    ROUND256_16_TO_63(c, d, e, f, g, h, a, b); \
    ROUND256_16_TO_63(b, c, d, e, f, g, h, a)

    R256_16; R256_16; R256_16;
    R256_16; R256_16; R256_16;

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    state[4] += e;
    state[5] += f;
    state[6] += g;
    state[7] += h;
}


sha::sha(algorithm algorithm){
    sha_ctx_ = new SHA_CTX();
    reset(algorithm);
}

sha::~sha(){
    delete sha_ctx_;
    sha_ctx_ = nullptr;
}

void sha::reset(algorithm algorithm){
    // 5 bytes 8 bytes
    sha_ctx_->digest_len = static_cast<uint32_t>(algorithm) >> 5;
    switch (algorithm)
    {
        case SHA1:
            sha_ctx_->state[0] = 0x67452301;
            sha_ctx_->state[1] = 0xEFCDAB89;
            sha_ctx_->state[2] = 0x98BADCFE;
            sha_ctx_->state[3] = 0x10325476;
            sha_ctx_->state[4] = 0xC3D2E1F0;
            sha_ctx_->transform = sha1_transform;
        break;
        case SHA256:
            sha_ctx_->state[0] = 0x6A09E667;
            sha_ctx_->state[1] = 0xBB67AE85;
            sha_ctx_->state[2] = 0x3C6EF372;
            sha_ctx_->state[3] = 0xA54FF53A;
            sha_ctx_->state[4] = 0x510E527F;
            sha_ctx_->state[5] = 0x9B05688C;
            sha_ctx_->state[6] = 0x1F83D9AB;
            sha_ctx_->state[7] = 0x5BE0CD19;
            sha_ctx_->transform = sha256_transform;
            break;
        default:
            Z_ASSERT(0);
            break;
    }
    sha_ctx_->count = 0;
}

void sha::update(const uint8_t* bytes,size_t byteSize){
    size_t i, j;

    j = sha_ctx_->count & 63;
    sha_ctx_->count += byteSize;

    if ((j + byteSize) > 63) {
        memcpy(&sha_ctx_->buffer[j], bytes, (i = 64 - j));
        sha_ctx_->transform(sha_ctx_->state, sha_ctx_->buffer);
        for (; i + 63 < byteSize; i += 64)
            sha_ctx_->transform(sha_ctx_->state, &bytes[i]);
        j = 0;
    } else
        i = 0;
    memcpy(&sha_ctx_->buffer[j], &bytes[i], byteSize - i);
}

void sha::update(const std::string& str){
    update(reinterpret_cast<const uint8_t*>(str.c_str()),str.size());
}

std::string sha::hash_bin(){
    int i;
    uint64_t finalcount = Z_BE2HE64C(sha_ctx_->count << 3);

    update(reinterpret_cast<const uint8_t*>("\200"), 1);
    while ((sha_ctx_->count & 63) != 56)
        update(reinterpret_cast<const uint8_t*>(""), 1);
    update((uint8_t *)&finalcount, 8); /* Should cause a transform() */
    char buf[64]={0};
    for (i = 0; i < sha_ctx_->digest_len; i++)
        Z_WBE32(buf + i*4, sha_ctx_->state[i]);
    
    return std::string(buf);
}

std::string sha::hash() {
    std::string str = hash_bin();
    std::ostringstream result;

    for (size_t i = 0; i < str.size(); i++)
    {
        char b[3];
        ::sprintf(b, "%02x", static_cast<unsigned char>(str[i]));
        result << b;
    }

    return result.str();
}

std::string sha::hash(algorithm algorithm,const uint8_t* bytes,size_t byteSize){
    sha sha(algorithm);
    sha.update(bytes,byteSize);
    return sha.hash();
}

std::string sha::hash_bin(algorithm algorithm,const uint8_t* bytes,size_t byteSize){
    sha sha(algorithm);
    sha.update(bytes,byteSize);
    return sha.hash_bin();
}

std::string sha::hash(algorithm algorithm,const std::string& str){
    sha sha(algorithm);
    sha.update(reinterpret_cast<const uint8_t*>(str.c_str()),str.size());
    return sha.hash();
}

std::string sha::hash_bin(algorithm algorithm,const std::string& str){
    sha sha(algorithm);
    sha.update(reinterpret_cast<const uint8_t*>(str.c_str()),str.size());
    return sha.hash_bin();
}
};//!namesapce hash
};//!namespace zpkg