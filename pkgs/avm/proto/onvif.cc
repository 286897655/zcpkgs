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

#include "onvif.h"
#include <zlog/log.h>
#include <zpkg/times.h>
#include <zpkg/base64.h>
#include <zpkg/hash/hash.h>
#include <tinyxml2/tinyxml2.h>
#include <pugixml/pugixml.hpp>
#include <llhttp/llhttp.h>

namespace avm{

namespace onvif{
    static constexpr const char kDEVICE_SERVICE_PATH[] = "/onvif/device_service";
    static constexpr const char kMEDIA_SERVICE_PATH[] = "/onvif/media_service";
    static constexpr const char kEVENT_SERVICE_PATH[] = "/onvif/event_service";
    // s:Envelope
    // soap:Envelope
    // SOAP-ENV:Envelope
    static constexpr const char kSOAP_ENVELOPE[] = "s:Envelope";//use simple
    static constexpr const char kSOAP_ENVELOPE_NAMESPACE[] = "xmlns:s";
    static constexpr const char kSOAP_ENVELOPE_NAMESPACE_URI[] = "http://www.w3.org/2003/05/soap-envelope";

    // soap Header
    // s:Header
    static constexpr const char kSOAP_HEADER[] = "s:Header";

    // soap Body
    // s:Body
    static constexpr const char kSOAP_BODY[] = "s:Body";

    /**
     * xml namespace
     * Onvif Core Specification
     * 
     * Prefix      Namespace URI                            Description
     * tt          http://www.onvif.org/ver10/schema        XML schema descriptions in this specification.
     * tds         http://www.onvif.org/ver10/device/wsdl   The namespace for the WSDL device service
     * trt         http://www.onvif.org/ver10/media/wsdl    The namespace for the WSDL media service.
     * tev         http://www.onvif.org/ver10/events/wsdl   The namespace for the WSDL event service.
     * ter         http://www.onvif.org/ver10/error         The namespace for ONVIF defined faults
     * dn          http://www.onvif.org/ver10/network/wsdl  The namespace used for the remote device discovery service in this specification.
     * tns1        http://www.onvif.org/ver10/topics        The namespace for the ONVIF topic namespace
    */
    static constexpr const char kSOAP_TT_NAMESPACE[] = "xmlns:tt";
    static constexpr const char kSOAP_TT_NAMESPACE_URI[] = "http://www.onvif.org/ver10/schema";
    static constexpr const char kSOAP_TDS_NAMESPACE[] = "xmlns:tds";
    static constexpr const char kSOAP_TDS_NAMESPACE_URI[] = "http://www.onvif.org/ver10/device/wsdl";
    static constexpr const char kSOAP_TRT_NAMESPACE[] = "xmlns:trt";
    static constexpr const char kSOAP_TRT_NAMESPACE_URI[] = "http://www.onvif.org/ver10/media/wsdl";
    static constexpr const char kSOAP_TEV_NAMESPACE[] = "xmlns:tev";
    static constexpr const char kSOAP_TEV_NAMESPACE_URI[] = "http://www.onvif.org/ver10/event/wsdl";
    static constexpr const char kSOAP_TER_NAMESPACE_URI[] = "http://www.onvif.org/ver10/error";
    static constexpr const char kSOAP_DN_NAMESPACE_URI[] = "http://www.onvif.org/ver10/network/wsdl";
    static constexpr const char kSOAP_TNS1_NAMESAPCE_URI[] = "http://www.onvif.org/ver10/topics";

    namespace tds{
        
    }

    static constexpr const char* kSOAP_FAULTS_REASION[]={
            "OK",
            "SOAP version mismatch",
            "SOAP header blocks not understood",
            "Unsupported SOAP data encoding",
            "Well-Formed error",
            "Tag Mismatch",
            "No Tag",
            "Namespace Error",
            "Required Attribute not present",
            "Prohibited Attribute",
            "Invalid Args",
            "Argument Value Invalid",
            "Unknown Action",
            "Operation not Permitted",
            "Sender not Authorized",
            "Optional Action Not Implemented",
            "Action Failed",
            "Out of memory",
            "Critical Error"
    };

