/*
Old naming rule:
1. "-" to ""
2. "+" to "Plus"
*/
function supportsite_model(country_code, model_name){
var real_model_name = "";	
if(model_name == "DSL-N12U-C1")
	real_model_name = "DSL-N12U_C1";
else if(model_name == "DSL-N12E-C1" && (country_code == "SG" || country_code == "FR"))
	real_model_name = "DSLN12E_C1_with_5dBi_antenna";
else if(model_name == "DSL-N12E-C1")
	real_model_name = "DSLN12E_C1";
else if(model_name == "DSL-N10-C1" && country_code == "SG")
	real_model_name = "DSLN10_C1_with_5dBi_antenna";
else if(model_name == "DSL-N10-C1")
	real_model_name = "DSLN10_C1";
else if(model_name == "DSL-N10P-C1")
	real_model_name = "DSL-N10P_C1";
else if(model_name == "DSL-N55U-C1")
	real_model_name = "DSL-N55U_C1";
else if(model_name == "DSL-N55U-D1")
	real_model_name = "DSL-N55U_D1";
else	
	real_model_name = model_name;
	
real_model_name = real_model_name.replace("-", "");
real_model_name = real_model_name.replace("+", "Plus");
return real_model_name;

}	
	
