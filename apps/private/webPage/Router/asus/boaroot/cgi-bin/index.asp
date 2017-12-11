<html xmlns="http://www.w3.org/1999/xhtml">
<html xmlns:v>
<script>
	var noRedirectQIS = "<% tcWebApi_Get("SysInfo_Entry","noRedirectQIS","s") %>";
	if("<% tcWebApi_staticGet("SysInfo_Entry","x_Setting","s") %>" == "0" && noRedirectQIS.toUpperCase() != "YES")
		document.location.href = "/QIS_wizard.asp";
	else
		document.location.href = "/index2.asp";
</script>
</html>
