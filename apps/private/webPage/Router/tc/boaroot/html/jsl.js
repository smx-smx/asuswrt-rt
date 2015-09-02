
function jslDoToggleCheckBox(id)
{
  var checkbox,i;
  var num = jslDoToggleCheckBox.arguments.length;

  if(num==0) return;
  if(num==1)
  {
    // Toggle only one checkbox
    checkbox = document.getElementById(id);
	if(checkbox == null) return;

    if(checkbox.value=="on")
    {
      checkbox.value="off";
     checkbox.checked=false;
    }
    else
    {
      checkbox.value="on";
     checkbox.checked=true;
    }
  }
  else
  {
    checkbox = document.getElementById(id);
	if(checkbox == null) return;

    // Set the first one to "checked" and the rest to "unchecked"
    checkbox.value="on";
    checkbox.checked=true;

    for(i=1;i < num; i++)
	{
      checkbox = document.getElementById(jslDoToggleCheckBox.arguments[i]);
      if(checkbox == null) return;

      checkbox.value="off";
      checkbox.checked=false;

	}
  }
  return false;
}

function jslDoToggleViews(id)
{
  var view,i;
  var num = jslDoToggleViews.arguments.length;

  if(num==0) return;
  if(num==1)
  {
    // Toggle only one view
    view = document.getElementById(id);
	if(view == null) return;

    if(view.style.display=="block")
      view.style.display="none";
	else
      view.style.display="";
  }
  else
  {
    for(i=1;i < num; i++)
	{
      view = document.getElementById(jslDoToggleViews.arguments[i]);
      if(view == null) return;

      view.style.display="none";
	}

    view = document.getElementById(id);
	if(view == null) return;

    // Set the first view to "block"
    view.style.display="";
  }
  return false;
}
function doLoadPage(id,page)
{
  var handle = document.getElementById(id);
  if(handle!=null) handle.src=page;
}
function jslDoClearHighlight(id)
{
  var element;
  element = document.getElementById(id);
  if(element != null)
  {
	element.style.background = "white";
  }
  element.blur();  
  return false;
}

function jslDoSetHighlight(id,color)
{
  var element;
  element = document.getElementById(id);
  if(element != null)
  {
	element.style.background = color;
  }
  element.select();
  element.focus();
  return false;
}

function jslIsString(id)
{
  if(document.getElementById(id).value.match("[a-zA-Z0-9_]+")!=null) return true;
  return false;
}

function jslSetValue(variable,value)
{
   document.getElementById(variable).value=document.getElementById(value).value;
}

function jslDisable(id)
{
  document.getElementById(id).disabled=true;
}

function jslEnable(id)
{
  document.getElementById(id).disabled=false;
}

function jslDoToggleRadio(id,index,maxId)
{
  var radiobox,i;
  var ids;

  if(maxId==1)
  {
    // Toggle only one radio
    radiobox = document.getElementById(id+index);
	if(radiobox == null) return;

    if(radiobox.value=="on")
    {
      radiobox.value="off";
      radiobox.checked=false;
    }
    else
    {
      radiobox.value="on";
      radiobox.checked=true;
    }
  }
  else
  {
  
     radiobox = document.getElementById(id+index);
	if(radiobox == null) return;

    // Set the current one to "checked" and the rest to "unchecked"
    radiobox.value="on";
    radiobox.checked=true;
      for(i=0;i<maxId; i++)
	{
		
if(i==index) continue;
      		radiobox = document.getElementById(id+i);
      		if(radiobox == null) return;

      		radiobox.value="off";
                radiobox.checked=false;
	}
  }
   return false;
}

function jslDoToggleDisabled(preText,currentId,maxId)
{
  var Element,i;
  var ids;

  if(maxId==1)
  {
    Element = document.getElementById(preText+currentId);
	if(Element == null) return;
    if(Element.disabled==true)
     Element.disabled=false;
    else Element.disabled=true;
  }
  else
  {
    Element = document.getElementById(preText+currentId);
	if(Element == null) return;

    // Set the current one to "true" and the rest to "false"
    Element.disabled=false;
   for(i=0;i<maxId; i++)
   {
     if(i==currentId) continue;
     Element = document.getElementById(preText+i);
     if(Element == null) return;
     Element.disabled=true;
   }
  }
  return false;
}

function jslPostToViewCheckBox(ViewId,PostId)
{
  if(document.getElementById(PostId).value=="1")
  {
  	document.getElementById(ViewId).value="on";
       
  	document.getElementById(ViewId).checked=true;
  }
  else
  {
  	document.getElementById(ViewId).value="off";
       
  	document.getElementById(ViewId).checked=false;
  }
}
function jslViewToPostCheckBox(PostId,ViewId)
{
  if(document.getElementById(ViewId).value=="on")
   document.getElementById(PostId).value="1";
  else document.getElementById(PostId).value="0";
}
function jslFormSetPost(id)
{
  document.getElementById(id).method="POST";
}
function jslFormSetGet(id)
{
  document.getElementById(id).method="GET";
}
function jslFormSubmit(id)
{
  document.getElementById(id).submit();
}
function jslPageErrorMsg(msg)
{
  var hint;
  if(msg!="")
  {
    hint=document.getElementById("uiViewErrorHint");
	if(hint!=null) hint.value="Point at the error marker to see error details";
    alert("Check "+msg+" errors.");
	return false;
  }
  return true;
}

function jslGoTo(Menu,Page)
{

  if((Menu!=null) && (Menu!="")) document.getElementById("uiPostMenu").value=Menu;
  if((Page!=null) && (Page!="")) document.getElementById("uiPostPageName").value=Page;
  jslFormSetGet("uiPostForm");
  jslFormSubmit("uiPostForm");
}

