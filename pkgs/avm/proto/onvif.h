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

#ifndef AVM_PROTO_ONVIF_H_
#define AVM_PROTO_ONVIF_H_

#include <string>
#include <memory>
#include <ctime>

namespace avm{

struct soap{
    static constexpr const char kSOAP_HTTP_CONTENT_TYP[] = "application/soap+xml; charset=utf-8";
    static std::string format_soap_message(const std::string& message);
};

struct onvif_soap
{
    /**
     * ONVIF-Core-Specification.pdf
     * 5.8.2.2 Generic faults
     * Table 4: Generic faults
    */
    enum Faults{
        // SOAP has no error
        OK = 0,
        ////////////////////////////Fault Code env://///////////////////////////
        // Reason: SOAP version mismatch
        // Description: the device found an invalid element information item instead of the expected Envelope element information item.
        VersionMismatch,
        // Reason: SOAP header blocks not understood
        // Description: One or more mandatory SOAP headers blocks were not understood
        MustUnderstand,
        // Reason: Unsupported SOAP data encoding
        // Description: SOAP header block or SOAP body child element information item is scoped with data encoding that is not supported by the device
        DataEncodingUnknown,
        ////////////////////////////Fault Code env://///////////////////////////
        ////////////////////////////Fault Code env:Sender /////////////////////////////
        // Reason: Well-Formed error
        // Description: XML Well-Formed violation occured
        WellFormed,
        // Reason: Tag Mismatch
        // Description: There was a tag name or namespace mismatch
        TagMismatch,
        // Reason: No Tag
        // Description: XML element tag was missing
        Tag,
        // Reason: Namespace Error
        // Description: SOAP Namespace error occured
        Namespace,
        // Reason: Required Attribute not present
        // Description: There was a missing required attribute
        MissingAttr,
        // Reason: Prohibited Attribute
        // Description: A prohibited attribute was present
        ProhibAttr,
        // Reason: Invalid Args
        // Description: An error due to any of the following:
        //              missing argument
        //              too many arguments
        //              arguments are of the wrong data type
        InvalidArgs,
        // Reason: Argument Value Invalid
        // Description: The argument value is invalid
        InvalidArgVal,
        // Reason: Unknown Action
        // Description: An unknown action is specified
        UnknownAction,
        // Reason: Operation not Permitted
        // Description: The requested operation is not permitted by the device
        OperationProhibited,
        // Reason: Sender not Authorized
        // Description: The action requested requires authorization and the sender is not authorized
        NotAuthorized,
        ////////////////////////////Fault Code env:Sender /////////////////////////////
        ////////////////////////////Fault Code env:Receiver /////////////////////////////
        // Reason: Optional Action Not Implemented
        // Description: The requested action is optional and is not implemented by the device
        ActionNotSupported,
        // Reason: Action Failed 
        // Description: The requested SOAP action failed
        Action,
        // Reason: Out of memory
        // Description: The device does not have sufficient memory to complete the action
        OutofMemory,
        // Reason: Critical Error
        // Description: The device has encountered and error condition which it cannot recover by itself and needs reset or power cycle
        CriticalError
        ////////////////////////////Fault Code env:Receiver /////////////////////////////
    };
    static std::string faluts_reason(Faults fault);

    Faults Fault();
    bool IsOK();
    std::string Reason();

    Faults fault_code = Faults::OK;

    // 非explicit的单参构造来隐式转换方便代码编写
    onvif_soap(Faults code):fault_code(code){};

    bool operator==(Faults code){
        return fault_code == code;
    }

    bool operator!=(Faults code){
        return fault_code != code;
    }
};

// constexpr size_t sz_onvif_soap = sizeof(onvif_soap);

struct onvif_device_params{
    std::string xaddr;
    std::string usr;
    std::string pwd;
};
struct onvif_device_infomation{
    std::string Manufacturer;
    std::string Model;
    std::string FirmwareVersion;
    std::string SerialNumber;
    std::string HardwareId;
    int Authentication;
};
struct onvif_device_system{
    // time diff of local
    std::time_t utc_diff;
};

// Authentication over HTTP and HTTPS
// The services defined in this standard, whenever consumed overt HTTP and HTTPS, shall be protected using
// digest authentication according to [RFC 2617] with the following exceptions.
// • Legacy devices supporting [WS-UsernameToken] and
// • TLS client authorization and
// • Devices supporting JWT client authorization based on [RFC 6750], only over HTTPS.
// If server supports both digest authentication as specified in [RFC 2617] and the user name token profile as
// specified in WS-Security the following behavior shall be adapted: a web service request can be authenticated
// on the HTTP level via digest authentication [RFC 2617] or on the web service level via the WS-Security (WSS)
// framework. If a client does not supply authentication credentials along with a web service request, the server
// shall assume that the client intends to use digest authentication [RFC 2617], if required. Hence, if a client does
// not provide authentication credentials when requesting a service that requires authentication, it will receive an
// HTTP 401 error according to [RFC 2617]. Note that this behaviour on the server’s side differs from the case
// of supporting only username token profile, which requires for this case an HTTP 400 error on the HTTP level
// and a SOAP:Fault env:Sender ter:NotAuthorized error on the WS level.
// A client should not simultaneously supply authentication credentials on both the HTTP level and the WS level.
// If a server receives a web service request that contains authentication credentials on both the HTTP level and
// the WS level, it shall first validate the credentials provided on the HTTP layer. If this validation was successful,
// the server shall finally validate the authentication credentials provided on the WS layer.
// Both digest authentication and WS-Security provide only a rudimentary level of security. In a system where
// security is important, it is recommended to always configure the device for TLS-based access (see Security
// Service). Digest authentication or WS-Security messages can be combined with TLS security for a client and
// server authentication. A protected transport level of security will provide an acceptable level of security in many
// systems.
// JWT client authorization should only be used over TLS secured connections, in order to protect bearer tokens
// against replay attacks.
// An ONVIF compliant device should authenticate an RTSP request at the RTSP level. If HTTP is used to tunnel
// the RTSP request the device shall not authenticate on the HTTP level.
// When authenticating RTSP or HTTP methods, an ONVIF compliant device shall use digest authentication
// [RFC 2617] or JWT-based authorization. The credentials shall be managed with the GetUsers, CreateUsers,
// DeleteUsers and SetUser methods. If the device also supports WS-Security, the same set of credentials shall
// be used.
enum OnvifAuth{
    None = 0,
    WS_UsernameToken = 1 << 0,//lagacy type
    Http_Basic = 1 << 1,// Http Basic base64 
    Http_Digest = 1 << 2 // Http Digest md5 or sha256
};

class onvif_exception{

};

/**
 * onvif proxy use to define 
 * a onvif client proxy request and response parse
*/
class onvif_proxy final{
public:
    // default 80 port and don't use https http://ip:80
    // if port is 443,use https https://ip:443
    // other port if use https https://ip:port
    explicit onvif_proxy(const std::string& usr,const std::string& pwd,const std::string& ip,int port = 80,bool use_https = false);
    // tiny_onvif pimpl pattern,onvif_proxy can't use default destructor
     ~onvif_proxy();

