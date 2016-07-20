var AIS_VERSION = "ais.cnn.2";
var cnnad_transactionID = null;
function cnnad_getTransactionID() {
	if (cnnad_transactionID == null) {
		cnnad_transactionID = "";
		cnnad_transactionID = Math.round((new Date()).getTime() / 1000) + '' + Math.floor(Math.random()*9007199254740992);
	}
	return cnnad_transactionID;
}
function cnnad_readCookie( name ) {
	if ( document.cookie == '' ) { // there is no cookie, so go no further
		return null;
	} else { // there is a cookie
		var ca = document.cookie.split(';');
		var nameEQ = name + "=";
		for(var i=0; i < ca.length; i++) {
			var c = ca[i];
			while (c.charAt(0)==' ') c = c.substring(1, c.length); //delete spaces
			if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length, c.length);
		}
		return null;
	}
}
function cnnad_haveCookie(a){return document.cookie&&(-1<document.cookie.indexOf("; "+a+"=")||document.cookie.indexOf(a+"=")==0);}
function cnnad_ugsync() {
        if (!cnnad_haveCookie('ugs')) {
		document.write('<scr'+'ipt src="http://www.ugdt'+'urner.com/xd.sjs"></scr'+'ipt>');
        }
}
cnnad_getTransactionID();
cnnad_ugsync();
/* ControlTagCNN.js */
window.Krux||((Krux=function(){Krux.q.push(arguments)}).q=[]);
(function(){
	var k=document.createElement('script');k.type='text/javascript';k.async=true;
	var m,src=(m=location.href.match(/\bkxsrc=([^&]+)/))&&decodeURIComponent(m[1]);
	k.src = /^https?:\/\/([^\/]+\.)?krxd\.net(:\d{1,5})?\//i.test(src) ? src : src === "disable" ? "" :
	(location.protocol==="https:"?"https:":"http:")+"//cdn.krxd.net/controltag?confid=ITb_4eqO";
	var s=document.getElementsByTagName('script')[0];s.parentNode.insertBefore(k,s);
}());
/* KruxInterchangeDirectTag.rev3.js */
window.Krux||((Krux=function(){Krux.q.push(arguments);}).q=[]);
window.kvs = [];
(function getKruxData(){
    function retrieve(n){
        var m, k='kx'+n;
        if (window.localStorage) {
            return window.localStorage[k] || "";
        } else if (navigator.cookieEnabled) {
            m = document.cookie.match(k+'=([^;]*)');
            return (m && unescape(m[1])) || "";
        } else {
            return ''; }
    }
    Krux.user = retrieve('user');
    Krux.segments = retrieve('segs') && retrieve('segs').split(',') || [];
    for (var i = 0; i < Krux.segments.length; i++ ) {
        if (kvs.length < 20) kvs.push(Krux.segments[i]);
    }
})();

function krux_getDESegments(){
    var segmentString = "&kxid=";
    if (Krux.user) {
        segmentString += Krux.user;
    }
    segmentString += '&kxseg=' + kvs.join(",");
    return segmentString;
}

function krux_getFWSegments(){
    return 'kxseg=' + kvs.join(",kxseg=");
}
