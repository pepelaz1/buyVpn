<?php
	$hosts = "safe-inet.com;www.host32423.com;www.google.com";
    $prog_ver = "1.0";
    $prog_url = "http://safe-inet.com/download/BuyVpn_Install_1.0.exe";
    $config_ver = "1.0";
    $config_url = "http://safe-inet.com/download/config.zip";
	
	echo base64_encode($hosts)."\n".$prog_ver."\n".echo base64_encode($prog_url)."\n".$config_ver."\n".echo base64_encode($config_url)."\n";
?>