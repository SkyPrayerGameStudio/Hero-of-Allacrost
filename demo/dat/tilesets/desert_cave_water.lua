local ns = {}
setmetatable(ns, {__index = _G})
desert_cave_water = ns;
setfenv(1, ns);

file_name = "dat/tilesets/desert_cave_water.lua"
image = "img/tilesets/desert_cave_water.png"
num_tile_cols = 16
num_tile_rows = 16

walkability = { [0] = {}, [1] = {}, [2] = {}, [3] = {}, 
		[4]= {}, [5] = {}, [6] = {}, [7] = {},
		[8] = {}, [9] = {}, [10] = {}, [11] = {}, 
		[12] = {}, [13] = {}, [14] = {}, [15] = {} 
}


walkability[0][0]   = { 1, 1, 1, 1 }
walkability[0][1]   = { 1, 1, 1, 1 }
walkability[0][2]   = { 1, 1, 1, 1 }
walkability[0][3]   = { 1, 1, 1, 1 }
walkability[0][4]   = { 1, 1, 1, 1 }
walkability[0][5]   = { 1, 1, 1, 1 }
walkability[0][6]   = { 1, 1, 1, 1 }
walkability[0][7]   = { 1, 1, 1, 1 }
walkability[0][8]   = { 1, 1, 1, 1 }
walkability[0][9]   = { 1, 1, 1, 1 }
walkability[0][10]  = { 1, 1, 1, 1 }
walkability[0][11]  = { 1, 1, 1, 1 }
walkability[0][12]  = { 1, 1, 1, 1 }
walkability[0][13]  = { 1, 1, 1, 1 }
walkability[0][14]  = { 1, 1, 1, 1 }
walkability[0][15]  = { 0, 0, 0, 0 }
walkability[1][0]   = { 1, 1, 1, 1 }
walkability[1][1]   = { 1, 1, 1, 1 }
walkability[1][2]   = { 1, 1, 1, 1 }
walkability[1][3]   = { 1, 1, 1, 1 }
walkability[1][4]   = { 1, 1, 1, 1 }
walkability[1][5]   = { 1, 1, 1, 1 }
walkability[1][6]   = { 1, 1, 1, 1 }
walkability[1][7]   = { 1, 1, 1, 1 }
walkability[1][8]   = { 1, 1, 1, 1 }
walkability[1][9]   = { 1, 1, 1, 1 }
walkability[1][10]  = { 1, 1, 1, 1 }
walkability[1][11]  = { 1, 1, 1, 1 }
walkability[1][12]  = { 1, 1, 1, 1 }
walkability[1][13]  = { 1, 1, 1, 1 }
walkability[1][14]  = { 1, 1, 1, 1 }
walkability[1][15]  = { 0, 0, 0, 0 }
walkability[2][0]   = { 1, 1, 1, 1 }
walkability[2][1]   = { 1, 1, 1, 1 }
walkability[2][2]   = { 1, 1, 1, 1 }
walkability[2][3]   = { 1, 1, 1, 1 }
walkability[2][4]   = { 1, 1, 1, 1 }
walkability[2][5]   = { 1, 1, 1, 1 }
walkability[2][6]   = { 1, 1, 1, 1 }
walkability[2][7]   = { 1, 1, 1, 1 }
walkability[2][8]   = { 1, 1, 1, 1 }
walkability[2][9]   = { 1, 1, 1, 1 }
walkability[2][10]  = { 1, 1, 1, 1 }
walkability[2][11]  = { 1, 1, 1, 1 }
walkability[2][12]  = { 1, 1, 1, 1 }
walkability[2][13]  = { 1, 1, 1, 1 }
walkability[2][14]  = { 1, 1, 1, 1 }
walkability[2][15]  = { 0, 0, 0, 0 }
walkability[3][0]   = { 1, 1, 1, 1 }
walkability[3][1]   = { 1, 1, 1, 1 }
walkability[3][2]   = { 1, 1, 1, 1 }
walkability[3][3]   = { 1, 1, 1, 1 }
walkability[3][4]   = { 1, 1, 1, 1 }
walkability[3][5]   = { 1, 1, 1, 1 }
walkability[3][6]   = { 1, 1, 1, 1 }
walkability[3][7]   = { 1, 1, 1, 1 }
walkability[3][8]   = { 1, 1, 1, 1 }
walkability[3][9]   = { 1, 1, 1, 1 }
walkability[3][10]  = { 1, 1, 1, 1 }
walkability[3][11]  = { 1, 1, 1, 1 }
walkability[3][12]  = { 1, 1, 1, 1 }
walkability[3][13]  = { 1, 1, 1, 1 }
walkability[3][14]  = { 1, 1, 1, 1 }
walkability[3][15]  = { 1, 1, 1, 1 }
walkability[4][0]   = { 1, 1, 1, 1 }
walkability[4][1]   = { 1, 1, 1, 1 }
walkability[4][2]   = { 1, 1, 1, 1 }
walkability[4][3]   = { 1, 1, 1, 1 }
walkability[4][4]   = { 1, 1, 1, 1 }
walkability[4][5]   = { 1, 1, 1, 1 }
walkability[4][6]   = { 1, 1, 1, 1 }
walkability[4][7]   = { 1, 1, 1, 1 }
walkability[4][8]   = { 1, 1, 1, 1 }
walkability[4][9]   = { 1, 1, 1, 1 }
walkability[4][10]  = { 1, 1, 1, 1 }
walkability[4][11]  = { 1, 1, 1, 1 }
walkability[4][12]  = { 1, 1, 1, 1 }
walkability[4][13]  = { 1, 1, 1, 1 }
walkability[4][14]  = { 1, 1, 1, 1 }
walkability[4][15]  = { 1, 1, 1, 1 }
walkability[5][0]   = { 1, 1, 1, 1 }
walkability[5][1]   = { 1, 1, 1, 1 }
walkability[5][2]   = { 1, 1, 1, 1 }
walkability[5][3]   = { 1, 1, 1, 1 }
walkability[5][4]   = { 1, 1, 1, 1 }
walkability[5][5]   = { 1, 1, 1, 1 }
walkability[5][6]   = { 1, 1, 1, 1 }
walkability[5][7]   = { 1, 1, 1, 1 }
walkability[5][8]   = { 1, 1, 1, 1 }
walkability[5][9]   = { 1, 1, 1, 1 }
walkability[5][10]  = { 1, 1, 1, 1 }
walkability[5][11]  = { 0, 0, 0, 0 }
walkability[5][12]  = { 0, 0, 0, 0 }
walkability[5][13]  = { 0, 0, 0, 0 }
walkability[5][14]  = { 0, 0, 0, 0 }
walkability[5][15]  = { 0, 0, 0, 0 }
walkability[6][0]   = { 1, 1, 1, 1 }
walkability[6][1]   = { 1, 1, 1, 1 }
walkability[6][2]   = { 1, 1, 1, 1 }
walkability[6][3]   = { 1, 1, 1, 1 }
walkability[6][4]   = { 1, 1, 1, 1 }
walkability[6][5]   = { 1, 1, 1, 1 }
walkability[6][6]   = { 1, 1, 1, 1 }
walkability[6][7]   = { 1, 1, 1, 1 }
walkability[6][8]   = { 1, 1, 1, 1 }
walkability[6][9]   = { 1, 1, 1, 1 }
walkability[6][10]  = { 0, 0, 0, 0 }
walkability[6][11]  = { 0, 0, 0, 0 }
walkability[6][12]  = { 0, 0, 0, 0 }
walkability[6][13]  = { 0, 0, 0, 0 }
walkability[6][14]  = { 0, 0, 0, 0 }
walkability[6][15]  = { 0, 0, 0, 0 }
walkability[7][0]   = { 1, 1, 1, 1 }
walkability[7][1]   = { 1, 1, 1, 1 }
walkability[7][2]   = { 1, 1, 1, 1 }
walkability[7][3]   = { 1, 1, 1, 1 }
walkability[7][4]   = { 1, 1, 1, 1 }
walkability[7][5]   = { 1, 1, 1, 1 }
walkability[7][6]   = { 1, 1, 1, 1 }
walkability[7][7]   = { 1, 1, 1, 1 }
walkability[7][8]   = { 1, 1, 1, 1 }
walkability[7][9]   = { 1, 1, 1, 1 }
walkability[7][10]  = { 0, 0, 0, 0 }
walkability[7][11]  = { 0, 0, 0, 0 }
walkability[7][12]  = { 0, 0, 0, 0 }
walkability[7][13]  = { 0, 0, 0, 0 }
walkability[7][14]  = { 0, 0, 0, 0 }
walkability[7][15]  = { 0, 0, 0, 0 }
walkability[8][0]   = { 0, 0, 0, 0 }
walkability[8][1]   = { 0, 0, 0, 0 }
walkability[8][2]   = { 0, 0, 0, 0 }
walkability[8][3]   = { 0, 0, 0, 0 }
walkability[8][4]   = { 0, 0, 0, 0 }
walkability[8][5]   = { 0, 0, 0, 0 }
walkability[8][6]   = { 0, 0, 0, 0 }
walkability[8][7]   = { 0, 0, 0, 0 }
walkability[8][8]   = { 0, 0, 0, 0 }
walkability[8][9]   = { 0, 0, 0, 0 }
walkability[8][10]  = { 0, 0, 0, 0 }
walkability[8][11]  = { 0, 0, 0, 0 }
walkability[8][12]  = { 0, 0, 0, 0 }
walkability[8][13]  = { 0, 0, 0, 0 }
walkability[8][14]  = { 0, 0, 0, 0 }
walkability[8][15]  = { 0, 0, 0, 0 }
walkability[9][0]   = { 0, 0, 0, 0 }
walkability[9][1]   = { 0, 0, 0, 0 }
walkability[9][2]   = { 0, 0, 0, 0 }
walkability[9][3]   = { 0, 0, 0, 0 }
walkability[9][4]   = { 0, 0, 0, 0 }
walkability[9][5]   = { 0, 0, 0, 0 }
walkability[9][6]   = { 0, 0, 0, 0 }
walkability[9][7]   = { 0, 0, 0, 0 }
walkability[9][8]   = { 0, 0, 0, 0 }
walkability[9][9]   = { 0, 0, 0, 0 }
walkability[9][10]  = { 0, 0, 0, 0 }
walkability[9][11]  = { 0, 0, 0, 0 }
walkability[9][12]  = { 0, 0, 0, 0 }
walkability[9][13]  = { 0, 0, 0, 0 }
walkability[9][14]  = { 0, 0, 0, 0 }
walkability[9][15]  = { 0, 0, 0, 0 }
walkability[10][0]  = { 0, 0, 0, 0 }
walkability[10][1]  = { 0, 0, 0, 0 }
walkability[10][2]  = { 0, 0, 0, 0 }
walkability[10][3]  = { 0, 0, 0, 0 }
walkability[10][4]  = { 0, 0, 0, 0 }
walkability[10][5]  = { 0, 0, 0, 0 }
walkability[10][6]  = { 0, 0, 0, 0 }
walkability[10][7]  = { 0, 0, 0, 0 }
walkability[10][8]  = { 0, 0, 0, 0 }
walkability[10][9]  = { 0, 0, 0, 0 }
walkability[10][10] = { 0, 0, 0, 0 }
walkability[10][11] = { 0, 0, 0, 0 }
walkability[10][12] = { 0, 0, 0, 0 }
walkability[10][13] = { 0, 0, 0, 0 }
walkability[10][14] = { 0, 0, 0, 0 }
walkability[10][15] = { 0, 0, 0, 0 }
walkability[11][0]  = { 0, 0, 0, 0 }
walkability[11][1]  = { 0, 0, 0, 0 }
walkability[11][2]  = { 0, 0, 0, 0 }
walkability[11][3]  = { 0, 0, 0, 0 }
walkability[11][4]  = { 0, 0, 0, 0 }
walkability[11][5]  = { 0, 0, 0, 0 }
walkability[11][6]  = { 0, 0, 0, 0 }
walkability[11][7]  = { 0, 0, 0, 0 }
walkability[11][8]  = { 0, 0, 0, 0 }
walkability[11][9]  = { 0, 0, 0, 0 }
walkability[11][10] = { 0, 0, 0, 0 }
walkability[11][11] = { 0, 0, 0, 0 }
walkability[11][12] = { 0, 0, 0, 0 }
walkability[11][13] = { 0, 0, 0, 0 }
walkability[11][14] = { 0, 0, 0, 0 }
walkability[11][15] = { 0, 0, 0, 0 }
walkability[12][0]  = { 0, 0, 0, 0 }
walkability[12][1]  = { 0, 0, 0, 0 }
walkability[12][2]  = { 0, 0, 0, 0 }
walkability[12][3]  = { 0, 0, 0, 0 }
walkability[12][4]  = { 0, 0, 0, 0 }
walkability[12][5]  = { 0, 0, 0, 0 }
walkability[12][6]  = { 0, 0, 0, 0 }
walkability[12][7]  = { 0, 0, 0, 0 }
walkability[12][8]  = { 0, 0, 0, 0 }
walkability[12][9]  = { 0, 0, 0, 0 }
walkability[12][10] = { 0, 0, 0, 0 }
walkability[12][11] = { 0, 0, 0, 0 }
walkability[12][12] = { 0, 0, 0, 0 }
walkability[12][13] = { 0, 0, 0, 0 }
walkability[12][14] = { 0, 0, 0, 0 }
walkability[12][15] = { 0, 0, 0, 0 }
walkability[13][0]  = { 0, 0, 0, 0 }
walkability[13][1]  = { 0, 0, 0, 0 }
walkability[13][2]  = { 0, 0, 0, 0 }
walkability[13][3]  = { 0, 0, 0, 0 }
walkability[13][4]  = { 0, 0, 0, 0 }
walkability[13][5]  = { 0, 0, 0, 0 }
walkability[13][6]  = { 0, 0, 0, 0 }
walkability[13][7]  = { 0, 0, 0, 0 }
walkability[13][8]  = { 0, 0, 0, 0 }
walkability[13][9]  = { 0, 0, 0, 0 }
walkability[13][10] = { 0, 0, 0, 0 }
walkability[13][11] = { 0, 0, 0, 0 }
walkability[13][12] = { 0, 0, 0, 0 }
walkability[13][13] = { 0, 0, 0, 0 }
walkability[13][14] = { 0, 0, 0, 0 }
walkability[13][15] = { 0, 0, 0, 0 }
walkability[14][0]  = { 0, 0, 0, 0 }
walkability[14][1]  = { 0, 0, 0, 0 }
walkability[14][2]  = { 0, 0, 0, 0 }
walkability[14][3]  = { 0, 0, 0, 0 }
walkability[14][4]  = { 0, 0, 0, 0 }
walkability[14][5]  = { 0, 0, 0, 0 }
walkability[14][6]  = { 0, 0, 0, 0 }
walkability[14][7]  = { 0, 0, 0, 0 }
walkability[14][8]  = { 0, 0, 0, 0 }
walkability[14][9]  = { 0, 0, 0, 0 }
walkability[14][10] = { 0, 0, 0, 0 }
walkability[14][11] = { 0, 0, 0, 0 }
walkability[14][12] = { 0, 0, 0, 0 }
walkability[14][13] = { 0, 0, 0, 0 }
walkability[14][14] = { 0, 0, 0, 0 }
walkability[14][15] = { 0, 0, 0, 0 }
walkability[15][0]  = { 0, 0, 0, 0 }
walkability[15][1]  = { 0, 0, 0, 0 }
walkability[15][2]  = { 0, 0, 0, 0 }
walkability[15][3]  = { 0, 0, 0, 0 }
walkability[15][4]  = { 0, 0, 0, 0 }
walkability[15][5]  = { 0, 0, 0, 0 }
walkability[15][6]  = { 0, 0, 0, 0 }
walkability[15][7]  = { 0, 0, 0, 0 }
walkability[15][8]  = { 0, 0, 0, 0 }
walkability[15][9]  = { 0, 0, 0, 0 }
walkability[15][10] = { 0, 0, 0, 0 }
walkability[15][11] = { 0, 0, 0, 0 }
walkability[15][12] = { 0, 0, 0, 0 }
walkability[15][13] = { 0, 0, 0, 0 }
walkability[15][14] = { 0, 0, 0, 0 }
walkability[15][15] = { 0, 0, 0, 0 }


