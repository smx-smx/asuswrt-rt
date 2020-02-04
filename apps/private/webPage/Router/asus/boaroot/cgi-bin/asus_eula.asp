	<div style="background:#232E32;height:50px;border-top-left-radius: 4px;border-top-right-radius: 4px;">
		<div style="font: 16px bolder monospace;padding: 12px 0 0 10px;"><%tcWebApi_get("String_Entry","ASUS_Notice","s")%></div>
	</div>
	<div id="asus_eula_content" style="background:#293438;border-top:1px solid #3B474B;border-bottom: 1px solid #3B474B;overflow-y: auto;padding:15px 12px 0 12px;">
		<div id="asus_eula_desc"><%tcWebApi_get("String_Entry","ASUS_eula_desc0","s")%></div>
		<div>
			<ol style="margin-left: -15px;">
				<li><%tcWebApi_get("String_Entry","ASUS_eula_desc1","s")%>
					<ol class="custom_ol">
						<li><%tcWebApi_get("String_Entry","ASUS_eula_desc1_1","s")%>
							<ol>
								<li id="asus_eula_desc1_1_1"><%tcWebApi_get("String_Entry","ASUS_eula_desc1_1_1","s")%></li>
							</ol>
						</li>
						<li><%tcWebApi_get("String_Entry","ASUS_eula_desc1_2_s1","s")%>&nbsp;<%tcWebApi_get("String_Entry","ASUS_eula_desc1_2_s2","s")%>
							<ol>
								<li><%tcWebApi_get("String_Entry","ASUS_eula_desc1_2_1","s")%></li>
							</ol>
						</li>
						<li id = "alexa_desc"><%tcWebApi_get("String_Entry","ASUS_eula_desc1_3","s")%></li>
						<li id = "ifttt_desc"><%tcWebApi_get("String_Entry","ASUS_eula_desc1_4","s")%></li>
					</ol>
				</li>
				<li><%tcWebApi_get("String_Entry","ASUS_eula_desc2","s")%></li>
			</ol>
		</div>

		<div style="height: 40px">
			<input type="checkbox" id="ASUS_EULA_enable" value="0"><span id="ageHint"><%tcWebApi_get("String_Entry","ASUS_eula_age","s")%></span>
		</div>
	</div>

	<div style="text-align: center;background:#232E32;height:70px;border-bottom-left-radius: 4px;border-bottom-right-radius: 4px;">
		<input id="cancelBtn" class="button_gen" type="button" style="margin-top:15px;" value="<%tcWebApi_get("String_Entry","CTL_Disagree","s")%>">
		<input id="applyBtn" class="button_gen" type="button" value="<%tcWebApi_get("String_Entry","CTL_Agree","s")%>">
	</div>

	<script>
		$j("#asus_eula_content").html($j("#asus_eula_content").html().replace(/\\'/g, "'"))

		var fn_array = ["DDNS"];
		if(ifttt_support >= 0){fn_array.push("IFTTT™");}
		if(alexa_support >= 0){fn_array.push("Alexa™");}
		$j("#asus_eula_desc").html($j("#asus_eula_desc").html().replace('DDNS', fn_array.join("/")));

		if(alexa_support >= 0 && ifttt_support < 0){
			$j("#asus_eula_desc1_1_1").html("<%tcWebApi_get("String_Entry","ASUS_eula_desc1_1_12","s")%>");
			$j("#ifttt_desc").css("display", "none");
		}
		else if(alexa_support < 0 && ifttt_support < 0){
			$j("#asus_eula_desc1_1_1").html("<%tcWebApi_get("String_Entry","ASUS_eula_desc1_1_13","s")%>");
			$j("#alexa_desc").css("display", "none");
			$j("#ifttt_desc").css("display", "none");
		}

		var preferredLang = parent.get_preferred_lang_type();
		var url="";
		switch(preferredLang){
				case '1':
					url = "https://www.asus.com/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '3':
					url = "https://www.asus.com.cn/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '18':
					url = "https://www.asus.com/tw/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '2':
					url = "https://www.asus.com/br/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '4':
					url = "https://www.asus.com/cz/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '5':
					url = "https://www.asus.com/dk/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '6':
					url = "https://www.asus.com/de/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '7':
					url = "https://www.asus.com/es/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '8':
					url = "https://www.asus.com/fi/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '9':
					url = "https://www.asus.com/fr/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '10':
					url = "https://www.asus.com/it/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '11':
					url = "https://www.asus.com/my/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '12':
					url = "https://www.asus.com/no/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '13':
					url = "https://www.asus.com/pl/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '14':
					url = "https://www.asus.com/ru/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '15':
					url = "https://www.asus.com/se/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '16':
					url = "https://www.asus.com/th/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '17':
					url = "https://www.asus.com/tr/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				case '19':
					url = "https://www.asus.com/ua-ua/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

				default:
					url = "https://www.asus.com/Terms_of_Use_Notice_Privacy_Policy/Privacy_Policy";
					break;

        }
		$j("#eula_url").attr("href", url);
	</script>
