#include "stdafx.h"
#include <time.h>
#include <cstdlib>
#include <ctime>
#include "MapGen.h"

int Total_Rooms = 12; // The number of rooms
int Max_Room_Size = 20; // Maximum room size
int Min_Room_Size = 6; // Minimum room size
int Room_Overlap = 0; // Can rooms overlap?

	// Create the central tunnel
void Make_Special_Tunnel(char* map) {

	int Location = Size + 4; // start at the top of the map
	
	int Bottom = Size * Size; // dig to lower part of the map
	Bottom -= Size * 2;
	
	int direction;
	int amount;

	int x;
	
	while (Location < Bottom) {
	
		// choose a direction, down or right
		direction = rand();
		direction = direction % 2;

		amount = rand();
		amount = (amount % 6) + 4;

		for (x = 0; x < amount; x++) {
		
			if (direction == 0) { // down
			
				map[Location] = 'o';
				Location += Size;
			}

			else { // right
			
				map[Location] = 'o';
				Location++;
			}
		}
	}
}

	// Create randomly sized rooms
void Make_Rooms(char* map) { 

	int Rooms;
	int empty;
	int Height;
	int Width;
	int x;
	int y;
	int xy;

	for (Rooms = Total_Rooms; Rooms != 0; Rooms--) { // Run the loop for each Room
		
		// Get a random Location in the map
		int Total_Size = Size * Size;
		int Location = rand();
		Location %= Total_Size;

		// Get random room dimensions
		Height = rand();
		Height %= Max_Room_Size;
		if (Height < Min_Room_Size) { Height = Min_Room_Size; }

		Width = rand();
		Width %= Max_Room_Size;
		if (Width < Min_Room_Size) { Width = Min_Room_Size; }

		empty = 1; // Assume location is empty

		// Check to see if there is already a room here
		for (y = 0; y < Height; y++) {
		
			for (x = 0; x < Width; x++) {
			
				xy = Location;
				xy += y * Size;
				xy += x;

				if (map[xy] == '.') {
				
					empty = 0;
				}
			}
		}

		if (Room_Overlap == 1) {
		
			empty = 1;
		}

		// If space is empty make the new room
		if (empty == 1) {
		
			for (y = 0; y < Height; y++) {

				for (x = 0; x < Width; x++) {

					xy = Location;
					xy += y * Size;
					xy += x;

					map[xy] = '.';
				}
			}

			map[xy + Size] = 'T'; // mark tiles for future tunnels
		}
	}
}

	// Fill map with #
void Fill_Map(char* map) {

	int y;
	int Total_Size = Size * Size;

	for (y = 0; y != Total_Size; y++) {
	
		map[y] = '#';
	}
}

	// Close off the top and bottom of the map
void Close_Map(char* map) {

	int x;
	int Bottom_Row;

	for (x = 0; x < Size; x++) {
		
		map[x] = '#';

		Bottom_Row = Size*Size;
		Bottom_Row -= Size;
		map[Bottom_Row + x] = '#';
	}

	
}

	// Create tunnels from each room to the central tunnel
void Tunnel(char* map) {

	int x = 0;
	int Total_Size = Size*Size;
	Total_Size -= Size;

	int direction;
	int amount = 0;

	int location;
	int top_or_bot;

	while (x < Total_Size) { // scane through the map
	
		if (map[x] == 'T') { // if the start of a tunnel is found
		
			location = x;
			if (location < (Size*Size) / 2) { // top half will dig down
			
				top_or_bot = 0;
			}
			
			else { // bottom half will dig up
			
				top_or_bot = 1;
			}
			
			while (map[location] != 'o' || location >= Size*Size || location <=0) { // tunnel until you find 'o'

				map[location] = '.'; // Place a floor tile
				
				if (amount == 0) { // choose a new direction and amount 
					
					direction = rand();
					direction %= 4;

					if (direction == 0 && top_or_bot == 0) { // top half can't dig up
					
						direction = rand();
						direction = (direction % 3) + 1;
					}
					
					if (direction == 3 && top_or_bot == 1) { // bottom half can't dig down
					
						direction = rand();
						direction %= 3;
					}
					
					amount = rand();
					amount %= 6;
					amount += 4;
				}
				
				if (direction == 0) { // up
				
					location -= Size;
					amount--;
				}

				else if (direction == 1) { // right
				
					location++;
					amount--;
				}

				else if (direction == 2) { // left
				
					location--;
					amount--;
				}

				else { // down
				
					location += Size;
					amount--;
				}
			}
		}

		x++;
	}
}

	// close off the top and bottom with 'o'
