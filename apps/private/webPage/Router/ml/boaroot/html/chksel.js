function TcCheckSel(selitem, value){
	if(value=="N/A"){
		selitem.selectedIndex=0;
	}
	for(k=0;k<selitem.length;k++){
		if(selitem.options[k].value==value){
			selitem.selectedIndex=k;
		break;
		}
	}
}
function valDoValidateIntegerGain(Integer)
{   
   if(Integer.match("^[-0-9]+")) 
   { 
   	return true;
   }
   alert(_("JSVPIInValid"));
   return false;
}
