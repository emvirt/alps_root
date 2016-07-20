function cnnShowSelectWeatherLocation(fromLink,toSelect) {
	document.getElementById(fromLink).style.display = 'none';
	document.getElementById(toSelect).style.display = 'block';
}

function cnnShowSelectWeatherSelectBoxes(Selected) {
	document.getElementById(Selected).style.display = 'block';
}

function cnnHideCityMatches(id) {
	if (document.getElementById && document.getElementById(id) && document.getElementById(id).style) {
			 document.getElementById(id).style.visibility="hidden";
			 document.getElementById(id).style.display="none";
	};
}

function cnnShowCityMatches(id) {
	if (document.getElementById &&  document.getElementById(id) && document.getElementById(id).style) {

			 document.getElementById(id).style.visibility="visible";
			 document.getElementById(id).style.display="block";
			 
			 document.getElementById('cnnWeatherRegionBox').style.display="none";
			 document.getElementById('cnnWeatherStateBox').style.display="none";
			 document.getElementById('cnnWeatherCityBox').style.display="none";
			
	}; 
	
	document.getElementById('cnnFindWeatherMoreOptions').style.display="block";
	return false;
}

function CNN_changeMap(el,url) { //url argument was intended for making XHR request, I don't know how we're planning on building these from the back end. It may be as simple as including SSI's in the divs below and just showing and hiding the div. If we need to add the ability to make an XHR request to fetch the data that should be trivial to do assuming there aren't cross domain issues to tackle.
	try {
		cnnCheckTab(el.parentNode);
	} catch(e) {};
}

function cnnInsertBadLocBanner(domId){
	$(domId).innerHTML = '<div class="cnn_wthrbnnrgry"><div>Oops, we can\'t find your bookmarked location anymore. Use the \'Find Weather\' box below to select a location, then update your bookmark.</div></div><div class="cnnPad9Top"></div>';
}
function cnnCheckTab(tabID){
	var tabContainer = $('cnnTabHeader').getElementsBySelector('div');
	var maps = $('cnnWeatherMaps').getElementsBySelector('div');		
	var tabs = $$('div#cnnTabNav li');
	var firstTab = tabs[0]
	var lastTab = tabs[tabs.length - 1];
	if (tabID == firstTab) {
		$(tabID).className = "currentLeft";
	} else if (tabID == lastTab) {
		$(tabID).className = "currentRight";
	} else {
		$(tabID).className = "current";
	}
	for (var i = 0; i<tabs.length; i++) {
		if (tabs[i] != tabID && ($(tabs[i]).className != '')) {
			if (i == 0) {
				$(tabs[i]).className = 'cnn2pxLeftPad';
			} else {
				$(tabs[i]).className = '';
			}
			try {
				$(maps[i]).className = 'cnnInactiveMap';	
			} catch(e) {};
		} else if(tabs[i] == tabID) {
			$(tabContainer[0]).className = 'cnnTab'+i;
			try {
				$(maps[i]).className = '';
				//if(!$(maps[i]).hasImg && (tabs.length - 1) != i){ // If have not loaded the map already and if it not the map loaded on page load
				if(!$(maps[i]).hasImg && !$(maps[i]).getElementsByTagName("img")[0].src){
					var cnnMapHTML = $(maps[i]).innerHTML;
					cnnMapHTML = cnnMapHTML.replace('_src', 'src');
					$(maps[i]).innerHTML =  cnnMapHTML;
					$(maps[i]).hasImg = true;
				}

			} catch(e) {};
		}
	}
}

/* styled dropdowns
=========================================================================== */
var cnnDropdownOpen = false;