    static void tds_soap_envelope_attr(tinyxml2::XMLElement* envelope){
        // set tds envelope attribute
        envelope->SetAttribute(onvif::kSOAP_ENVELOPE_NAMESPACE,onvif::kSOAP_ENVELOPE_NAMESPACE_URI);
        envelope->SetAttribute(onvif::kSOAP_TDS_NAMESPACE,onvif::kSOAP_TDS_NAMESPACE_URI);
        envelope->SetAttribute(onvif::kSOAP_TT_NAMESPACE,onvif::kSOAP_TT_NAMESPACE_URI);
    };

    static void trt_sop_envelope_attr(tinyxml2::XMLElement* envelope){
        // set trt envelope attribute
        envelope->SetAttribute(onvif::kSOAP_ENVELOPE_NAMESPACE,onvif::kSOAP_ENVELOPE_NAMESPACE_URI);
        envelope->SetAttribute(onvif::kSOAP_TRT_NAMESPACE,onvif::kSOAP_TRT_NAMESPACE_URI);
        envelope->SetAttribute(onvif::kSOAP_TT_NAMESPACE,onvif::kSOAP_TT_NAMESPACE_URI);
    };

    struct wsse_pwd_digest{
        std::string nonce;
        std::string pwd_digest;
        std::string created;
    };
    static std::unique_ptr<wsse_pwd_digest> wsse_digest(const std::string& pwd,std::time_t utc_offset){
        // nonce createat password 
        // 使用当前时间作为随机种子
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<size_t> dist(0, INT_MAX);
        // nonce 20 位 5个随机int
        char nonce_buffer[21]={0};
        for(int i = 0; i < 5;i++){
            Z_WBE32(nonce_buffer+i*4,dist(rng));
        }
        std::string nonce_base64 = zpkg::base64_encode(std::string(nonce_buffer));
        // 时间需要UTC标准时 ‘2025-10-11T06:13:31Z’含毫秒格式
        std::string created_str;
        {
            z_time_t now_time = zpkg::times::system_clock_ms();
            time_t now_s = now_time / 1000;
            int now_ms = now_time % 1000;
            time_t now_time_diff = now_s + utc_offset;//加上系统差值
            
            std::string diff_str = zpkg::ctime::fmt_timet(now_time_diff,"%Y-%m-%dT%H:%M:%S");
            char mill_buffer[32]={0};
            sprintf(mill_buffer,".%03dZ",now_ms);
            created_str = diff_str + std::string(mill_buffer);
        }
        // Password_Digest = Base64 ( SHA-1 ( nonce + created + password ) ) 
        std::string pwd_digest = zpkg::base64_encode(zpkg::hash::sha::hash_bin(zpkg::hash::SHA1,(std::string(nonce_buffer)+created_str+pwd)));
        
        return std::make_unique<wsse_pwd_digest>(
                wsse_pwd_digest{
                    .nonce = nonce_base64,
                    .pwd_digest = pwd_digest,
                    .created = created_str});
    };
}

std::string soap::format_soap_message(const std::string& message){
    tinyxml2::XMLDocument doc;
    doc.Parse(message.c_str());

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);

    return printer.CStr();
}

std::string onvif_soap::faluts_reason(Faults fault){
    return onvif::kSOAP_FAULTS_REASION[static_cast<int>(fault)];
}

onvif_soap::Faults onvif_soap::Fault(){
    return fault_code;
}

bool onvif_soap::IsOK(){
    return fault_code == Faults::OK;
}

std::string onvif_soap::Reason(){
    return faluts_reason(fault_code);
}

