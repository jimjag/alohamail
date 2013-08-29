<?php
/********************************************************

	FILE: include/pref_header.inc
	PURPOSE:
		Show header in "source/prefs.php" and "source/pref_colors.php"
		
********************************************************/
?>
<table border="0" cellspacing="2" cellpadding="0" width="100%">
<tr class="dk"><td><span class="bigTitle"><?php echo $prefHeaderStrings[0]?></span></td></tr>
<tr><td class="mainLight">
    [<a href="prefs.php?user=<?php echo $sid; ?>" class="mainLight"><?php echo $prefHeaderStrings[1]?></a>]
    [<a href="pref_identities.php?user=<?php echo $sid; ?>" class="mainLight"><?php echo $prefHeaderStrings[3]?></a>]
    <?php
	if ($ICL_CAPABILITY["filters"])
		echo '[<a href="pref_filters.php?user='.$sid.'" class="mainLight">'.$prefHeaderStrings[4].'</a>]';
    ?>
	[<a href="pref_colors.php?user=<?php echo $sid; ?>" class="mainLight"><?php echo $prefHeaderStrings[2]?></a>]
	<?php
	if ($GPG_ENABLE){
		echo '[<a href="pref_gpg.php?user='.$sid.'" class="mainLight">GPG</a>]';
	}
	include_once('../conf/plugins.php');
	if (is_array($PLUGIN_HANDLERS['prefs'])){
		foreach($PLUGIN_HANDLERS['prefs'] as $piname=>$pi_a){
			if (!$pi_a['on']) continue;
			echo ' [<a href="pref_shell.php?user='.$sid.'&plugin='.$piname.'" class="mainLight">'.$pi_a['label'].'</a>]';
		}
	}
	?>
</td></tr>
</table>