animated_tiles = {}
animated_tiles[1]  = {0, 15, 3, 15, 6, 15, 9, 10, 48, 15, 51, 15, 54, 10, 57, 10, 9, 10, 6, 15}
animated_tiles[2]  = {1, 15, 4, 15, 7, 15, 10, 10, 49, 15, 52, 15, 55, 10, 58, 10, 10, 10, 7, 15}
animated_tiles[3]  = {2, 15, 5, 15, 8, 15, 11, 10, 50, 15, 53, 15, 56, 10, 59, 10, 11, 10, 8, 15}
animated_tiles[4]  = {16, 15, 19, 15, 22, 15, 25, 10, 64, 15, 67, 15, 70, 10, 73, 10, 25, 10, 22, 15}
animated_tiles[5]  = {17, 15, 20, 15, 23, 15, 26, 10, 65, 15, 68, 15, 71, 10, 74, 10, 26, 10, 23, 15}
animated_tiles[6]  = {18, 15, 21, 15, 24, 15, 27, 10, 66, 15, 69, 15, 72, 10, 75, 10, 27, 10, 24, 15}
animated_tiles[7]  = {32, 15, 35, 15, 38, 15, 41, 10, 80, 15, 83, 15, 86, 10, 89, 10, 41, 10, 38, 15}
animated_tiles[8]  = {33, 15, 36, 15, 39, 15, 42, 10, 81, 15, 84, 15, 87, 10, 90, 10, 42, 10, 39, 15}
animated_tiles[9]  = {34, 15, 37, 15, 40, 15, 43, 10, 82, 15, 85, 15, 88, 10, 91, 10, 43, 10, 40, 15}
animated_tiles[10] = {12, 15, 13, 15, 14, 15, 15, 10, 28, 15, 29, 15, 30, 10, 31, 10, 15, 10, 14, 15}
animated_tiles[11] = {44, 15, 45, 15, 46, 15, 47, 10, 60, 15, 61, 15, 62, 10, 63, 10, 47, 10, 46, 15}
animated_tiles[12] = {96, 15, 97, 15, 98, 15, 99, 10, 100, 15, 101, 15, 102, 10, 103, 10, 104, 100, 105, 15}
animated_tiles[13] = {112, 15, 113, 15, 114, 15, 115, 10, 116, 15, 117, 15, 118, 10, 119, 10, 120, 10, 121, 15}