/**
 *                  table 7: Default Access Policy Definition
 *                          Administrator   Operator    User     Anonymous
 * PRE_AUTH                        X            X         X          X
 * READ_SYSTEM                     X            X         X
 * READ_SYSTEM_SENSITIVE           X            X
 * READ_SYSTEM_SECRET              X
 * WRITE_SYSTEM                    X
 * UNRECOVERABLE                   X
 * READ_MEDIA                      X            X         X
 * ACTUATE                         X            X
*/


class onvif_proxy::tiny_onvif{
public:
    explicit tiny_onvif(onvif_proxy* const proxy):proxy_(proxy){
        xaddr_device_service_ = proxy_->device_params_->xaddr + onvif::kDEVICE_SERVICE_PATH;
        xaddr_media_service_ = proxy_->device_params_->xaddr + onvif::kMEDIA_SERVICE_PATH;
    }
    ~tiny_onvif(){

    }

    const std::string& device_service() const{
        return xaddr_device_service_;
    }

    // PRE_AUTH do not authenticate
    std::string GetSystemDateAndTimeRequest() const{
        tinyxml2::XMLDocument doc;
        // insert xml declaration
        doc.InsertEndChild(doc.NewDeclaration());
        // envelope node
        tinyxml2::XMLElement* envelope_node = doc.NewElement(onvif::kSOAP_ENVELOPE);
        doc.InsertEndChild(envelope_node);
        // set envelope attribute
        onvif::tds_soap_envelope_attr(envelope_node);
        // <s:Header> do not use to auth
        soap_envolope_add_empty_header(envelope_node);
        //  <s:Body>
        //      <tds:GetSystemDateAndTime />
        //  </s:Body>
        envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("tds:GetSystemDateAndTime");

        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    }

    onvif_soap GetSystemDateAndTimeResponse(const std::string& content){
        pugi::xml_document doc;
        auto result = doc.load_string(content.c_str());
        pugi::xpath_query query("//tds:GetSystemDateAndTimeResponse");
        auto querynode = query.evaluate_node_set(doc);
        if(querynode.empty()){
            zlog("GetSystemDateAndTime Response no tds:GetSystemDateAndTimeResponse");
            // 理论上这个应该是必须回复的
            zlog("GetSystemDateAndTime Response:\n{}",content);
            
            return onvif_soap::InvalidArgs;
        }
        pugi::xpath_node utc_date_node = doc.select_node("//tt:UTCDateTime//tt:Date");
        pugi::xpath_node utc_time_node = doc.select_node("//tt:UTCDateTime//tt:Time");

        std::tm tm_device={0};
        tm_device.tm_year = std::atoi(utc_date_node.node().child_value("tt:Year")) - 1900;
        tm_device.tm_mon = std::atoi(utc_date_node.node().child_value("tt:Month")) - 1;
        tm_device.tm_mday = std::atoi(utc_date_node.node().child_value("tt:Day"));
        tm_device.tm_hour = std::atoi(utc_time_node.node().child_value("tt:Hour"));
        tm_device.tm_min = std::atoi(utc_time_node.node().child_value("tt:Minute"));
        tm_device.tm_sec = std::atoi(utc_time_node.node().child_value("tt:Second"));
        std::time_t utc_device = zpkg::ctime::tm2timet(&tm_device);
        std::time_t utc_local = zpkg::ctime::utc_timet();
        
        proxy_->device_service_proxy_->device_system_->utc_diff = utc_device - utc_local;

        return onvif_soap::OK;
    }

    // READ_SYSTEM,should authentication
    std::string GetDeviceInformationRequest(bool with_wss) const{
        tinyxml2::XMLDocument doc;
        // insert xml declaration
        doc.InsertEndChild(doc.NewDeclaration());
        // envelope node
        tinyxml2::XMLElement* envelope_node = doc.NewElement(onvif::kSOAP_ENVELOPE);
        doc.InsertEndChild(envelope_node);
        // set envelope attribute
        onvif::tds_soap_envelope_attr(envelope_node);
        if(with_wss){
            // <s:Header> add Auth Header
            soap_envolope_add_ws_auth_header(envelope_node);
        }else{
            // <s:Header> empty soap header
            soap_envolope_add_empty_header(envelope_node);
        }
        
        //  <s:Body>
        //    <tds:GetDeviceInformation />
        //  </s:Body>
        envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("tds:GetDeviceInformation");

        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    }

