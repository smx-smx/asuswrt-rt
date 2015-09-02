uptimeStr = "<% uptime(); %>";
boottime = parseInt(uptimeStr.substring(32,42));
sync_status_update = "<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>";
adsl_timestamp_update = parseInt("<% tcWebApi_get("WebCurSet_Entry","dsl_uptime","s") %>");
log_lineState="<%tcWebApi_staticGet("Info_Adsl","lineState","s")%>";
log_Opmode="<%tcWebApi_staticGet("Info_Adsl","Opmode","s")%>";
log_AdslType="<%tcWebApi_staticGet("Info_Adsl","AdslType","s")%>";
log_AdslType=log_AdslType.replace("_", " ");
log_SNRMarginDown="<%tcWebApi_staticGet("Info_Adsl","SNRMarginDown","s")%>";
log_SNRMarginUp="<%tcWebApi_staticGet("Info_Adsl","SNRMarginUp","s")%>";
log_AttenDown="<%tcWebApi_staticGet("Info_Adsl","AttenDown","s")%>";
log_AttenUp="<%tcWebApi_staticGet("Info_Adsl","AttenUp","s")%>";
log_WanListMode="<%tcWebApi_staticGet("Info_Adsl","WanListMode","s")%>";
log_DataRateDown="<%tcWebApi_staticGet("Info_Adsl","DataRateDown","s")%>";
log_DataRateUp="<%tcWebApi_staticGet("Info_Adsl","DataRateUp","s")%>";
log_AttainDown="<%tcWebApi_staticGet("Info_Adsl","AttainDown","s")%>";
log_AttainUp="<%tcWebApi_staticGet("Info_Adsl","AttainUp","s")%>";
log_PowerDown="<%tcWebApi_staticGet("Info_Adsl","PowerDown","s")%>";
log_PowerUp="<%tcWebApi_staticGet("Info_Adsl","PowerUp","s")%>";
log_CRCDown="<%tcWebApi_staticGet("Info_Adsl","CRCDown","s")%>";
log_CRCUp="<%tcWebApi_staticGet("Info_Adsl","CRCUp","s")%>";


