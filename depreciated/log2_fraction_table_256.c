/*
 * Copyright (c) 2008, 2009, 2010 Joseph Gaeddert
 * Copyright (c) 2008, 2009, 2010 Virginia Polytechnic
 *                                Institute & State University
 *
 * This file is part of liquid.
 *
 * liquid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * liquid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with liquid.  If not, see <http://www.gnu.org/licenses/>.
 */

// 
// log2_fraction_table_256.c
//

const unsigned int log2_fraction_table_256[256] = {
    0, 1, 3, 4, 6, 7, 9, 10, 11, 13, 14, 16, 17, 18, 20, 21, 
    22, 24, 25, 26, 28, 29, 30, 32, 33, 34, 36, 37, 38, 40, 41, 42, 
    44, 45, 46, 47, 49, 50, 51, 52, 54, 55, 56, 57, 59, 60, 61, 62, 
    63, 65, 66, 67, 68, 69, 71, 72, 73, 74, 75, 77, 78, 79, 80, 81, 
    82, 84, 85, 86, 87, 88, 89, 90, 92, 93, 94, 95, 96, 97, 98, 99, 
    100, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 116, 117, 
    118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 
    134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 
    150, 151, 152, 153, 154, 155, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 
    165, 166, 167, 168, 169, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 178, 
    179, 180, 181, 182, 183, 184, 185, 185, 186, 187, 188, 189, 190, 191, 192, 192, 
    193, 194, 195, 196, 197, 198, 198, 199, 200, 201, 202, 203, 203, 204, 205, 206, 
    207, 208, 208, 209, 210, 211, 212, 212, 213, 214, 215, 216, 216, 217, 218, 219, 
    220, 220, 221, 222, 223, 224, 224, 225, 226, 227, 228, 228, 229, 230, 231, 231, 
    232, 233, 234, 234, 235, 236, 237, 238, 238, 239, 240, 241, 241, 242, 243, 244, 
    244, 245, 246, 247, 247, 248, 249, 249, 250, 251, 252, 252, 253, 254, 255, 255
};

