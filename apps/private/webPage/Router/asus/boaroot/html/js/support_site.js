/*
New naming rule:
1. " " "/" "-" "_" "&" "!" to "-"
2. "+" to "-plus"
*/

function supportsite_model(country_code, model_name){
var real_model_name = "";

if(model_name == "DSL-N10-D1" && country_code == "SG")
	real_model_name = "DSL-N10-D1-with-5dBi-antenna";

real_model_name = model_name.replace(" ", "-");
real_model_name = real_model_name.replace("/", "-");
real_model_name = real_model_name.replace("_", "-");
real_model_name = real_model_name.replace("&", "-");
real_model_name = real_model_name.replace("!", "-");
real_model_name = real_model_name.replace("+", "-plus");	

return real_model_name;
}	
	
