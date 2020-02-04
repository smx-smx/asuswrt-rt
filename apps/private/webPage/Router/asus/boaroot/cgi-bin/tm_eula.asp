
<div style="background:#232E32;height:40px;border-top-left-radius: 4px;border-top-right-radius: 4px;">
	<div style="font: 16px bolder monospace;padding: 12px 0 0 10px;"><%tcWebApi_get("String_Entry","lyra_TrendMicro_agreement","s")%></div>
</div>

<div id="tm_eula_content" style="background:#293438;border-top:1px solid #3B474B;border-bottom: 1px solid #3B474B;overflow-y: auto;font-size:14px;padding:15px 12px 0 12px;">
	<div><%tcWebApi_get("String_Entry","TM_eula_desc1","s")%></div>
	<div style="margin: 18px 0;"><%tcWebApi_get("String_Entry","TM_eula_desc2","s")%></div>
	<div style="margin-top: -10px;"><%tcWebApi_get("String_Entry","TM_privacy_policy","s")%></div>
	<div style="margin-top: 5px;"><%tcWebApi_get("String_Entry","TM_data_collection","s")%></div>
	<div style="margin: 18px 0;"><%tcWebApi_get("String_Entry","TM_eula_desc3","s")%></div>
</div>

<div style="text-align: center;background:#232E32;height:60px;border-bottom-left-radius: 4px;border-bottom-right-radius: 4px;">
	<input id="cancelBtn" class="button_gen" type="button" style="margin-top:15px;" value="<%tcWebApi_get("String_Entry","CTL_Disagree","s")%>">	<!-- onclick="cancel();"-->
	<input id="applyBtn" class="button_gen" type="button" value="<%tcWebApi_get("String_Entry","CTL_Agree","s")%>">	<!-- onclick="eula_confirm();"-->
</div>

<script>
	var href_lang = get_supportsite_lang();
	var $j = jQuery.noConflict();
	$j("#tm_eula_content").html($j("#tm_eula_content").html().replace(/\\'/g, "'"))
	if(href_lang != "/" && href_lang != "/my/" && href_lang != "/fi/" && href_lang != "/ua/"){
		if(href_lang == ".cn/"){
			$j("#eula_url").attr("href", "https://www.asus.com/Microsite/networks/Trend_Micro_EULA/cn");
		}
		else if(href_lang == "/dk/"){
			$j("#eula_url").attr("href", "https://www.asus.com/Microsite/networks/Trend_Micro_EULA/da");
		}
		else if(href_lang == "/se/"){
			$j("#eula_url").attr("href", "https://www.asus.com/Microsite/networks/Trend_Micro_EULA/sv");
		}
		else if(href_lang == "/br/"){
			$j("#eula_url").attr("href", "https://www.asus.com/Microsite/networks/Trend_Micro_EULA/pt");
		}
		else{
			$j("#eula_url").attr("href", "https://www.asus.com/Microsite/networks/Trend_Micro_EULA"+href_lang);
		}
	}
	else{
		$j("#eula_url").attr("href", "https://www.asus.com/Microsite/networks/Trend_Micro_EULA/");
	}	
	$j("#tm_eula_url").attr("href", "https://www.trendmicro.com/en_us/about/legal/privacy-policy-product.html");
	$j("#tm_disclosure_url").attr("href", "https://success.trendmicro.com/data-collection-disclosure");
</script>