    onvif_soap GetDeviceInformationResponse(const std::string& content){
        pugi::xml_document doc;
        auto result = doc.load_string(content.c_str());
        pugi::xpath_query query("//tds:GetDeviceInformationResponse");
        auto querynode = query.evaluate_node_set(doc);
        if(querynode.empty()){
            return onvif_soap::MustUnderstand;
        }

        // tds:Manufacturer
        proxy_->device_service_proxy_->device_info_->Manufacturer = doc.select_node("//tds:Manufacturer").node().child_value();
        // tds:Model
        proxy_->device_service_proxy_->device_info_->Model = doc.select_node("//tds:Model").node().child_value();
        // tds:FirmwareVersion
        proxy_->device_service_proxy_->device_info_->FirmwareVersion = doc.select_node("//tds:FirmwareVersion").node().child_value();
        // tds:SerialNumber
        proxy_->device_service_proxy_->device_info_->SerialNumber = doc.select_node("//tds:SerialNumber").node().child_value();
        // tds:HardwareId
        proxy_->device_service_proxy_->device_info_->HardwareId = doc.select_node("//tds:HardwareId").node().child_value();

        return onvif_soap::OK;
    }

    // use GetDeviceInformation api with no auth to get auth type
    std::string GetDeviceAuthenticationRequest() const{
        tinyxml2::XMLDocument doc;
        // insert xml declaration
        doc.InsertEndChild(doc.NewDeclaration());
        // envelope node
        tinyxml2::XMLElement* envelope_node = doc.NewElement(onvif::kSOAP_ENVELOPE);
        doc.InsertEndChild(envelope_node);
        // set envelope attribute
        onvif::tds_soap_envelope_attr(envelope_node);
        // <s:Header> with no auth
        soap_envolope_add_empty_header(envelope_node);
        //  <s:Body>
        //    <tds:GetDeviceInformation />
        //  </s:Body>
        envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("tds:GetDeviceInformation");

        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    }

    onvif_soap GetDeviceAuthenticationResponse(const std::string& header,const std::string& content){
        // 解析返回的header
        throw std::runtime_error("unimplementation");
    }

    std::string GetServicesRequest(bool IncludeCapability) const{
        tinyxml2::XMLDocument doc;
        // insert xml declaration
        doc.InsertEndChild(doc.NewDeclaration());
        // envelope node
        tinyxml2::XMLElement* envelope_node = doc.NewElement(onvif::kSOAP_ENVELOPE);
        doc.InsertEndChild(envelope_node);
        // set envelope attribute
        onvif::tds_soap_envelope_attr(envelope_node);
        // <s:Header> add Auth Header
        soap_envolope_add_ws_auth_header(envelope_node);

        // <s:Body>
        //     <tds:GetServices>
        //         <tds:IncludeCapability>true</tds:IncludeCapability>
        //     </tds:GetServices>
        // </s:Body>
        tinyxml2::XMLElement* includecapability = envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("tds:GetServices")->InsertNewChildElement("tds:IncludeCapability");
        includecapability->SetText(IncludeCapability);
        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    }

    onvif_soap GetServicesResponse(const std::string& content){
        // TODO zhaoj unimplementation
        throw std::runtime_error("unimplementation");
    }

    std::string GetScopesRequest() const{
        tinyxml2::XMLDocument doc;
        // insert xml declaration
        doc.InsertEndChild(doc.NewDeclaration());
        // envelope node
        tinyxml2::XMLElement* envelope_node = doc.NewElement(onvif::kSOAP_ENVELOPE);
        doc.InsertEndChild(envelope_node);
        // set envelope attribute
        onvif::tds_soap_envelope_attr(envelope_node);
        // <s:Header> add Auth Header
        soap_envolope_add_ws_auth_header(envelope_node);

        // <s:Body>
        //     <tds:GetScopes />
        // </s:Body>
        envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("tds:GetScopes");
        
        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    }

