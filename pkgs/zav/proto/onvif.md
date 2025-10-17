# 接口示例与说明

# onvif http1.1请求的header
User-Agent: 自定义
Connection: Close
Content-Type: application/soap+xml; charset=utf-8;
Host: ip地址
Content-Length: 205

# GetSystemDateAndTime 这个接口不用认证
<?xml version="1.0" encoding="utf-8"?>
<s:Envelope xmlns:s="http://www.w3.org/2003/05/soap-envelope" xmlns:tds="http://www.onvif.org/ver10/device/wsdl" xmlns:tt="http://www.onvif.org/ver10/schema">
  <s:Body>
    <tds:GetSystemDateAndTime />
  </s:Body>
</s:Envelope>

需要通过GetSystemTimeAndDate定期心跳来获取时间差 在需要认证的接口上需要时间差来计算正确的nonce

# webservice 认证标准
Password_Digest = Base64 ( SHA-1 ( nonce + created + password ) )  

# response 大华response
HTTP/1.1 200 OK
Content-Type: application/soap+xml; charset=utf-8
Connection: close
Content-Length: 1007

<?xml version="1.0" encoding="utf-8" standalone="yes"?>
<s:Envelope xmlns:sc="http://www.w3.org/2003/05/soap-encoding" xmlns:s="http://www.w3.org/2003/05/soap-envelope" xmlns:tt="http://www.onvif.org/ver10/schema" xmlns:tds="http://www.onvif.org/ver10/device/wsdl">
  <s:Header />
  <s:Body>
    <tds:GetSystemDateAndTimeResponse>
      <tds:SystemDateAndTime>
        <tt:DateTimeType>NTP</tt:DateTimeType>
        <tt:DaylightSavings>false</tt:DaylightSavings>
        <tt:TimeZone>
          <tt:TZ>GMT+08:00PDT,M1.1.1/00:00:00,M1.1.2/00:00:00</tt:TZ>
        </tt:TimeZone>
        <tt:UTCDateTime>
          <tt:Time>
            <tt:Hour>6</tt:Hour>
            <tt:Minute>38</tt:Minute>
            <tt:Second>42</tt:Second>
          </tt:Time>
          <tt:Date>
            <tt:Year>2025</tt:Year>
            <tt:Month>10</tt:Month>
            <tt:Day>9</tt:Day>
          </tt:Date>
        </tt:UTCDateTime>
        <tt:LocalDateTime>
          <tt:Time>
            <tt:Hour>14</tt:Hour>
            <tt:Minute>38</tt:Minute>
            <tt:Second>42</tt:Second>
          </tt:Time>
          <tt:Date>
            <tt:Year>2025</tt:Year>
            <tt:Month>10</tt:Month>
            <tt:Day>9</tt:Day>
          </tt:Date>
        </tt:LocalDateTime>
      </tds:SystemDateAndTime>
    </tds:GetSystemDateAndTimeResponse>
  </s:Body>
</s:Envelope>

# response 宇视response
HTTP/1.1 200 OK
Server: gSOAP/2.7
Content-Type: application/soap+xml; charset=utf-8
Content-Length: 3401
Connection: close

