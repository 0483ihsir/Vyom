<?php


if ($_GET[data]== 'Good')
{
	
	
		$s="People with respiratory or heart disease, the elderly and children are the groups most at risk.";
	
	
}
else if ($_GET[data] == 'Moderate')
{
	
	
		$s="People with respiratory or heart disease, the elderly and children are the groups most at risk.";
		$s.="Unusually sensitive people should consider reducing prolonged or heavy exertion.";
		$s.="Unusually sensitive people should consider reducing prolonged or heavy exertion.";	
	
}
else if ($_GET[data]   == 'Unhealthy for Sensitive Groups')
{
	
		$s="People with respiratory or heart disease, the elderly and children are the groups most at risk.";
		$s.="Increasing likelihood of respiratory symptoms in sensitive individuals, aggravation of heart or lung disease and premature mortality in persons with cardiopulmonary disease and the elderly.";
		$s.="People with respiratory or heart disease, the elderly and children should limit prolonged exertion.";
	
}
else if ($_GET[data]  == 'Unhealthy')
{
	
		$s="People with respiratory or heart disease, the elderly and children are the groups most at risk.";
		$s.="Increased aggravation of heart or lung disease and premature mortality in persons with cardiopulmonary disease and the elderly; increased respiratory effects in general population.";
		$s.="People with respiratory or heart disease, the elderly and children should avoid prolonged exertion; everyone else should limit prolonged exertion.";
	
}
else if ($_GET[data]  == 'Very Unhealthy')
{
	
		$s="People with respiratory or heart disease, the elderly and children are the groups most at risk.";
		$s.="Significant aggravation of heart or lung disease and premature mortality in persons with cardiopulmonary disease and the elderly; significant increase in respiratory effects in general population.";
		$s.="People with respiratory or heart disease, the elderly and children should avoid any outdoor activity; everyone else should avoid prolonged exertion.";
	
}
else if ($_GET[data]  == 'Hazardous')
{
	
		$s="People with respiratory or heart disease, the elderly and children are the groups most at risk.";
		$s.="Serious aggravation of heart or lung disease and premature mortality in persons with cardiopulmonary disease and the elderly; serious risk of respiratory effects in general population.";
		$s.="Everyone should avoid any outdoor exertion; people with respiratory or heart disease, the elderly and children should remain indoors.";
	
}
else
{
	
	$s="";
	$s.="";

}

echo $s;




?>