    onvif_soap GetScopesResponse(const std::string& content){
        throw std::runtime_error("not unimplementation");
    }

    std::string GetCapabilitiesRequest() const{
        tinyxml2::XMLDocument doc;
        // insert xml declaration
        doc.InsertEndChild(doc.NewDeclaration());
        // envelope node
        tinyxml2::XMLElement* envelope_node = doc.NewElement(onvif::kSOAP_ENVELOPE);
        doc.InsertEndChild(envelope_node);
        // set envelope attribute
        onvif::tds_soap_envelope_attr(envelope_node);
        // <s:Header> add Auth Header
        soap_envolope_add_ws_auth_header(envelope_node);

        // <s:Body>
        //     <tds:GetCapabilities>
        //         <tds:Category>All</tds:Category>
        //     </tds:GetCapabilities>
        // </s:Body>
        tinyxml2::XMLElement* category_node = envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("tds:GetCapabilities")->InsertNewChildElement("tds:Category");
        category_node->SetText("All");

        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    }

    onvif_soap GetCapabilitiesResponse(const std::string& content){
        throw std::runtime_error("not unimplementation");
    }

    ////////////////////media service///////////////////////////
    const std::string& media_service() const{
        return xaddr_media_service_;
    }
    std::string GetVideoSourceConfigurationsRequest() const{
        tinyxml2::XMLDocument doc;
        // insert xml declaration
        doc.InsertEndChild(doc.NewDeclaration());
        // envelope node
        tinyxml2::XMLElement* envelope_node = doc.NewElement(onvif::kSOAP_ENVELOPE);
        doc.InsertEndChild(envelope_node);

        // set envelope attribute
        onvif::trt_sop_envelope_attr(envelope_node);
        // <s:Header> add Auth Header
        soap_envolope_add_ws_auth_header(envelope_node);

        // <soap:Body>
        //     <trt:GetVideoSourceConfigurations />
        // </soap:Body>
        envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("trt:GetVideoSourceConfigurations");
        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    };

    onvif_soap GetVideoSourceConfigurationsResponse(const std::string& content){
        throw std::runtime_error("not unimplementation");
    }

    std::string GetOSDsRequest(const std::string& vsctoken) const{
        tinyxml2::XMLDocument doc;
        // insert xml declaration
        doc.InsertEndChild(doc.NewDeclaration());
        // envelope node
        tinyxml2::XMLElement* envelope_node = doc.NewElement(onvif::kSOAP_ENVELOPE);
        doc.InsertEndChild(envelope_node);

        // set envelope attribute
        onvif::trt_sop_envelope_attr(envelope_node);
        // <s:Header> add Auth Header
        soap_envolope_add_ws_auth_header(envelope_node);

        // <soap:Body>
        //     <trt:GetOSDs>
        //         <trt:ConfigurationToken>token</trt:ConfigurationToken>
        //     </trt:GetOSDs>
        // </soap:Body>
        tinyxml2::XMLElement* tokennode = envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("trt:GetOSDs")->InsertNewChildElement("trt:ConfigurationToken");
        tokennode->SetText(vsctoken.c_str());

        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    }

    onvif_soap GetOSDsResponse(const std::string& content){
        throw std::runtime_error("not unimplementation");
    }

    std::string GetProfilesRequest() const{
        tinyxml2::XMLDocument doc;
        // insert xml declaration
        doc.InsertEndChild(doc.NewDeclaration());
        // envelope node
        tinyxml2::XMLElement* envelope_node = doc.NewElement(onvif::kSOAP_ENVELOPE);
        doc.InsertEndChild(envelope_node);

        // set envelope attribute
        onvif::trt_sop_envelope_attr(envelope_node);
        // <s:Header> add Auth Header
        soap_envolope_add_ws_auth_header(envelope_node);

        // <soap:Body>
        //     <trt:GetProfiles />
        // </soap:Body>
        envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("trt:GetProfiles");
        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    }