// CNN dropdown menu (JavaScript object literal)
var cnnDD = {
	curId: "", // id of currently-open dropdown
	ignoreMouseDownBody: false,
	menus: [],

	rowHeight: 17,
	combinedBorderWidth: 20,
	scrollbarWidth: 18,

	minMenuWidth: 105,
	maxMenuWidth: 400,
	defaultMenuWidth: 205,
	defaultRowWidth: 150,
	combinedRowLRPad: 18,
	scrollbarRPad: 12,


	buildDisabledDropdown: function(menuId, buttonWidth, buttonClass, hiddenListSuffix) {
		// default parameters
		if (!buttonWidth) buttonWidth = 140;
		if (!buttonClass) buttonClass = 'cnnDDWireLtg';

		var wrapId = menuId + "_wrap";
		var listId = menuId + "_list" + (hiddenListSuffix ? '_' + hiddenListSuffix : '');

		if ($(wrapId) && $(listId)) {

			// hide the <select>
			$(listId).style.display = "none";

			// Get the displayed value for the first select option
			var listItems = $(listId).options;
			var buttonText = listItems[0].innerHTML;

			var buttonTextLPad = 10;
			var buttonTextRPad = 34;
			var buttonTextWidth = buttonWidth - (buttonTextLPad + buttonTextRPad);

			var leftBgStyle = 'background:#fff url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_ltg_left.gif) 0 0 no-repeat;';
			var rightBgStyle = 'background:url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_ltg_right.gif) 100% 0 no-repeat;';

			switch (buttonClass) {
				case 'cnnDDWire':
					leftBgStyle = 'background:#fff url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_wire_left.gif) 0 0 no-repeat;';
					rightBgStyle = 'background:url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_wire_right.gif) 100% 0 no-repeat;';
					break;
				case 'cnnBlkBgWhtBox':
					leftBgStyle = 'background:#fff url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_blk_left.gif) 0 0 no-repeat;';
					rightBgStyle = 'background:url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_blk_right.gif) 100% 0 no-repeat;';
					break;
				case 'cnnBlueBgWhtBox':
					leftBgStyle = 'background:#fff url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_blue_left.gif) 0 0 no-repeat;';
					rightBgStyle = 'background:url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_blue_right.gif) 100% 0 no-repeat;';
					break;
			}


			// build content for the button
			var strContent = "\n\n\n\n";
			strContent += '	<div class="cnnDDContainer" style="width:'+buttonWidth+'px;">'+"\n";

			strContent += '		<div class="'+buttonClass+'">'+"\n";
			strContent += '			<div class="cnnDDBtn" onmousedown="return cnnDD.mouseDownBtn(event, \''+menuId+'\');" onclick="return cnnDD.open(\''+menuId+'\')" style="'+rightBgStyle+'">'+"\n";
			strContent += '				<table width="'+buttonWidth+'" border="0" cellspacing="0" cellpadding="0">'+"\n";
			strContent += '					<tr>'+"\n";
			strContent += '						<td width="'+buttonTextLPad+'"><div class="cnnDDBtnLeft" style="'+leftBgStyle+'"></div></td>'+"\n";
			strContent += '						<td width="'+buttonTextWidth+'">'+"\n";
			strContent += '							<div class="cnnDDValueContainer">'+"\n";
			strContent += '								<div id="'+menuId+'_Val" class="cnnDDValue" style="width:'+buttonTextWidth+'px;color:#c5c5c5;">'+buttonText+'</div>'+"\n";
			strContent += '						</td>'+"\n";
			strContent += '						<td width="'+buttonTextRPad+'"><img src="http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_icon_disabled.gif" alt="" border="0"></td>'+"\n";
			strContent += '					</tr>'+"\n";
			strContent += '				</table>'+"\n";
			strContent += '			</div><!--/cnnDDBtn -->'+"\n\n";
			strContent += '		</div><!--/'+buttonClass+' -->'+"\n\n";

			strContent += '	</div><!--/cnnDDContainer -->'+"\n";
			strContent += "\n\n";

			// draw the new content
			$(wrapId).innerHTML = strContent;

			// reset the list
			$(listId).selectedIndex = 0;

		}//else id of select not found [ abort ]
	},

	buildDropdown: function(menuId, buttonWidth, menuWidth, numVisibleRows, buttonClass, hiddenListSuffix) {
		// default parameters
		if (!buttonWidth) buttonWidth = 140;
		if (!menuWidth) menuWidth = this.defaultMenuWidth;
		if (!numVisibleRows) numVisibleRows = 10;
		if (!buttonClass) buttonClass = 'cnnDDWireLtg';

		if (menuWidth < this.minMenuWidth) menuWidth = this.minMenuWidth;
		if (menuWidth > this.maxMenuWidth) menuWidth = this.maxMenuWidth;

		var wrapId = menuId + "_wrap";
		var listId = menuId + "_list" + (hiddenListSuffix ? '_' + hiddenListSuffix : '');

		this.menus[menuId] = new Array();
		this.menus[menuId].listId = listId;
		this.menus[menuId].updateFirstRow = false;

		if ($(wrapId) && $(listId)) {
			// hide the <select>
			$(listId).style.display = "none";

			var displayedValue = new Array();
			var internalValue = new Array();
			var disabledRow = new Array();

			var listItems = $(listId).options;
			for (var i=0;i<listItems.length;i++) {
				displayedValue[i] = listItems[i].innerHTML;
				internalValue[i] = listItems[i].value;
				disabledRow[i] = listItems[i].disabled;
			}
			var selectedRow = $(listId).selectedIndex;

			// If no row was explicitly selected
			if (selectedRow == 0) {
				// See if the first row matches one of the later rows
				for (i=1;i<displayedValue.length;i++) {
					if (displayedValue[i] == displayedValue[0]) {
						selectedRow = i;
						this.menus[menuId].updateFirstRow = true;
						break;
					}
				}
			}
			var buttonText = displayedValue[selectedRow];
			var numRows = displayedValue.length;

			var buttonTextLPad = 10;
			var buttonTextRPad = 34;
			var buttonTextWidth = buttonWidth - (buttonTextLPad + buttonTextRPad);

			// minus left and right borders
			var fullRowWidth = menuWidth - this.combinedBorderWidth;

			// without scrollbar
			var visibleRowsHeight = numRows * this.rowHeight;
			var rowWidth = fullRowWidth;

			// with scrollbar
			if (numRows > numVisibleRows) {
				visibleRowsHeight = numVisibleRows * this.rowHeight;
				rowWidth -= 10;
			}

			var leftBgStyle = 'background:#fff url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_ltg_left.gif) 0 0 no-repeat;';
			var rightBgStyle = 'background:url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_ltg_right.gif) 100% 0 no-repeat;';

			switch (buttonClass) {
				case 'cnnDDWire':
					leftBgStyle = 'background:#fff url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_wire_left.gif) 0 0 no-repeat;';
					rightBgStyle = 'background:url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_wire_right.gif) 100% 0 no-repeat;';
					break;
				case 'cnnBlkBgWhtBox':
					leftBgStyle = 'background:#fff url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_blk_left.gif) 0 0 no-repeat;';
					rightBgStyle = 'background:url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_blk_right.gif) 100% 0 no-repeat;';
					break;
				case 'cnnBlueBgWhtBox':
					leftBgStyle = 'background:#fff url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_blue_left.gif) 0 0 no-repeat;';
					rightBgStyle = 'background:url(http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_blue_right.gif) 100% 0 no-repeat;';
					break;
			}


			// build content for the menu
			var strContent = "\n\n\n\n";
			strContent += '	<div class="cnnDDContainer" style="width:'+buttonWidth+'px;">'+"\n";

			strContent += '		<div class="cnnDDBoxContainer">'+"\n";
			strContent += '		<div class="cnnDDBox" id="'+menuId+'" style="width:'+menuWidth+'px;" onmousedown="return cnnDD.mouseDown(event, \''+menuId+'\');">'+"\n";
			strContent += '			<div class="cnnDDBoxHeader"><div class="cnnDDBoxHeaderTL"></div><div class="cnnDDBoxHeaderTR"></div></div>'+"\n";
			strContent += '			<div class="cnnDDBoxContent">'+"\n";

			strContent += '				<div class="cnnDDContent" style="width:'+fullRowWidth+'px;">'+"\n";
			strContent += '					<div class="cnnPad6Top"></div>'+"\n";
			strContent += '					<div class="cnnDDList" style="height:'+visibleRowsHeight+'px; width:'+rowWidth+'px;">'+"\n";
			strContent += '						<ul>'+"\n";

			for (var i=0;i<displayedValue.length;i++) {
				if ((i==0) && (this.menus[menuId].updateFirstRow)) {
					strContent += '						<li id="'+menuId+'_hdnVal"><a href="javascript:cnnDD.select('+i+',\''+this.encodeAttr(displayedValue[i])+'\',\''+this.encodeAttr(internalValue[i])+'\');">'+displayedValue[i]+'</a></li>'+"\n";
				}
				else if (disabledRow[i]) {
					strContent += '						<li class="cnnDDSeparator"><span>'+displayedValue[i]+'</span></li>'+"\n";
				}
				else {
					strContent += '						<li><a href="javascript:cnnDD.select('+i+',\''+this.encodeAttr(displayedValue[i])+'\',\''+this.encodeAttr(internalValue[i])+'\');">'+displayedValue[i]+'</a></li>'+"\n";
				}
			}
			strContent += '						</ul>'+"\n";
			strContent += '					</div>'+"\n";
			strContent += '					<div class="cnnPad8Top"></div>'+"\n";
			strContent += '				</div><!-- /cnnDDContent -->'+"\n";

			strContent += '			</div><!-- /cnnDDBoxContent -->'+"\n";
			strContent += '			<div class="cnnDDBoxFooter"><div class="cnnDDBoxFooterBL"></div><div class="cnnDDBoxFooterBR"></div></div>'+"\n";
			strContent += '		</div><!--/cnnDDBox-->'+"\n";
			strContent += '		</div><!--/cnnDDBoxContainer-->'+"\n";

			strContent += '		<div class="'+buttonClass+'">'+"\n";
			strContent += '			<div class="cnnDDBtn" onmousedown="return cnnDD.mouseDownBtn(event, \''+menuId+'\');" onclick="return cnnDD.open(\''+menuId+'\')" style="'+rightBgStyle+'">'+"\n";
			strContent += '				<table width="'+buttonWidth+'" border="0" cellspacing="0" cellpadding="0">'+"\n";
			strContent += '					<tr>'+"\n";
			strContent += '						<td width="'+buttonTextLPad+'"><div class="cnnDDBtnLeft" style="'+leftBgStyle+'"></div></td>'+"\n";
			strContent += '						<td width="'+buttonTextWidth+'">'+"\n";
			strContent += '							<div class="cnnDDValueContainer">'+"\n";
			strContent += '								<div id="'+menuId+'_Val" class="cnnDDValue" style="width:'+buttonTextWidth+'px;">'+buttonText+'</div>'+"\n";
			strContent += '						</td>'+"\n";
			strContent += '						<td width="'+buttonTextRPad+'"><img src="http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/btn_icon.gif" alt="" border="0"></td>'+"\n";
			strContent += '					</tr>'+"\n";
			strContent += '				</table>'+"\n";
			strContent += '			</div><!--/cnnDDBtn -->'+"\n\n";
			strContent += '		</div><!--/'+buttonClass+' -->'+"\n\n";

			strContent += '	</div><!--/cnnDDContainer -->'+"\n";
			strContent += "\n\n";

			// draw the new content
			$(wrapId).innerHTML = strContent;

			// capture mousedown
			document.body.onmousedown = cnnMouseDown;
		}//else id of select not found [ abort ]
	},

	buildOverlay: function(menuId, menuWidth, numVisibleRows, dx, dy) {
		// default parameters
		if (!menuWidth) menuWidth = this.defaultMenuWidth;
		if (!numVisibleRows) numVisibleRows = 10;

		if (menuWidth < this.minMenuWidth) menuWidth = this.minMenuWidth;
		if (menuWidth > this.maxMenuWidth) menuWidth = this.maxMenuWidth;

		var leftPos = -20;
		var topPos = 1;
		if (dx) leftPos += dx;
		if (dy) topPos += dy;

		var wrapId = menuId + "_wrap";
		var listId = menuId + "_list";
		var titleId = menuId + "_title";

		if ($(wrapId) && $(titleId) && $(listId)) {
			// hide the list
			$(listId).style.display = "none";

			var title = $(titleId).innerHTML;

			// Get the displayed value for each select option
			var listItems = $(listId).getElementsByTagName('li');
			var displayedList = new Array();
			for (var i=0;i<listItems.length;i++) {
				displayedList[i] = listItems[i].innerHTML;
			}

			var numRows = displayedList.length;

			var menuTitleRPad = 60;
			var menuTitleWidth = menuWidth - menuTitleRPad;

			// minus left and right borders
			var fullRowWidth = menuWidth - this.combinedBorderWidth;

			// without scrollbar
			var visibleRowsHeight = numRows * this.rowHeight;
			var rowWidth = menuWidth - this.combinedBorderWidth;

			// with scrollbar
			if (numRows > numVisibleRows) {
				visibleRowsHeight = numVisibleRows * this.rowHeight;
				rowWidth -= 10;
			}


			// build content for the menu
			var strContent = "\n\n\n\n";
			strContent += ' <div class="cnnDDOvrBoxContainer">'+"\n";
			strContent += '		<div class="clear"><img src="http://i.cdn.turner.com/cnn/images/1.gif" width="1" height="1" border="0" alt=""></div>'+"\n";
			strContent += '		<div class="cnnDDOvrBox" id="'+menuId+'" style="width:'+menuWidth+'px;left:'+leftPos+'px; top:'+topPos+'px;" onmousedown="return cnnDD.mouseDown(event, \''+menuId+'\');">'+"\n";
			strContent += '			<div class="cnnDDBoxHeader"><div class="cnnDDBoxHeaderTL"></div><div class="cnnDDBoxHeaderTR"></div></div>'+"\n";
			strContent += '			<div class="cnnDDBoxContent">'+"\n";
			strContent += '				<div class="cnnDDOvrCloseContainer"><div class="cnnDDOvrClose" onclick="cnnDD.close(); return true;"><img src="http://i.cdn.turner.com/cnn/.element/img/2.0/global/dropdowns/overlay_close.png" width="12" height="12" alt="" border="0"></div></div>'+"\n";
			strContent += '				<div class="cnnDDContent" style="width:'+fullRowWidth+'px;">'+"\n";
			strContent += '					<div class="cnnDDOvrTitle" style="width:'+menuTitleWidth+'px;overflow:hidden;">'+title+'</div>'+"\n";
			strContent += '					<div class="cnnDDList" style="height:'+visibleRowsHeight+'px;width:'+rowWidth+'px;">'+"\n";
			strContent += '						<ul>'+"\n";

			for (var i=0;i<displayedList.length;i++) {
				strContent += '					<li>'+displayedList[i]+'</li>'+"\n";
			}
			strContent += '						</ul>'+"\n";
			strContent += '					</div><!-- /cnnDDList -->'+"\n";
			strContent += '					<div class="cnnPad12Top"></div>'+"\n";
			strContent += '				</div><!-- /cnnDDContent -->'+"\n";
			strContent += '			</div><!-- /cnnDDBoxContent -->'+"\n";
			strContent += '			<div class="cnnDDBoxFooter"><div class="cnnDDBoxFooterBL"></div><div class="cnnDDBoxFooterBR"></div></div>'+"\n";
			strContent += '		</div><!--/cnnDDOvrBox-->'+"\n";
			strContent += ' </div><!--/cnnDDOvrBoxContainer-->'+"\n";
			strContent += "\n\n";
			// draw the new content
			$(wrapId).innerHTML = strContent;

			// capture mousedown
			document.body.onmousedown = cnnMouseDown;

		}//else id of select not found [ abort ]
	},


	select: function(index, displayedValue, internalValue) {
		if ($(this.curId)) {
			var menuId = this.curId;

			// close the dropdown
			this.close();

			// change the displayed dropdown value (button text)
			if ($(menuId + '_Val')) {
				$(menuId + '_Val').innerHTML = displayedValue;
			}

			// set the first row of the menu to the current value
			if ((this.menus[menuId].updateFirstRow) && $(menuId + '_hdnVal')) {
				$(menuId+'_hdnVal').innerHTML = '<a href="javascript:cnnDD.select(' + index + ',\'' + this.encodeAttr(displayedValue) + '\',\'' + this.encodeAttr(internalValue) + '\')">' + displayedValue + '</a>';
			}

			var listId = this.menus[menuId].listId;
			if ($(listId)) {
				// if the value has changed
				if ($(listId).selectedIndex != index) {
					// set the index of the selected option for the invisible <select>
					$(listId).selectedIndex = index;

					// If an onchange event handler exists
					if ($(listId).onchange) {
						$(listId).onchange();
					}
				}
			}

			// if a callback function exists
			try {
				var onChoose = eval(menuId + '_OnChoose');
				if (onChoose) {
					onChoose();
				}
			}
			catch(err) {
			}
		}
	},

	open: function(id) {
		if($(id)) {
			// Was the same menu clicked again?
			var sameMenu = (this.curId == id);

			// If a menu is already open
			this.close();

			// If a different menu was clicked
			if (!sameMenu) {
				$(id).style.display = "block";
				this.curId = id;
				cnnDropdownOpen = true;
			}
		}
	},

	close: function() {
		if ($(this.curId)) {
			$(this.curId).style.display = "none";
			this.curId = '';
			cnnDropdownOpen = false;
		}
	},

	encodeAttr: function(str) {
		str=str.replace(/\\/g,'\\\\');
		str=str.replace(/\'/g,'\\\'');
		str=str.replace(/\"/g,'&quot;');
		str=str.replace(/\0/g,'\\0');
		return str;
	},

	mouseDown: function(e, id) {
		this.ignoreMouseDownBody = true;
		return true;
	},

	mouseDownBtn: function(e, id) {
		// True if the same dropdown button was clicked again.
		this.ignoreMouseDownBody = (id && (this.curId == id));
		return true;
	},

	mouseDownBody: function(e) {
		if (!this.ignoreMouseDownBody) {
			this.close();
		}
		this.ignoreMouseDownBody = false;
		return true;
	}
}
/* end styled dropdowns
=========================================================================== */