void Close_Special(char* map) {

	int x;
	int Bottom_Row;

	for (x = 0; x < Size; x++) {

		map[x] = 'o';

		Bottom_Row = Size*Size;
		Bottom_Row -= Size;
		map[Bottom_Row + x] = 'o';
	}


}

	// Clear all 'o' characters
void Clear_Special(char* map) {

	int x;

	for (x = 0; x < Size*Size; x++) {
	
		if (map[x] == 'o') {
		
			map[x] = '.';
		}
	}
}

	// Fill in dead ends
void Fill_Dead_Ends(char* map) {

	int x;
	int count;

	int Total_Size = Size*Size;
	Total_Size -= Size;
	Total_Size--;

	for (x = Size ; x < Total_Size; x++) { // Scan top to bottom, only gets half
	
		count = 0;

		if (map[x - Size] == '#') { // up
		
			count++;
		}

		if (map[x + 1] == '#') { // right
		
			count++;
		}

		if (map[x + Size] == '#') { // down
		
			count++;
		}

		if (map[x - 1] == '#') { // left
		
			count++;
		}

		if (count == 3) { // If the tile is surrounded by 3 '#' it bacomes '#'
		
			map[x] = '#';
		}
	}

	for (x = Total_Size; x > Size; x--) { // Scan bottom to top, gets the other half

		count = 0;

		if (map[x - Size] == '#') { // up

			count++;
		}

		if (map[x + 1] == '#') { // right

			count++;
		}

		if (map[x + Size] == '#') { // down

			count++;
		}

		if (map[x - 1] == '#') { // left

			count++;
		}

		if (count == 3) { // If the tile is surrounded by 3 '#' it bacomes '#'

			map[x] = '#';
		}
	}
}

	// Clear up lone walls
void Clear_Lone_Walls(char* map) {

	int x;
	int count;

	int Total_Size = Size*Size;
	Total_Size -= Size;
	Total_Size--;

	for (x = Size; x < Total_Size; x++) { // Scan top to bottom, only gets half

		count = 0;

		if (map[x - Size] == '.') { // up

			count++;
		}

		if (map[x + 1] == '.') { // right

			count++;
		}

		if (map[x + Size] == '.') { // down

			count++;
		}

		if (map[x - 1] == '.') { // left

			count++;
		}

		if (count == 3 || count == 4) { // If the tile is surrounded by 3 '.' it bacomes '.'

			map[x] = '.';
		}
	}

	for (x = Total_Size; x > Size; x--) { // Scan bottom to top, gets the other half

		count = 0;

		if (map[x - Size] == '.') { // up

			count++;
		}

		if (map[x + 1] == '.') { // right

			count++;
		}

		if (map[x + Size] == '.') { // down

			count++;
		}

		if (map[x - 1] == '.') { // left

			count++;
		}

		if (count == 3 || count == 4) { // If the tile is surrounded by 3 '.' it bacomes '.'

			map[x] = '.';
		}
	}
}

char* Map_Generate() {

	char* map = new char[Size*Size];

	Fill_Map(map);
	Make_Rooms(map);
	Make_Special_Tunnel(map); 
	Close_Special(map); 
	Tunnel(map); 
	Clear_Special(map);
	Close_Map(map);
	
	Fill_Dead_Ends(map);
	//Clear_Lone_Walls(map);

	return map;
}