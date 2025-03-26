/*============================================================================
 Where are the errors in this code? Any chance of Run_Time_Errors?
============================================================================*/
int new_position(int sensor_pos1 , int sensor_pos2)
{
	int actuator_position;
	int x, y, tmp, magnitude,err;

	actuator_position = 2; 
	tmp = 0;
	magnitude = sensor_pos1/100;
        y =  magnitude + 5;         
	

	while (actuator_position < 10)
	{
		actuator_position++;
		tmp+= sensor_pos2 / 100;
		y += 3;
	}
	
	if (((3*magnitude) + 100) > 43)
	{
        magnitude++;
		x = actuator_position;
		actuator_position = x / (x - y);
	}
	return actuator_position*magnitude + tmp;
}
