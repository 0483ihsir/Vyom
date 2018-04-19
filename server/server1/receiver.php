<?php

$d=$_GET['data'];
if(isset($d))
{
    $fp=fopen("time.txt","w");
   // $d=time();
    fwrite($fp,$d);
    fclose($fp);
 // echo AQIPM25($d);
}

function Linear($AQIhigh, $AQIlow,$Conchigh, $Conclow, $Concentration)
{
$linear;
$Conc=(float)($Concentration);
$a;
$a=(($Conc-$Conclow)/($Conchigh-$Conclow))*($AQIhigh-$AQIlow)+$AQIlow;
$linear=round($a);
return $linear;
}


function AQIPM25($Concentration)
{
$Conc=(float)$Concentration;
$c;
$AQI;
$c=(floor(10*$Conc))/10;
if ($c>=0 && $c<12.1)
{
	$AQI=Linear(50,0,12,0,$c);
}
else if ($c>=12.1 && $c<35.5)
{
	$AQI=Linear(100,51,35.4,12.1,$c);
}
else if ($c>=35.5 && $c<55.5)
{
	$AQI=Linear(150,101,55.4,35.5,$c);
}
else if ($c>=55.5 && $c<150.5)
{
	$AQI=Linear(200,151,150.4,55.5,$c);
}
else if ($c>=150.5 && $c<250.5)
{
	$AQI=Linear(300,201,250.4,150.5,$c);
}
else if ($c>=250.5 && $c<350.5)
{
	$AQI=Linear(400,301,350.4,250.5,$c);
}
else if ($c>=350.5 && $c<500.5)
{
	$AQI=Linear(500,401,500.4,350.5,$c);
}
else if ($c>=500.5 && $c<1000)
{
	$AQI=Linear(600,501,650.4,450.5,$c);
}
else
{
	$AQI=773;
}
return $AQI;
}

?>