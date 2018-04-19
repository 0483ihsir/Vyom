<?php

$d=$_GET['data'];
if(isset($d))
{
    $fp=fopen("mode.txt","w");
   // $d=time();
    fwrite($fp,$d);
    fclose($fp);
}
echo "success";

?>