<?xml version="1.0" encoding="UTF-8"?>
<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://www.w3.org/2003/05/soap-envelope" xmlns:SOAP-ENC="http://www.w3.org/2003/05/soap-encoding" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:tns="http://tempuri.org/tns.xsd" xmlns:tns1="http://www.onvif.org/ver10/topics" xmlns:c14n="http://www.w3.org/2001/10/xml-exc-c14n#" xmlns:wsu="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd" xmlns:ds="http://www.w3.org/2000/09/xmldsig#" xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd" xmlns:wsa="http://schemas.xmlsoap.org/ws/2004/08/addressing" xmlns:wsa5="http://www.w3.org/2005/08/addressing" xmlns:xmime5="http://tempuri.org/xmime5.xsd" xmlns:xop="http://www.w3.org/2004/08/xop/include" xmlns:tplt="http://www.onvif.org/ver10/plus/schema" xmlns:tt="http://www.onvif.org/ver10/schema" xmlns:wstop="http://docs.oasis-open.org/wsn/t-1" xmlns:wsrfr="http://docs.oasis-open.org/wsrf/r-2" xmlns:wsrfbf="http://docs.oasis-open.org/wsrf/bf-2" xmlns:tanae="http://www.onvif.org/ver20/analytics/wsdl/AnalyticsEngineBinding" xmlns:tanre="http://www.onvif.org/ver20/analytics/wsdl/RuleEngineBinding" xmlns:tan="http://www.onvif.org/ver20/analytics/wsdl" xmlns:tds="http://www.onvif.org/ver10/device/wsdl" xmlns:tes-cppb="http://www.onvif.org/ver10/events/wsdl/CreatePullPointBinding" xmlns:tes-e="http://www.onvif.org/ver10/events/wsdl/EventBinding" xmlns:tes-nc="http://www.onvif.org/ver10/events/wsdl/NotificationConsumerBinding" xmlns:tes-np="http://www.onvif.org/ver10/events/wsdl/NotificationProducerBinding" xmlns:tes-ppb="http://www.onvif.org/ver10/events/wsdl/PullPointBinding" xmlns:tes-pps="http://www.onvif.org/ver10/events/wsdl/PullPointSubscriptionBinding" xmlns:tev="http://www.onvif.org/ver10/events/wsdl" xmlns:tes-psmb="http://www.onvif.org/ver10/events/wsdl/PausableSubscriptionManagerBinding" xmlns:wsnt="http://docs.oasis-open.org/wsn/b-2" xmlns:tes-sm="http://www.onvif.org/ver10/events/wsdl/SubscriptionManagerBinding" xmlns:timg="http://www.onvif.org/ver20/imaging/wsdl" xmlns:tmd="http://www.onvif.org/ver10/deviceIO/wsdl" xmlns:tpl="http://www.onvif.org/ver10/plus/wsdl" xmlns:tptz="http://www.onvif.org/ver20/ptz/wsdl" xmlns:tr2="http://www.onvif.org/ver20/media/wsdl" xmlns:trc="http://www.onvif.org/ver10/recording/wsdl" xmlns:trp="http://www.onvif.org/ver10/replay/wsdl" xmlns:trt="http://www.onvif.org/ver10/media/wsdl" xmlns:trv="http://www.onvif.org/ver10/receiver/wsdl" xmlns:tse="http://www.onvif.org/ver10/search/wsdl" xmlns:ter="http://www.onvif.org/ver10/error">
  <SOAP-ENV:Header>
  </SOAP-ENV:Header>
  <SOAP-ENV:Body>
    <tds:GetSystemDateAndTimeResponse>
      <tds:SystemDateAndTime>
        <tt:DateTimeType>Manual</tt:DateTimeType>
        <tt:DaylightSavings>false</tt:DaylightSavings>
        <tt:TimeZone>
          <tt:TZ>CST-8</tt:TZ>
        </tt:TimeZone>
        <tt:UTCDateTime>
          <tt:Time>
            <tt:Hour>6</tt:Hour>
            <tt:Minute>42</tt:Minute>
            <tt:Second>4</tt:Second>
          </tt:Time>
          <tt:Date>
            <tt:Year>2025</tt:Year>
            <tt:Month>10</tt:Month>
            <tt:Day>9</tt:Day>
          </tt:Date>
        </tt:UTCDateTime>
        <tt:LocalDateTime>
          <tt:Time>
            <tt:Hour>14</tt:Hour>
            <tt:Minute>42</tt:Minute>
            <tt:Second>4</tt:Second>
          </tt:Time>
          <tt:Date>
            <tt:Year>2025</tt:Year>
            <tt:Month>10</tt:Month>
            <tt:Day>9</tt:Day>
          </tt:Date>
        </tt:LocalDateTime>
      </tds:SystemDateAndTime>
    </tds:GetSystemDateAndTimeResponse>
  </SOAP-ENV:Body>
</SOAP-ENV:Envelope>