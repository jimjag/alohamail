<?php
/*
FILE: themes/theme_name/info.inc
PURPOSE:
	Various information related to theme
*/

/**
	$THEME_OVERRIDES
		The array contains elements with the
		same keys as $my_colors.  Fields that
		are overrided by the theme (in override.inc)
		should have a value of '1', while 
		settings that aren't overrided should
		have a value of '0'.
**/

$THEME_OVERRIDES = array();
$THEME_OVERRIDES["tool_bg"] = 1;
$THEME_OVERRIDES["tool_link"] = 1;
$THEME_OVERRIDES["folder_bg"] = 1;
$THEME_OVERRIDES["folder_link"] = 1;
$THEME_OVERRIDES["main_bg"] = 1;
$THEME_OVERRIDES["main_hilite"] = 1;
$THEME_OVERRIDES["main_text"] = 1;
$THEME_OVERRIDES["main_link"] = 1;
$THEME_OVERRIDES["main_head_bg"] = 1;
$THEME_OVERRIDES["main_head_txt"] = 1;
$THEME_OVERRIDES["main_darkbg"] = 1;
$THEME_OVERRIDES["main_light_txt"] = 1;
$THEME_OVERRIDES["quotes"]=0;
$THEME_OVERRIDES["font_family"]=0;
$THEME_OVERRIDES["font_size"]=0;
$THEME_OVERRIDES["small_font_size"]=0;
$THEME_OVERRIDES["menu_font_size"]=0;
$THEME_OVERRIDES["folderlist_font_size"]=0;
$THEME_OVERRIDES["error"]=1;


/**
	$THEME_VARS
		Various variables and settings
**/
$THEME_VARS['toolbar_height'] = 30;
$THEME_VARS['logo'] = ''; //url to your logo
$THEME_VARS['logo_href'] = ''; //target url when clicked to logo
$THEME_VARS['logo_alt'] = ''; //alternative text for browsers without images

/**
	$DISPLAY_LIBS
		Display library file name.  Must reside in theme folder.
**/
$DISPLAY_LIB = array();
$DISPLAY_LIB['read_message'] = 'disp_lib_rm.php';
$DISPLAY_LIB['main'] = 'disp_lib_main.php';


/**
	$CSS_URLS
		If you have static CSS files, give the URLs here.
		Static CSS files are faster and give you more control,
		but your users won't be able to change colors and fonts.
		The basic format is:
			$CSS_URLS[<key>] = <url>;
		Where
			<key> : 'default', or file name, e.g. 'main.php'
			<url> : empty string ('') or URL relative to source folder
					or an absolute path/URL (if the CSS file lives somewhere else)
					e.g. 'themes/mytheme/main.css'
**/
$CSS_URLS = array();
$CSS_URLS['default'] = '';	//default, use dynamic CSS

//this line below will use 'themes/default/main.css' for main.php
//
//$CSS_URLS['main.php'] = 'themes/default/main.css';



/**
	$CSS_INCLUDES
		CSS template to be included by source/css.php.
		Values must refer to PHP files inside the theme folder.
		You may use $my_colors values form within such files,
		since $my_colors is loaded in source/css.php
**/
$CSS_INCLUDES = array();
$CSS_INCLUDES['default'] = 'css_main.php';
$CSS_INCLUDES['read_message.php'] = 'css_readmessage.php';
$CSS_INCLUDES['radar.php'] = 'css_radar.php';
$CSS_INCLUDES['main2.php'] = 'css_main.php';
$CSS_INCLUDES['main.php'] = 'css_main.php';

//Add something like below, if you have special CSS
//that's needed only in main.php (or any other file)
//
//$CSS_INCLUDES['main.php'] = 'css_main.php'

?>