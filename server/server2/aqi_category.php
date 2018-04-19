<?php

if(isset($_GET['data']))echo AQICategory($_GET['data']);

function AQICategory($AQIndex)
{
$AQI=(float)$AQIndex;
$AQICategory;
if ($AQI<=50)
{
	$AQICategory="Good";
}
else if ($AQI>50 && $AQI<=100)
{
	$AQICategory="Moderate";
}
else if ($AQI>100 && $AQI<=150)
{
	$AQICategory="Unhealthy for Sensitive Groups";
}
else if ($AQI>150 && $AQI<=200)
{
	$AQICategory="Unhealthy";
}
else if ($AQI>200 && $AQI<=300)
{
	$AQICategory="Very Unhealthy";
}
else if ($AQI>300 && $AQI<=400)
{
	$AQICategory="Hazardous";
}
else if ($AQI>400 && $AQI<=500)
{
	$AQICategory="Hazardous";
}
else
{
	$AQICategory="Out of Range";
}
return $AQICategory;
}

?>