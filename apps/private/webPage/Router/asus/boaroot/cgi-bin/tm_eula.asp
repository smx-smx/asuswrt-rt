
<div style="background:#232E32;height:40px;border-top-left-radius: 4px;border-top-right-radius: 4px;">
	<div style="font: 16px bolder Microsoft JhengHei;padding: 12px 0 0 10px;"><%tcWebApi_get("String_Entry","lyra_TrendMicro_agreement","s")%></div>
</div>

<div id="tm_eula_content" style="background:#293438;height:340px;border-top:1px solid #3B474B;border-bottom: 1px solid #3B474B;overflow-y: auto;font-size:14px;padding:15px 12px 0 12px;">
	<div>By using AiProtection, Traffic analyzer, Apps analyzer and Adaptive QoS, you agree to the <a id="eula_url" style="text-decoration:underline;" target="_blank">Trend Micro End User License Agreement.</a></div>
	<div style="margin: 18px 0;">Disclaimer : AiProtection, Traffic analyzer, Apps analyzer and Adaptive QoS contains services provided by Trend Micro, please be aware that ASUS shall bear no responsibility for any and all information security, the privacy practices and the services provided or implemented by Trend Micro. ASUS encourage you to read and be aware of the privacy statements or terms and conditions of Trend Micro before your use of these service.</div>
</div>

<div style="background:#232E32;height:60px;border-bottom-left-radius: 4px;border-bottom-right-radius: 4px;">
	<input class="button_gen" type="button" style="margin-left:27%;margin-top:15px;" onclick="cancel();" value="<%tcWebApi_get("String_Entry","CTL_Disagree","s")%>">
	<input class="button_gen" type="button"  onclick="eula_confirm();" value="<%tcWebApi_get("String_Entry","CTL_Agree","s")%>">
</div>