    onvif_soap GetProfilesResponse(const std::string& content){
        throw std::runtime_error("not unimplementation");
    }

    /**
     *  Description:
     *  This operation requests a URI that can be used to initiate a live media stream using RTSP as the control protocol. The returned URI shall remain valid
     *  indefinitely even if the profile is changed. The ValidUntilConnect, ValidUntilReboot and Timeout Parameter shall be set accordingly (ValidUntilConnect=false,
     *  ValidUntilReboot=false, timeout=PT0S).
     *  The correct syntax for the StreamSetup element for these media stream setups defined in 5.1.1 of the streaming specification are as follows:
     *  1. RTP unicast over UDP: StreamType = "RTP_unicast", TransportProtocol = "UDP"
     *  2. RTP over RTSP over HTTP over TCP: StreamType = "RTP_unicast", TransportProtocol = "HTTP"
     *  3. RTP over RTSP over TCP: StreamType = "RTP_unicast", TransportProtocol = "RTSP"
     *  If a multicast stream is requested at least one of VideoEncoderConfiguration, AudioEncoderConfiguration and MetadataConfiguration shall have a valid
     *  multicast setting.
     *  For full compatibility with other ONVIF services a device should not generate Uris longer than 128 octets.
     *  SOAP action:
     *  http://www.onvif.org/ver10/media/wsdl/GetStreamUr
    */
    std::string GetStreamUriRequest(const std::string& profiletoken) const{
        tinyxml2::XMLDocument doc;
        // insert xml declaration
        doc.InsertEndChild(doc.NewDeclaration());
        // envelope node
        tinyxml2::XMLElement* envelope_node = doc.NewElement(onvif::kSOAP_ENVELOPE);
        doc.InsertEndChild(envelope_node);

        // set envelope attribute
        onvif::trt_sop_envelope_attr(envelope_node);
        // <s:Header> add Auth Header
        soap_envolope_add_ws_auth_header(envelope_node);

        // <soap:Body>
        //     <GetStreamUri xmlns="http://www.onvif.org/ver10/media/wsdl">
        //         <StreamSetup>
        //             <Stream>RTP-Unicast</Stream>
        //             <Transport>
        //                  <Protocol>RTSP</Protocol>
        //             </Transport>
        //         </StreamSetup>
        //         <ProfileToken>profilexxx</ProfileToken>
        //     </GetStreamUri>
        // </soap:Body>
        auto getstreamurinode = envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("GetStreamUri");
        getstreamurinode->SetAttribute("xmlns","http://www.onvif.org/ver10/media/wsdl");
        auto streamsetupnode = getstreamurinode->InsertNewChildElement("StreamSetup");
        auto streamtypenode = streamsetupnode->InsertNewChildElement("Stream");
        streamtypenode->SetText("RTP-Unicast");
        auto protocolnode = streamsetupnode->InsertNewChildElement("Transport")->InsertNewChildElement("Protocol");
        // use rtsp over tcp
        protocolnode->SetText("RTSP");

        auto profilenode = getstreamurinode->InsertNewChildElement("ProfileToken");
        profilenode->SetText(profiletoken.c_str());

        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    }

    onvif_soap GetStreamUriResponse(const std::string& content){
        throw std::runtime_error("not unimplementation");
    }