    // reset device proxy
    // do not reset device_params,reset device_infomation
    void reset();

    /**
     * An ONVIF compliant device shall provide the device management and event service
     * 至少包含device_service和event_service 其他能力通过GetService和GetCapabilitis判断获取
     * 
    */
    class device_service_proxy{
    public:
        std::unique_ptr<onvif_device_infomation> device_info_;
        std::unique_ptr<onvif_device_system> device_system_;
    public:
        explicit device_service_proxy(onvif_proxy* const proxy);
        std::string XAddr() const;

        // GetScopes
        // This operation requests the scope parameters of a device. The scope parameters are used in the device
        // discovery to match a probe message, see Section 7. The Scope parameters are of two different types:
        // • Fixed
        // • Configurable
        // Fixed scope parameters are permanent device characteristics and cannot be removed through the device
        // management interface. The scope type is indicated in the scope list returned in the get scope parameters
        // response. A device shall support retrieval of discovery scope parameters through the GetScopes command.
        // As some scope parameters are mandatory, the device shall return a non-empty scope list in the response
        std::string proxy_GetScopes() const;
        onvif_soap parse_GetScopes(const std::string& content);
        ////////////////////Capabilities//////////////////////////////
        // GetWsdlUrl deprecated since 20.12
        // std::string GetWsdlUrl();
        // void proxy_GetWsdlUrl(const std::string& content);
        std::string proxy_GetServices(bool IncludeCapability) const;
        onvif_soap parse_GetServices(const std::string& content);
        /**
         * GetCapabilities this method has been replaced by the more generic GetServices method. 
         * For capabilities of individual services refer to the GetServiceCapabilities methods
        */
        std::string proxy_GetCapabilities() const;
        onvif_soap parse_GetCapabilities(const std::string& content);
        ////////////////////Capabilities//////////////////////////////
        
        ////////////////////System//////////////////////////////
        std::string proxy_GetSystemDateAndTime() const;
        onvif_soap parse_GetSystemDateAndTime(const std::string& content);
        std::string proxy_GetDeviceInformation(bool with_wss) const;
        onvif_soap parse_GetDeviceInformation(const std::string& content);
        std::string proxy_GetDeviceAuthentication() const;
        onvif_soap parse_GetDeviceAuthentication(const std::string& header,const std::string& content);

        ////////////////////System//////////////////////////////

        ////////////////////Network//////////////////////////////
        ////////////////////Network//////////////////////////////
        ////////////////////Security//////////////////////////////
        ////////////////////Security//////////////////////////////
    private:
        onvif_proxy* const proxy_;
    };
    device_service_proxy* device_service();
    class media_service_proxy{
    public:
        explicit media_service_proxy(onvif_proxy* const proxy);
        const std::string XAddr() const;
        // GetVideoSources 和 GetVideoSourceConfigurations的区别
        // 从宇视nvr155来看GetVideoSources把001-008通道都返回了
        // GetVideoSourceConfigurations只返回了在线的003和005通道
        std::string proxy_GetVideoSources() const;
        onvif_soap parse_GetVideoSources(const std::string& content);
        std::string proxy_GetVideoSourceConfigurations() const;
        onvif_soap parse_GetVideoSourceConfigurations(const std::string& content);
        // This operation lists all existing OSD configurations for the device. The device shall support the listing of existing
        // OSD configurations through the GetOSDs command
        // REQUEST VideoSourceConfigurationToken optional [tt:ReferenceToken]
        std::string proxy_GetOSDs(const std::string& vsctoken) const;
        onvif_soap parse_GetOSDs(const std::string& content);
        std::string proxy_GetProfiles() const;
        onvif_soap parse_GetProfiles(const std::string& content);
        std::string proxy_GetStreamUri(const std::string& profiletoken) const;
        onvif_soap parse_GetStreamUri(const std::string& content);
    private:
        onvif_proxy* const proxy_;
    };
    media_service_proxy* media_service();

private:
    class tiny_onvif;
    std::unique_ptr<tiny_onvif> tiny_;
    std::unique_ptr<onvif_device_params> device_params_;
    std::unique_ptr<device_service_proxy> device_service_proxy_;
    std::unique_ptr<media_service_proxy> media_service_proxy_;
};

/**
 * onvif service use to define a onvif server
*/
class onvif_service{

};

};//!namespace avm

#endif//!AVM_PROTO_ONVIF_H_