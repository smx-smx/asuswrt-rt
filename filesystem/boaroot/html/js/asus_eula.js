var $j = jQuery.noConflict();
$j('head').append('<link rel="stylesheet" href="/css/asus_eula.css" type="text/css" />');
$j('head').append('<script type="text/javascript" src="/js/httpApi.js">');

var ASUS_EULA = {
	"agree_eula_callback": function(){},

	"disagree_eula_callback": function(){},

	"config": function(set_callback, disagree_callback){
		ASUS_EULA.agree_eula_callback = set_callback;
		ASUS_EULA.disagree_eula_callback = disagree_callback;
		return ASUS_EULA;
	},

	"agree": function(eula_type){
		if($j("#ASUS_EULA_enable").is(":visible")){
			if($j("#ASUS_EULA_enable").prop("checked") === false){
				alert("<%tcWebApi_get("String_Entry","ASUS_eula_age_confirm","s")%>");
				$j("#ageHint").css({"color": "#FC0"});
				return false;
			}
		}

		httpApi.enableEula(eula_type, "1", function(){
			ASUS_EULA.close(eula_type);
			ASUS_EULA.agree_eula_callback(eula_type);
		})
	},

	"disagree": function(eula_type){
		switch(eula_type){
			case "asus":
				httpApi.unregisterAsusDDNS(function(){
					httpApi.enableEula("asus", "0", function(){
						ASUS_EULA.close("asus");
						ASUS_EULA.disagree_eula_callback(eula_type);
					})
				})
				break;

			case "tm":
				httpApi.enableEula("tm", "0", function(){
					ASUS_EULA.close("tm");
					ASUS_EULA.disagree_eula_callback(eula_type);
				})
				break;
		}
	},

	"check": function(eula_type){
		var eulaName = eula_type.toUpperCase() + "_EULA";
		var eulaTime = eula_type.toUpperCase() + "_EULA_time";
		var asusEula = httpApi.nvramGet([eulaName , eulaTime], true);

		if(asusEula[eulaName] != "1" || asusEula[eulaTime] == ""){
			ASUS_EULA.show(eula_type);
			return false;
		}
		else{
			return true;
		}
	},

	"close": function(eula_type){
		$j("#Loading").css({"visibility": "hidden"})
		$j("#loadingBlock").css({"visibility": ""})

		$j('#alert_' + eula_type + '_EULA').fadeOut(300, function(){
			$j(this).remove();
		})
	},

	"show": function(eula_type){
		$j("#loadingBlock").css({"visibility": "hidden"})
		$j("#Loading").css({"visibility": "visible"})

		$j("<div>")
			.attr({
				"id": "alert_" + eula_type + "_EULA",
				"class": "eula_panel_container"
			})
			.css({
				"margin-top": "50px",
				"margin-left": "210px",
				"width": "600px",
				"line-height": "18px",
				"text-align": "left",
				"font-size": "14px",
				"border-radius": "6px",
				"font-family": "monospace"
			})
			.load(eula_type + "_eula.asp", function(data){
				$j("#cancelBtn").click(function(){
					ASUS_EULA.disagree(eula_type);
				})

				$j("#applyBtn").click(function(){
					ASUS_EULA.agree(eula_type);
				})
			})
			.prependTo($j(".banner1"))
			.fadeIn(300)
	}
}
