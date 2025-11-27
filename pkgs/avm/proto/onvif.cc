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
#include <random>
#include <zlog/log.h>
#include <zpkg/times.h>
#include <zpkg/base64.h>
#include <zpkg/hash/hash.h>
#include <tinyxml2/tinyxml2.h>
#include <pugixml/pugixml.hpp>
#include <algorithm>
#include <cctype>

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
        std::string pwd_digest = zpkg::base64_encode(zpkg::hash::hash_bin(zpkg::hash::SHA1,(std::string(nonce_buffer)+created_str+pwd)));
        
        return std::make_unique<wsse_pwd_digest>(
                wsse_pwd_digest{
                    .nonce = nonce_base64,
                    .pwd_digest = pwd_digest,
                    .created = created_str});
    };
}

const char* soap::kSOAP_HTTP_CONTENT_TYPE = "application/soap+xml; charset=utf-8";
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
    std::string GetDeviceInformationRequest() const{
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
        doc.load_string(content.c_str());
        pugi::xpath_query query("//tds:GetDeviceInformationResponse");
        auto querynode = query.evaluate_node_set(doc);
        if(querynode.empty()){
            return onvif_soap::MustUnderstand;
        }
        onvif_device_infomation info;
        // tds:Manufacturer
        info.Manufacturer = doc.select_node("//tds:Manufacturer").node().child_value();
        // tds:Model
        info.Model = doc.select_node("//tds:Model").node().child_value();
        // tds:FirmwareVersion
        info.FirmwareVersion = doc.select_node("//tds:FirmwareVersion").node().child_value();
        // tds:SerialNumber
        info.SerialNumber = doc.select_node("//tds:SerialNumber").node().child_value();
        // tds:HardwareId
        info.HardwareId = doc.select_node("//tds:HardwareId").node().child_value();

        // emit device event
        proxy_->proxy_event_->on_device_info(info);

        return onvif_soap::OK;
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

    /**
     * @brief Get video source configurations response handler
     * 
     * This function processes the response message for getting video source configurations 
     * in the ONVIF protocol. The current version has not implemented the specific functionality 
     * and will throw a runtime exception when called.
     * 
     * @param content The response message content string
     * @return onvif_soap Returns the processed SOAP object
     * @throws std::runtime_error Throws "not unimplementation" exception when function is called
     */    
    onvif_soap GetVideoSourceConfigurationsResponse(const std::string& content){
        pugi::xml_document doc;
        doc.load_string(content.c_str());
        pugi::xpath_query query("//trt:GetVideoSourceConfigurationsResponse");
        auto querynode = query.evaluate_node_set(doc);
        if(querynode.empty()){
            return onvif_soap::MustUnderstand;
        }
        // may be multi video source configuration
        pugi::xpath_node_set node_sets = doc.select_nodes("//trt:Configurations");
        onvif_video_source_configuration vsc;
        for(pugi::xpath_node node : node_sets){
            vsc.token = node.node().attribute("token").as_string();
            vsc.view_mode = node.node().attribute("ViewMode").as_string();
            vsc.name = node.node().child("tt:Name").text().as_string();
            vsc.source_token = node.node().child("tt:SourceToken").text().as_string();

            proxy_->proxy_event_->on_video_source_configuration(vsc);
        }
        proxy_->proxy_event_->on_video_source_configuration_complete();
        return onvif_soap::OK;
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
        pugi::xml_document doc;
        doc.load_string(content.c_str());
        pugi::xpath_query query("//trt:GetOSDsResponse");
        auto querynode = query.evaluate_node_set(doc);
        if(querynode.empty()){
            return onvif_soap::MustUnderstand;
        }

        // tt:PlainText 
        auto osds = doc.select_nodes("//trt:OSDs");
        for(const auto& osd : osds){
            pugi::xpath_query query("//tt:PlainText");
            auto querynode = query.evaluate_node(osd);
            if(querynode.node().empty()){
                continue;
            }
            
            // found PlainText
            const std::string osd_text = querynode.node().child_value();
            const std::string vsc_token = osd.node().child("tt:VideoSourceConfigurationToken").text().as_string();
            proxy_->proxy_event_->on_osd(vsc_token,osd_text);
            return onvif_soap::OK;
        }
        
        return onvif_soap::DataEncodingUnknown;
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
        pugi::xml_document doc;
        doc.load_string(content.c_str());
        pugi::xpath_query query("//trt:GetProfilesResponse");
        auto querynode = query.evaluate_node_set(doc);
        if(querynode.empty()){
            return onvif_soap::MustUnderstand;
        }

        auto profiles = doc.select_nodes("//trt:Profiles");
        for(const auto& profile : profiles){
            // found video source configure
            pugi::xml_node vsc_node = profile.node().child("tt:VideoSourceConfiguration");
            if(vsc_node.empty()){
                continue;
            }
            std::string vsc_token = vsc_node.attribute("token").as_string();

            // found video source configure get token and name
            std::string profile_token = profile.node().attribute("token").as_string();
            std::string profile_name = profile.node().child("tt:Name").text().as_string();

            proxy_->proxy_event_->on_media_profile(vsc_token,profile_token,profile_name);
        }
        proxy_->proxy_event_->on_media_profile_complete();
        return onvif_soap::OK;
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
        //             <Stream xmlns="http://www.onvif.org/ver10/schema">RTP-Unicast</Stream>
        //             <Transport xmlns="http://www.onvif.org/ver10/schema">
        //                  <Protocol>RTSP</Protocol>
        //             </Transport>
        //         </StreamSetup>
        //         <ProfileToken>profilexxx</ProfileToken>
        //     </GetStreamUri>
        // </soap:Body>
        auto getstreamurinode = envelope_node->InsertNewChildElement(onvif::kSOAP_BODY)->InsertNewChildElement("GetStreamUri");
        getstreamurinode->SetAttribute("xmlns","http://www.onvif.org/ver10/media/wsdl");
        {
            auto streamsetupnode = getstreamurinode->InsertNewChildElement("StreamSetup");
            {
                auto streamtypenode = streamsetupnode->InsertNewChildElement("Stream");
                streamtypenode->SetText("RTP-Unicast");
                streamtypenode->SetAttribute("xmlns","http://www.onvif.org/ver10/schema");
            }
            {
                auto transportnode = streamsetupnode->InsertNewChildElement("Transport");
                transportnode->SetAttribute("xmlns","http://www.onvif.org/ver10/schema");
                {
                    auto protocolnode = transportnode->InsertNewChildElement("Protocol");
                    // use rtsp over tcp
                    protocolnode->SetText("RTSP");
                }
            }
            auto profilenode = getstreamurinode->InsertNewChildElement("ProfileToken");
            profilenode->SetText(profiletoken.c_str());
        }
        
        tinyxml2::XMLPrinter xml_printer;
        doc.Print(&xml_printer);

        return xml_printer.CStr();
    }

    onvif_soap GetStreamUriResponse(const std::string& profiletoken,const std::string& content){
        pugi::xml_document doc;
        doc.load_string(content.c_str());
        pugi::xpath_query query("//trt:GetStreamUriResponse");
        auto querynode = query.evaluate_node_set(doc);
        if(querynode.empty()){
            return onvif_soap::MustUnderstand;
        }
        auto media_uri = doc.select_node("//trt:MediaUri");
        if(media_uri.node().empty()){
            return onvif_soap::MustUnderstand;
        }
        std::string stream_uri = media_uri.node().child("tt:Uri").text().as_string();
        proxy_->proxy_event_->on_meida_profile_streamuri(profiletoken,stream_uri);

        return onvif_soap::OK;
    }

    ////////////////////media service///////////////////////////
private:
    /**
     * @brief Add an empty SOAP header to the envelope
     * 
     * This function inserts a new empty header element into the provided SOAP envelope element.
     * The header element uses the standard SOAP header tag name defined by `onvif::kSOAP_HEADER`.
     * 
     * @param envelope A pointer to the SOAP envelope XML element where the header will be added
     */
    void soap_envolope_add_empty_header(tinyxml2::XMLElement* const envelope) const{
        envelope->InsertNewChildElement(onvif::kSOAP_HEADER);
    }
    /**
     * @brief Add WS-Security authentication header to SOAP envelope
     * 
     * This function constructs a WS-Security authentication header using UsernameToken 
     * method to add user authentication information. The authentication information 
     * includes username, password digest, nonce, and creation timestamp.
     * 
     * @param envelope Pointer to the SOAP envelope XML element where the authentication 
     *                 header will be inserted
     */
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

onvif_proxy::onvif_proxy(onvif_proxy_event* event,const std::string& usr,const std::string& pwd,const std::string& ip,int port,bool use_https)
    :proxy_event_(event){
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

std::string onvif_proxy::device_service_proxy::proxy_GetDeviceInformation() const{
    return proxy_->tiny_->GetDeviceInformationRequest();
}
onvif_soap onvif_proxy::device_service_proxy::parse_GetDeviceInformation(const std::string& content){
    return proxy_->tiny_->GetDeviceInformationResponse(content);
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

onvif_soap onvif_proxy::media_service_proxy::parse_GetStreamUri(const std::string& profiletoken,const std::string& content){
    return proxy_->tiny_->GetStreamUriResponse(profiletoken,content);
}



};//!namespace avm