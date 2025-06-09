-- @brief rfc4571 Protocol dissector plugin for gb28181
-- @author zhaoj

-- create a new dissector
local NAME = "rfc4571"
local PORT = 9020
local rfc4571 = Proto(NAME, "rtp4571")

-- create fields of rfc4571
local field_length = ProtoField.uint16 ("len", "length")
rfc4571.fields = { field_length }

-- dissect packet
function dissectFullRFC4571 (tvb, pinfo, tree)
	-- rfc4571 packet frame
    --  0                   1                   2                   3
    --  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    --  ---------------------------------------------------------------
    -- |             LENGTH            |  RTP or RTCP packet ...       |
    --  ---------------------------------------------------------------
    
    if tvb:len() < 6 then return 0 end

    local ver = tvb:range(2,1)
    ver = ver:bitfield(0,2)
    if ver ~= 2 then return 0 end

    local subtree = tree:add(rfc4571, tvb())
    local offset = 0
        
    subtree:add(field_length, tvb:range(offset,2):uint())
    offset = offset + 2
    
    local rtcp_type = tvb:range(offset+1, 2):le_uint()
    if rtcp_type <= 200  or rtcp_type>=204 then
        Dissector.get("rtp"):call(tvb:range(offset):tvb(), pinfo, tree)
    else
        Dissector.get("rtcp"):call(tvb:range(offset):tvb(), pinfo, tree)
    end

    return tvb:len()
end

function getRFC4571Len (tvb, pinfo, offset)
    return tvb:range(offset, 2):uint()+2
end

function rfc4571.dissector (tvb, pinfo, tree)
    return dissect_tcp_pdus(tvb, tree, 6, getRFC4571Len, dissectFullRFC4571, true)
end

-- register this dissector
DissectorTable.get("tcp.port"):add(PORT, rfc4571)