    ////////////////////media service///////////////////////////
private:
    void soap_envolope_add_empty_header(tinyxml2::XMLElement* const envelope) const{
        envelope->InsertNewChildElement(onvif::kSOAP_HEADER);
    }
    void soap_envolope_add_ws_auth_header(tinyxml2::XMLElement* const envelope) const{
        tinyxml2::XMLElement* header = envelope->InsertNewChildElement(onvif::kSOAP_HEADER);
        header->SetAttribute(onvif::kSOAP_ENVELOPE_NAMESPACE,onvif::kSOAP_ENVELOPE_NAMESPACE_URI);

        static constexpr const char kSOAP_WSSE_NAMESPACE_URI[] = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd";
        static constexpr const char kSOAP_WSU_NAMESPACE_URI[] = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd";
        static constexpr const char kSOAP_WSSE_PASSWORD_TYPE_NAMESPACE_URI[] = "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest";
        // wsse:Security
        tinyxml2::XMLElement* wsseSecurity = header->InsertNewChildElement("wsse:Security");
        wsseSecurity->SetAttribute("xmlns:wsse",kSOAP_WSSE_NAMESPACE_URI);
        wsseSecurity->SetAttribute("xmlns:wsu",kSOAP_WSU_NAMESPACE_URI);
        // wsse:UsernameToken
        tinyxml2::XMLElement* wsseUsernameToken = wsseSecurity->InsertNewChildElement("wsse:UsernameToken");
        // wsse:Username
        tinyxml2::XMLElement* wsseUsername = wsseUsernameToken->InsertNewChildElement("wsse:Username");
        wsseUsername->SetText(proxy_->device_params_->usr.c_str());
        auto pwd_digest = onvif::wsse_digest(proxy_->device_params_->pwd,proxy_->device_service_proxy_->device_system_->utc_diff);
        // wsse:Password
        tinyxml2::XMLElement* wssePwd = wsseUsernameToken->InsertNewChildElement("wsse:Password");
        wssePwd->SetAttribute("Type",kSOAP_WSSE_PASSWORD_TYPE_NAMESPACE_URI);
        // password digest
        wssePwd->SetText(pwd_digest->pwd_digest.c_str());
        // wsse:Nonce
        tinyxml2::XMLElement* wsseNonce = wsseUsernameToken->InsertNewChildElement("wsse:Nonce");
        wsseNonce->SetText(pwd_digest->nonce.c_str());
        // wsu:Created
        tinyxml2::XMLElement* wsuCreated = wsseUsernameToken->InsertNewChildElement("wsu:Created");
        wsuCreated->SetText(pwd_digest->created.c_str());
    }

private:
    onvif_proxy* const proxy_;
    std::string xaddr_device_service_;
    std::string xaddr_media_service_;
};

onvif_proxy::onvif_proxy(const std::string& usr,const std::string& pwd,const std::string& ip,int port,bool use_https){
    device_params_ = std::make_unique<onvif_device_params>();
    std::string http_prefix = "http://";
    if(port == 443 || use_https){
        http_prefix = "https://";
    }
    device_params_->xaddr = http_prefix + ip + ":" + std::to_string(port);
    device_params_->usr = usr;
    device_params_->pwd = pwd;
    reset();
}

onvif_proxy::~onvif_proxy(){

}

 void onvif_proxy::reset(){
    tiny_ = std::make_unique<tiny_onvif>(this);
    device_service_proxy_ = std::make_unique<device_service_proxy>(this);
    device_service_proxy_ ->device_info_ = std::make_unique<onvif_device_infomation>();
    // default use ws username token
    device_service_proxy_->device_info_->Authentication = OnvifAuth::WS_UsernameToken;

    device_service_proxy_ ->device_system_ = std::make_unique<onvif_device_system>();
    // TODO zhaoj 是否支持media_service 是否要根据GetServices信息判断呢？
    media_service_proxy_ = std::make_unique<media_service_proxy>(this);
 }

onvif_proxy::device_service_proxy* onvif_proxy::device_service(){
    return device_service_proxy_.get();
}

onvif_proxy::media_service_proxy* onvif_proxy::media_service(){
    return media_service_proxy_ ? media_service_proxy_.get() : nullptr;
}

onvif_proxy::device_service_proxy::device_service_proxy(onvif_proxy* const proxy):proxy_(proxy){

}

