var $j = jQuery.noConflict();

var cachedData = {
	"get": {},
	"clear": function(dataArray){$j.each(dataArray, function(idx, val){delete cachedData.get[val];})}
}

var asyncData = {
	"get": {},
	"clear": function(dataArray){$j.each(dataArray, function(idx, val){delete asyncData.get[val];})}
}

var httpApi ={
	"nvramGetAsync": function(q){
		if(!q.success || !q.data) return false;

		var __nvramget = function(_nvrams){
			return _nvrams.map(function(elem){return "nvram_get(" + elem + "," + elem + ")";}).join("%3B");
		};

		$j.ajax({
			url: '/appGet.cgi?hook=' + __nvramget(q.data),
			dataType: 'json',
			error: q.error,
			success: function(encNvram){
				var decNvram = {};
				for (var name in encNvram){decNvram[name] = decodeURIComponent(encNvram[name]);}
				q.success(decNvram);
			}
		});
	},

	"hookGetAsync": function(q){
		if(!q.success || !q.data) return false;

		var queryString = q.data.split("-")[0] + "(" + (q.data.split("-")[1] || "") + ")";

		$j.ajax({
			url: '/appGet.cgi?hook=' + queryString,
			dataType: 'json',
			error: q.error,
			success: function(res){
				q.success(res[q.data]);
			}
		});
	},

	"nvramGet": function(objItems, forceUpdate){
		var queryArray = [];
		var retData = {};

		var __nvramget = function(_nvrams){
			return _nvrams.map(function(elem){return "nvram_get(" + elem + ")";}).join("%3B");
		};

		if(forceUpdate) cachedData.clear(objItems);

		objItems.forEach(function(key){
			if(cachedData.get.hasOwnProperty(key)){
				retData[key] = cachedData.get[key];
			}
			else if(asyncData.get.hasOwnProperty(key)){
				retData[key] = cachedData.get[key] = asyncData.get[key];
				if(forceUpdate) delete asyncData.get[key];
			}
			else{
				queryArray.push(key);
			}
		});

		if(queryArray.length != 0){
			$j.ajax({
				url: '/appGet.cgi?hook=' + __nvramget(queryArray),
				dataType: 'json',
				async: false,
				error: function(){
					for(var i=0; i<queryArray.length; i++){retData[queryArray[i]] = "";}
					retData.isError = true;

					$j.ajax({
						url: '/appGet.cgi?hook=' + __nvramget(queryArray),
						dataType: 'json',
						error: function(){
							for(var i=0; i<queryArray.length; i++){asyncData.get[queryArray[i]] = "";}
						},
						success: function(response){
							Object.keys(response).forEach(function(key){asyncData.get[key] = response[key];})
						}
					});
				},
				success: function(response){
					Object.keys(response).forEach(function(key){retData[key] = cachedData.get[key] = response[key];})
					retData.isError = false;
				}
			});
		}
		else{
			retData.isError = false;		
		}
		
		return retData;
	},

	"nvramSet": function(postData, handler){
		delete postData.isError;

		$j.ajax({
			url: '/applyapp.cgi',
			dataType: 'json',
			data: postData,
			error: function(){},
			success: function(response){
				if(handler) handler.call(response);
			}
		})
	},

	"hookGet": function(hookName, forceUpdate){
		var queryString = hookName.split("-")[0] + "(" + (hookName.split("-")[1] || "") + ")";
		var retData = {};

		if(cachedData.get.hasOwnProperty(hookName) && !forceUpdate){
			retData[hookName] = cachedData.get[hookName];
		}
		else if(asyncData.get.hasOwnProperty(hookName)){
			retData[hookName] = asyncData.get[hookName];
			if(forceUpdate) delete asyncData.get[hookName];
		}
		else{
			$j.ajax({
				url: '/appGet.cgi?hook=' + queryString,
				dataType: 'json',
				async: false,
				error: function(){
					retData[hookName] = "";
					retData.isError = true;
			
					$j.ajax({
						url: '/appGet.cgi?hook=' + queryString,
						dataType: 'json',
						error: function(){
							asyncData.get[hookName] = "";
						},
						success: function(response){
							asyncData.get[hookName] = response[hookName];
						}
					});
				},
				success: function(response){
					retData = response;
					cachedData.get[hookName] = response[hookName]
					retData.isError = false;
				}
			});
		}

		return retData[hookName];
	},

	"enableEula": function(_eulaType, enable, callback){
		var eulaType = _eulaType.toUpperCase()

		$j.ajax({
			url: '/set_' + eulaType + '_EULA.cgi?' + eulaType + '_EULA=' + enable,
			error: function(){},
			success: callback
		});
	},

	"unregisterAsusDDNS": function(callback){
		$j.ajax({
			url: '/unreg_ASUSDDNS.cgi',
			error: function(){},
			success: callback
		});
	}
}
