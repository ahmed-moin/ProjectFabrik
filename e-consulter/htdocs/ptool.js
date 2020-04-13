function Mouse(which, img)
{
   document.images[which].src = img;
   window.status = which;
}

function Maskenwechsel(which)
{
   document.forms[0].menu.value = '';
   document.forms[0].folgemaske.value = which;
   document.forms[0].submit();
}

function Menuwechsel(which)
{
   document.forms[0].menu.value = which;
   document.forms[0].submit();
}

function MaskeMenuwechsel(whichmaske, whichmenu)
{
  document.forms[0].folgemaske.value = whichmaske;
  document.forms[0].menu.value = whichmenu;
  document.forms[0].submit();
}

function Tool(tool)
{
  document.forms[0].action = tool;
  document.forms[0].maske.value = "";
  document.forms[0].submit();
}

function ToolMaske(tool, maske)
{
  document.forms[0].action = tool;
  document.forms[0].folgemaske.value = maske;
  document.forms[0].submit();
}

function SubmitIf(value)
{
   if (value)
   {
     document.forms[0].folgemaske.value = document.forms[0].maske.value;
     document.forms[0].submit();
   }
}

function SubmitUnless(value)
{
   if (value == 0)
   {
     document.forms[0].folgemaske.value = document.forms[0].maske.value;
     document.forms[0].submit();
   }
}

function SubmitEq(value, cmp)
{
   if (value == cmp)
   {
     document.forms[0].folgemaske.value = document.forms[0].maske.value;
     document.forms[0].submit();
   }
}

function SubmitNe(value, cmp)
{
   if (value != cmp)
   {
     document.forms[0].folgemaske.value = document.forms[0].maske.value;
     document.forms[0].submit();
   }
}

function Aktion(value)
{
  document.forms[0].aktion.value = value;
  document.forms[0].submit();
}

function changeFolder(which)
{
  document.forms[0].folgemaske.value = document.forms[0].maske.value;
  document.forms[0].folder.value = which;
  document.forms[0].submit();
}

function download(which)
{
  document.forms[0].folgemaske.value = document.forms[0].maske.value;
  document.forms[0].download.value = which;
  document.forms[0].submit();
}

