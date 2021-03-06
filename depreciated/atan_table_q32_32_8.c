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
// atan_table_q32_32_8.c
//

#include "fpm.h"

const q32_t atan_table_q32_32_8[32][8] = {
    {0x10000000, 0x168dfd70, 0x1b0263d1, 0x1d777715, 0x1eba795e, 0x1f5d140e, 0x1fae84f0, 0x1fd741d4},
    {0x10387a71, 0x16bae1af, 0x1b1cb798, 0x1d85393e, 0x1ec16e91, 0x1f609135, 0x1fb043d5, 0x1fd82151},
    {0x1070ee1b, 0x16e72e30, 0x1b368b1c, 0x1d92b222, 0x1ec83de2, 0x1f63fb42, 0x1fb1f928, 0x1fd8fc05},
    {0x10a9543a, 0x1712e1b8, 0x1b4fe02b, 0x1d9fe330, 0x1ecee81c, 0x1f67529c, 0x1fb3a51e, 0x1fd9d208},
    {0x10e1a611, 0x173dfb3f, 0x1b68b897, 0x1daccdd2, 0x1ed56e05, 0x1f6a97ab, 0x1fb547e8, 0x1fdaa376},
    {0x1119dcf2, 0x176879eb, 0x1b811633, 0x1db9736a, 0x1edbd061, 0x1f6dcad1, 0x1fb6e1ba, 0x1fdb7067},
    {0x1151f23d, 0x17925d0e, 0x1b98fad6, 0x1dc5d557, 0x1ee20fef, 0x1f70ec70, 0x1fb872c5, 0x1fdc38f4},
    {0x1189df66, 0x17bba428, 0x1bb06858, 0x1dd1f4f1, 0x1ee82d6c, 0x1f73fce9, 0x1fb9fb39, 0x1fdcfd35},
    {0x11c19dfa, 0x17e44ee1, 0x1bc76091, 0x1dddd38b, 0x1eee298d, 0x1f76fc99, 0x1fbb7b45, 0x1fddbd41},
    {0x11f9279f, 0x180c5d0d, 0x1bdde559, 0x1de97270, 0x1ef40508, 0x1f79ebdb, 0x1fbcf317, 0x1fde7930},
    {0x1230761c, 0x1833cea3, 0x1bf3f88a, 0x1df4d2e9, 0x1ef9c08c, 0x1f7ccb0a, 0x1fbe62dc, 0x1fdf3118},
    {0x12678358, 0x185aa3c1, 0x1c099bfa, 0x1dfff636, 0x1eff5cc6, 0x1f7f9a7c, 0x1fbfcac0, 0x1fdfe510},
    {0x129e4961, 0x1880dca9, 0x1c1ed17f, 0x1e0add94, 0x1f04da60, 0x1f825a89, 0x1fc12aef, 0x1fe0952c},
    {0x12d4c26a, 0x18a679bc, 0x1c339aed, 0x1e158a39, 0x1f0a39fe, 0x1f850b84, 0x1fc28392, 0x1fe14182},
    {0x130ae8d4, 0x18cb7b7e, 0x1c47fa18, 0x1e1ffd55, 0x1f0f7c43, 0x1f87adc0, 0x1fc3d4d3, 0x1fe1ea27},
    {0x1340b72a, 0x18efe28f, 0x1c5bf0cd, 0x1e2a3815, 0x1f14a1cf, 0x1f8a418e, 0x1fc51edb, 0x1fe28f2f},
    {0x13762829, 0x1913afac, 0x1c6f80d9, 0x1e343b9d, 0x1f19ab3c, 0x1f8cc73b, 0x1fc661d0, 0x1fe330ae},
    {0x13ab36be, 0x1936e3ae, 0x1c82ac06, 0x1e3e090e, 0x1f1e9923, 0x1f8f3f16, 0x1fc79ddb, 0x1fe3ceb7},
    {0x13dfde09, 0x19597f85, 0x1c957419, 0x1e47a184, 0x1f236c1a, 0x1f91a96b, 0x1fc8d320, 0x1fe4695d},
    {0x14141960, 0x197b843c, 0x1ca7dad4, 0x1e510615, 0x1f2824b2, 0x1f940682, 0x1fca01c5, 0x1fe500b3},
    {0x1447e44d, 0x199cf2f0, 0x1cb9e1f5, 0x1e5a37d1, 0x1f2cc37b, 0x1f9656a5, 0x1fcb29ef, 0x1fe594ca},
    {0x147b3a93, 0x19bdccd6, 0x1ccb8b34, 0x1e6337c4, 0x1f314900, 0x1f989a1a, 0x1fcc4bc0, 0x1fe625b6},
    {0x14ae182d, 0x19de1333, 0x1cdcd848, 0x1e6c06f6, 0x1f35b5cc, 0x1f9ad127, 0x1fcd675b, 0x1fe6b386},
    {0x14e0794e, 0x19fdc760, 0x1cedcadf, 0x1e74a667, 0x1f3a0a64, 0x1f9cfc10, 0x1fce7ce3, 0x1fe73e4d},
    {0x15125a62, 0x1a1ceac6, 0x1cfe64a5, 0x1e7d1714, 0x1f3e474c, 0x1f9f1b17, 0x1fcf8c79, 0x1fe7c61a},
    {0x1543b80f, 0x1a3b7edb, 0x1d0ea741, 0x1e8559f6, 0x1f426d05, 0x1fa12e7d, 0x1fd0963d, 0x1fe84afe},
    {0x15748f33, 0x1a598526, 0x1d1e9453, 0x1e8d6fff, 0x1f467c0e, 0x1fa33683, 0x1fd19a50, 0x1fe8cd0a},
    {0x15a4dce6, 0x1a76ff36, 0x1d2e2d78, 0x1e955a1e, 0x1f4a74e0, 0x1fa53365, 0x1fd298d0, 0x1fe94c4c},
    {0x15d49e79, 0x1a93eea8, 0x1d3d7446, 0x1e9d193d, 0x1f4e57f7, 0x1fa72561, 0x1fd391dd, 0x1fe9c8d4},
    {0x1603d175, 0x1ab05524, 0x1d4c6a4d, 0x1ea4ae42, 0x1f5225c7, 0x1fa90cb4, 0x1fd48593, 0x1fea42b0},
    {0x1632739a, 0x1acc3459, 0x1d5b111a, 0x1eac1a0b, 0x1f55dec5, 0x1faae996, 0x1fd57411, 0x1feab9f1},
    {0x166082df, 0x1ae78dfe, 0x1d696a32, 0x1eb35d77, 0x1f598363, 0x1facbc42, 0x1fd65d73, 0x1feb2ea3}
};


