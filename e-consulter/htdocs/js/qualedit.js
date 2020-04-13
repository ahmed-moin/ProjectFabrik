var win;

function edit(id, parentId, rate, name) {
  var qid = document.forms[0].qid;
  var qparent = document.forms[0].qparent;
  var qrate = document.forms[0].qrate;
  var qname = document.forms[0].qname;

  if (!win) {
    win = new Ext.Window({
      renderTo: 'mainform',
      y: window.pageYOffset + 200 + 'px',
      layout:'fit',
      width:430,
      height:190,
      closable: false,
      closeAction:'hide',
      plain: true,
      resizable: false,
      contentEl: 'qualform',
      title: 'Qualifikation editieren',

      buttons: [{
        text:'Abbrechen',
        handler: function() {
          qid.value = '';
          win.hide();
        }
      },{
          text: 'Speichern',
        handler: function(){
          if (qid.value == qparent.value) {
            alert('Qualifikation und uebergeordnete Qualifikation sind gleich!');
            return;
          }

          document.forms[0].submit();
        }
      }]
    });
  }
  
  qparent.options[ getPos(qparent, parentId) ].selected = true;
  qname.value = name;
  qrate.checked = rate;
  qid.value = id;

  win.show(this);
}


function del(id, name) {
  var ok = window.confirm("Qualifikation '" + name + "' wirklich loeschen? Alle Bewertungen bei Benutzern zu dieser Qualifikation werden ebenfalls geloescht und die untergeordneten Qualifikationen werden 'Nicht zugeordnet' zugewiesen!");

  if (ok) {
    document.forms[0].qid.value  = id;
    document.forms[0].qdel.value = 'd';

    document.forms[0].submit();
  }
}


function getPos(combo, id) {
  for (var i=0; i < combo.options.length; ++i) {
    if (combo.options[i].value == id)
      return i;
  }

  return 0;
}