std::string onvif_proxy::device_service_proxy::XAddr() const{
    return proxy_->tiny_->device_service();
}

std::string onvif_proxy::device_service_proxy::proxy_GetScopes() const{
    return proxy_->tiny_->GetScopesRequest();
}
onvif_soap onvif_proxy::device_service_proxy::parse_GetScopes(const std::string& content){
    return proxy_->tiny_->GetScopesResponse(content);
}

std::string onvif_proxy::device_service_proxy::proxy_GetServices(bool IncludeCapability) const{
    return proxy_->tiny_->GetServicesRequest(IncludeCapability);
}

onvif_soap onvif_proxy::device_service_proxy::parse_GetServices(const std::string& content){
    return proxy_->tiny_->GetServicesResponse(content);
}

std::string onvif_proxy::device_service_proxy::proxy_GetCapabilities() const{
    return proxy_->tiny_->GetCapabilitiesRequest();
}
onvif_soap onvif_proxy::device_service_proxy::parse_GetCapabilities(const std::string& content){
    return proxy_->tiny_->GetCapabilitiesResponse(content);
}

std::string onvif_proxy::device_service_proxy::proxy_GetSystemDateAndTime() const{
    return proxy_->tiny_->GetSystemDateAndTimeRequest();
}

onvif_soap onvif_proxy::device_service_proxy::parse_GetSystemDateAndTime(const std::string& content){
    return proxy_->tiny_->GetSystemDateAndTimeResponse(content);
}

std::string onvif_proxy::device_service_proxy::proxy_GetDeviceInformation(bool with_wss) const{
    return proxy_->tiny_->GetDeviceInformationRequest(with_wss);
}
onvif_soap onvif_proxy::device_service_proxy::parse_GetDeviceInformation(const std::string& content){
    return proxy_->tiny_->GetDeviceInformationResponse(content);
}

std::string onvif_proxy::device_service_proxy::proxy_GetDeviceAuthentication() const{
    return proxy_->tiny_->GetDeviceAuthenticationRequest();
}

onvif_soap onvif_proxy::device_service_proxy::parse_GetDeviceAuthentication(const std::string& header,const std::string& content){
    return proxy_->tiny_->GetDeviceAuthenticationResponse(header,content);
}

onvif_proxy::media_service_proxy::media_service_proxy(onvif_proxy* const proxy):proxy_(proxy){

}
const std::string onvif_proxy::media_service_proxy::XAddr() const{
    return proxy_->tiny_->media_service();
}

std::string onvif_proxy::media_service_proxy::proxy_GetVideoSourceConfigurations() const{
    return proxy_->tiny_->GetVideoSourceConfigurationsRequest();
}

onvif_soap onvif_proxy::media_service_proxy::parse_GetVideoSourceConfigurations(const std::string& content){
    return proxy_->tiny_->GetVideoSourceConfigurationsResponse(content);
}

std::string onvif_proxy::media_service_proxy::proxy_GetOSDs(const std::string& vsctoken) const{
    return proxy_->tiny_->GetOSDsRequest(vsctoken);
}

onvif_soap onvif_proxy::media_service_proxy::parse_GetOSDs(const std::string& content){
    return proxy_->tiny_->GetOSDsResponse(content);
}

std::string onvif_proxy::media_service_proxy::proxy_GetProfiles() const{
    return proxy_->tiny_->GetProfilesRequest();
}

onvif_soap onvif_proxy::media_service_proxy::parse_GetProfiles(const std::string& content){
    return proxy_->tiny_->GetProfilesResponse(content);
}

std::string onvif_proxy::media_service_proxy::proxy_GetStreamUri(const std::string& profiletoken) const{
    return proxy_->tiny_->GetStreamUriRequest(profiletoken);
}

onvif_soap onvif_proxy::media_service_proxy::parse_GetStreamUri(const std::string& content){
    return proxy_->tiny_->GetStreamUriResponse(content);
}



};//!namespace avm