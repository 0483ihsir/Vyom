<?php

$d=$_GET['distance'];
if(isset($d))
{
    $fp=fopen("distance.txt","w");
   // $d=time();
    fwrite($fp,$d);
    fclose($fp);
}

?>