<?php
/********************************************************
	conf/defaults.inc
		
	PURPOSE:
		Provide central location for specifying text to be display below login form

********************************************************/

?>
	<p>Powered by <a href="http://gitorious.org/alohamail">AlohaMail</a>
	<?php
		if (($v_stat=="Dev") || ($v_stat=="Devel")){
	?>
	<p>This version is still under development.
	<br>Parts of it may break or change suddenly without warning.
	<p>Mail bug reports to <a href="mailto:projects@sourismaline.fr">SourisMaline</a> 
	<?php
		}else{
	/*
	?>
	Visit <a href="http://ilohamail.org">IlohaMail.org</a> for more information on this program.
	<?php
	*/
	}

?>
        </center>
</body>

