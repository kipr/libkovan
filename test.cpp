#include <kovan/kovan.h>
#include <stdio.h>

int main()
{   // Begin main function code block.
	// 1. Attempt to open a connection to the depth sensor.
	if (depth_open() == 0)
	{
		printf("Failed to connect to depth sensor\n");
		return 1;
	}
	
	printf("Press SIDE-button to continue...\n");
	// while (!side_button());
	
	// Instantiate local variables.
	int n          = 0;
	int center_x   = 0;
	int center_y   = 0;
	int center_z   = 0;
	int nearest_x  = 0;
	int nearest_y  = 0;
	int nearest_z  = 0;
	int farthest_x = 0;
	int farthest_y = 0;
	int farthest_z = 0;
	int size       = 0;
	int angle      = 0;
	int row        = get_depth_image_height() / 2;
	
	// 2. Until the B-button is pressed, use the depth sensor.
	while (1)
	{
		depth_update();  // Update the depth image.
		depth_scanline_update(row);
		
		display_clear();
		
		n = get_depth_scanline_object_count();
		if (n > 0)
		{
			center_x   = get_depth_scanline_object_center_x(0);
			center_y   = get_depth_scanline_object_center_y(0);
			center_z   = get_depth_scanline_object_center_z(0);
			nearest_x  = get_depth_scanline_object_nearest_x(0);
			nearest_y  = get_depth_scanline_object_nearest_y(0);
			nearest_z  = get_depth_scanline_object_nearest_z(0);
			farthest_x = get_depth_scanline_object_farthest_x(0);
			farthest_y = get_depth_scanline_object_farthest_y(0);
			farthest_z = get_depth_scanline_object_farthest_z(0);
			size       = get_depth_scanline_object_size(0);
			angle      = get_depth_scanline_object_angle(0);
			
			printf("n = %d\n", n);
			printf("center(%d, %d, %d)\n", center_x, center_y, center_z);
			printf("nearest(%d, %d, %d)\n", nearest_x, nearest_y, nearest_z);
			printf("farthest(%d, %d, %d)\n", farthest_x, farthest_y, farthest_z);
			printf("size = %d mm\n", size);
			printf("angle = %d-degrees\n", angle);
		}
		else
		{
			printf("No scanline objects found...\n");
		}
		
		msleep(100);
	}
	
	// 3. Close the connection to the depth sensor.
	depth_close();
	
	// 4. Tell the user program has finished.
	printf("Done!\n");
	
	// 5. Exit the program by returning 0.
	return 0;
}   // End main function code block.
