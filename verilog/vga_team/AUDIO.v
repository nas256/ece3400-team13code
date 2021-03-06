`define ONE_SEC 25000000

module AUDIO(
  CLOCK_50,
  ENABLE,
  GPIO_1_D
);
   
	input  CLOCK_50;
	input  ENABLE;
	
	output [7:0] GPIO_1_D;

   reg [7:0] sin_table [0:1023];
    
	integer i;
	integer tone_counter;
	integer tone_duration;
	integer freq1;
	integer freq2;
	integer freq3;
   
	reg [7:0] data_out; 
	 
	assign GPIO_1_D[7:0] = data_out;
	 
    // Initialize the sine rom with samples. 
    initial begin
	   sin_table[0] = 40;
	   sin_table[1] = 40;
	   sin_table[2] = 40;
		sin_table[3] = 41;
		sin_table[4] = 41;
		sin_table[5] = 41;
		sin_table[6] = 41;
		sin_table[7] = 42;
		sin_table[8] = 42;
		sin_table[9] = 42;
		sin_table[10] = 42;
		sin_table[11] = 43;
		sin_table[12] = 43;
		sin_table[13] = 43;
		sin_table[14] = 43;
		sin_table[15] = 44;
		sin_table[16] = 44;
		sin_table[17] = 44;
		sin_table[18] = 44;
		sin_table[19] = 45;
		sin_table[20] = 45;
		sin_table[21] = 45;
		sin_table[22] = 45;
		sin_table[23] = 45;
		sin_table[24] = 46;
		sin_table[25] = 46;
		sin_table[26] = 46;
		sin_table[27] = 46;
		sin_table[28] = 47;
		sin_table[29] = 47;
		sin_table[30] = 47;
		sin_table[31] = 47;
		sin_table[32] = 48;
		sin_table[33] = 48;
		sin_table[34] = 48;
		sin_table[35] = 48;
		sin_table[36] = 49;
		sin_table[37] = 49;
		sin_table[38] = 49;
		sin_table[39] = 49;
		sin_table[40] = 49;
		sin_table[41] = 50;
		sin_table[42] = 50;
		sin_table[43] = 50;
		sin_table[44] = 50;
		sin_table[45] = 51;
		sin_table[46] = 51;
		sin_table[47] = 51;
		sin_table[48] = 51;
		sin_table[49] = 52;
		sin_table[50] = 52;
		sin_table[51] = 52;
		sin_table[52] = 52;
		sin_table[53] = 52;
		sin_table[54] = 53;
		sin_table[55] = 53;
		sin_table[56] = 53;
		sin_table[57] = 53;
		sin_table[58] = 54;
		sin_table[59] = 54;
		sin_table[60] = 54;
		sin_table[61] = 54;
		sin_table[62] = 55;
		sin_table[63] = 55;
		sin_table[64] = 55;
		sin_table[65] = 55;
		sin_table[66] = 55;
		sin_table[67] = 56;
		sin_table[68] = 56;
		sin_table[69] = 56;
		sin_table[70] = 56;
		sin_table[71] = 56;
		sin_table[72] = 57;
		sin_table[73] = 57;
		sin_table[74] = 57;
		sin_table[75] = 57;
		sin_table[76] = 58;
		sin_table[77] = 58;
		sin_table[78] = 58;
		sin_table[79] = 58;
		sin_table[80] = 58;
		sin_table[81] = 59;
		sin_table[82] = 59;
		sin_table[83] = 59;
		sin_table[84] = 59;
		sin_table[85] = 59;
		sin_table[86] = 60;
		sin_table[87] = 60;
		sin_table[88] = 60;
		sin_table[89] = 60;
		sin_table[90] = 60;
		sin_table[91] = 61;
		sin_table[92] = 61;
		sin_table[93] = 61;
		sin_table[94] = 61;
		sin_table[95] = 62;
		sin_table[96] = 62;
		sin_table[97] = 62;
		sin_table[98] = 62;
		sin_table[99] = 62;
		sin_table[100] = 62;
		sin_table[101] = 63;
		sin_table[102] = 63;
		sin_table[103] = 63;
		sin_table[104] = 63;
		sin_table[105] = 63;
		sin_table[106] = 64;
		sin_table[107] = 64;
		sin_table[108] = 64;
		sin_table[109] = 64;
		sin_table[110] = 64;
		sin_table[111] = 65;
		sin_table[112] = 65;
		sin_table[113] = 65;
		sin_table[114] = 65;
		sin_table[115] = 65;
		sin_table[116] = 66;
		sin_table[117] = 66;
		sin_table[118] = 66;
		sin_table[119] = 66;
		sin_table[120] = 66;
		sin_table[121] = 66;
		sin_table[122] = 67;
		sin_table[123] = 67;
		sin_table[124] = 67;
		sin_table[125] = 67;
		sin_table[126] = 67;
		sin_table[127] = 67;
		sin_table[128] = 68;
		sin_table[129] = 68;
		sin_table[130] = 68;
		sin_table[131] = 68;
		sin_table[132] = 68;
		sin_table[133] = 68;
		sin_table[134] = 69;
		sin_table[135] = 69;
		sin_table[136] = 69;
		sin_table[137] = 69;
		sin_table[138] = 69;
		sin_table[139] = 69;
		sin_table[140] = 70;
		sin_table[141] = 70;
		sin_table[142] = 70;
		sin_table[143] = 70;
		sin_table[144] = 70;
		sin_table[145] = 70;
		sin_table[146] = 70;
		sin_table[147] = 71;
		sin_table[148] = 71;
		sin_table[149] = 71;
		sin_table[150] = 71;
		sin_table[151] = 71;
		sin_table[152] = 71;
		sin_table[153] = 72;
		sin_table[154] = 72;
		sin_table[155] = 72;
		sin_table[156] = 72;
		sin_table[157] = 72;
		sin_table[158] = 72;
		sin_table[159] = 72;
		sin_table[160] = 72;
		sin_table[161] = 73;
		sin_table[162] = 73;
		sin_table[163] = 73;
		sin_table[164] = 73;
		sin_table[165] = 73;
		sin_table[166] = 73;
		sin_table[167] = 73;
		sin_table[168] = 74;
		sin_table[169] = 74;
		sin_table[170] = 74;
		sin_table[171] = 74;
		sin_table[172] = 74;
		sin_table[173] = 74;
		sin_table[174] = 74;
		sin_table[175] = 74;
		sin_table[176] = 74;
		sin_table[177] = 75;
		sin_table[178] = 75;
		sin_table[179] = 75;
		sin_table[180] = 75;
		sin_table[181] = 75;
		sin_table[182] = 75;
		sin_table[183] = 75;
		sin_table[184] = 75;
		sin_table[185] = 75;
		sin_table[186] = 76;
		sin_table[187] = 76;
		sin_table[188] = 76;
		sin_table[189] = 76;
		sin_table[190] = 76;
		sin_table[191] = 76;
		sin_table[192] = 76;
		sin_table[193] = 76;
		sin_table[194] = 76;
		sin_table[195] = 76;
		sin_table[196] = 76;
		sin_table[197] = 77;
		sin_table[198] = 77;
		sin_table[199] = 77;
		sin_table[200] = 77;
		sin_table[201] = 77;
		sin_table[202] = 77;
		sin_table[203] = 77;
		sin_table[204] = 77;
		sin_table[205] = 77;
		sin_table[206] = 77;
		sin_table[207] = 77;
		sin_table[208] = 77;
		sin_table[209] = 77;
		sin_table[210] = 78;
		sin_table[211] = 78;
		sin_table[212] = 78;
		sin_table[213] = 78;
		sin_table[214] = 78;
		sin_table[215] = 78;
		sin_table[216] = 78;
		sin_table[217] = 78;
		sin_table[218] = 78;
		sin_table[219] = 78;
		sin_table[220] = 78;
		sin_table[221] = 78;
		sin_table[222] = 78;
		sin_table[223] = 78;
		sin_table[224] = 78;
		sin_table[225] = 78;
		sin_table[226] = 78;
		sin_table[227] = 78;
		sin_table[228] = 78;
		sin_table[229] = 79;
		sin_table[230] = 79;
		sin_table[231] = 79;
		sin_table[232] = 79;
		sin_table[233] = 79;
		sin_table[234] = 79;
		sin_table[235] = 79;
		sin_table[236] = 79;
		sin_table[237] = 79;
		sin_table[238] = 79;
		sin_table[239] = 79;
		sin_table[240] = 79;
		sin_table[241] = 79;
		sin_table[242] = 79;
		sin_table[243] = 79;
		sin_table[244] = 79;
		sin_table[245] = 79;
		sin_table[246] = 79;
		sin_table[247] = 79;
		sin_table[248] = 79;
		sin_table[249] = 79;
		sin_table[250] = 79;
		sin_table[251] = 79;
		sin_table[252] = 79;
		sin_table[253] = 79;
		sin_table[254] = 79;
		sin_table[255] = 79;
		sin_table[256] = 79;
		sin_table[257] = 79;
		sin_table[258] = 79;
		sin_table[259] = 79;
		sin_table[260] = 79;
		sin_table[261] = 79;
		sin_table[262] = 79;
		sin_table[263] = 79;
		sin_table[264] = 79;
		sin_table[265] = 79;
		sin_table[266] = 79;
		sin_table[267] = 79;
		sin_table[268] = 79;
		sin_table[269] = 79;
		sin_table[270] = 79;
		sin_table[271] = 79;
		sin_table[272] = 79;
		sin_table[273] = 79;
		sin_table[274] = 79;
		sin_table[275] = 79;
		sin_table[276] = 79;
		sin_table[277] = 79;
		sin_table[278] = 79;
		sin_table[279] = 79;
		sin_table[280] = 79;
		sin_table[281] = 79;
		sin_table[282] = 79;
		sin_table[283] = 79;
		sin_table[284] = 78;
		sin_table[285] = 78;
		sin_table[286] = 78;
		sin_table[287] = 78;
		sin_table[288] = 78;
		sin_table[289] = 78;
		sin_table[290] = 78;
		sin_table[291] = 78;
		sin_table[292] = 78;
		sin_table[293] = 78;
		sin_table[294] = 78;
		sin_table[295] = 78;
		sin_table[296] = 78;
		sin_table[297] = 78;
		sin_table[298] = 78;
		sin_table[299] = 78;
		sin_table[300] = 78;
		sin_table[301] = 78;
		sin_table[302] = 78;
		sin_table[303] = 77;
		sin_table[304] = 77;
		sin_table[305] = 77;
		sin_table[306] = 77;
		sin_table[307] = 77;
		sin_table[308] = 77;
		sin_table[309] = 77;
		sin_table[310] = 77;
		sin_table[311] = 77;
		sin_table[312] = 77;
		sin_table[313] = 77;
		sin_table[314] = 77;
		sin_table[315] = 77;
		sin_table[316] = 76;
		sin_table[317] = 76;
		sin_table[318] = 76;
		sin_table[319] = 76;
		sin_table[320] = 76;
		sin_table[321] = 76;
		sin_table[322] = 76;
		sin_table[323] = 76;
		sin_table[324] = 76;
		sin_table[325] = 76;
		sin_table[326] = 76;
		sin_table[327] = 75;
		sin_table[328] = 75;
		sin_table[329] = 75;
		sin_table[330] = 75;
		sin_table[331] = 75;
		sin_table[332] = 75;
		sin_table[333] = 75;
		sin_table[334] = 75;
		sin_table[335] = 75;
		sin_table[336] = 74;
		sin_table[337] = 74;
		sin_table[338] = 74;
		sin_table[339] = 74;
		sin_table[340] = 74;
		sin_table[341] = 74;
		sin_table[342] = 74;
		sin_table[343] = 74;
		sin_table[344] = 74;
		sin_table[345] = 73;
		sin_table[346] = 73;
		sin_table[347] = 73;
		sin_table[348] = 73;
		sin_table[349] = 73;
		sin_table[350] = 73;
		sin_table[351] = 73;
		sin_table[352] = 72;
		sin_table[353] = 72;
		sin_table[354] = 72;
		sin_table[355] = 72;
		sin_table[356] = 72;
		sin_table[357] = 72;
		sin_table[358] = 72;
		sin_table[359] = 72;
		sin_table[360] = 71;
		sin_table[361] = 71;
		sin_table[362] = 71;
		sin_table[363] = 71;
		sin_table[364] = 71;
		sin_table[365] = 71;
		sin_table[366] = 70;
		sin_table[367] = 70;
		sin_table[368] = 70;
		sin_table[369] = 70;
		sin_table[370] = 70;
		sin_table[371] = 70;
		sin_table[372] = 70;
		sin_table[373] = 69;
		sin_table[374] = 69;
		sin_table[375] = 69;
		sin_table[376] = 69;
		sin_table[377] = 69;
		sin_table[378] = 69;
		sin_table[379] = 68;
		sin_table[380] = 68;
		sin_table[381] = 68;
		sin_table[382] = 68;
		sin_table[383] = 68;
		sin_table[384] = 68;
		sin_table[385] = 67;
		sin_table[386] = 67;
		sin_table[387] = 67;
		sin_table[388] = 67;
		sin_table[389] = 67;
		sin_table[390] = 67;
		sin_table[391] = 66;
		sin_table[392] = 66;
		sin_table[393] = 66;
		sin_table[394] = 66;
		sin_table[395] = 66;
		sin_table[396] = 66;
		sin_table[397] = 65;
		sin_table[398] = 65;
		sin_table[399] = 65;
		sin_table[400] = 65;
		sin_table[401] = 65;
		sin_table[402] = 64;
		sin_table[403] = 64;
		sin_table[404] = 64;
		sin_table[405] = 64;
		sin_table[406] = 64;
		sin_table[407] = 63;
		sin_table[408] = 63;
		sin_table[409] = 63;
		sin_table[410] = 63;
		sin_table[411] = 63;
		sin_table[412] = 62;
		sin_table[413] = 62;
		sin_table[414] = 62;
		sin_table[415] = 62;
		sin_table[416] = 62;
		sin_table[417] = 62;
		sin_table[418] = 61;
		sin_table[419] = 61;
		sin_table[420] = 61;
		sin_table[421] = 61;
		sin_table[422] = 60;
		sin_table[423] = 60;
		sin_table[424] = 60;
		sin_table[425] = 60;
		sin_table[426] = 60;
		sin_table[427] = 59;
		sin_table[428] = 59;
		sin_table[429] = 59;
		sin_table[430] = 59;
		sin_table[431] = 59;
		sin_table[432] = 58;
		sin_table[433] = 58;
		sin_table[434] = 58;
		sin_table[435] = 58;
		sin_table[436] = 58;
		sin_table[437] = 57;
		sin_table[438] = 57;
		sin_table[439] = 57;
		sin_table[440] = 57;
		sin_table[441] = 56;
		sin_table[442] = 56;
		sin_table[443] = 56;
		sin_table[444] = 56;
		sin_table[445] = 56;
		sin_table[446] = 55;
		sin_table[447] = 55;
		sin_table[448] = 55;
		sin_table[449] = 55;
		sin_table[450] = 55;
		sin_table[451] = 54;
		sin_table[452] = 54;
		sin_table[453] = 54;
		sin_table[454] = 54;
		sin_table[455] = 53;
		sin_table[456] = 53;
		sin_table[457] = 53;
		sin_table[458] = 53;
		sin_table[459] = 52;
		sin_table[460] = 52;
		sin_table[461] = 52;
		sin_table[462] = 52;
		sin_table[463] = 52;
		sin_table[464] = 51;
		sin_table[465] = 51;
		sin_table[466] = 51;
		sin_table[467] = 51;
		sin_table[468] = 50;
		sin_table[469] = 50;
		sin_table[470] = 50;
		sin_table[471] = 50;
		sin_table[472] = 49;
		sin_table[473] = 49;
		sin_table[474] = 49;
		sin_table[475] = 49;
		sin_table[476] = 49;
		sin_table[477] = 48;
		sin_table[478] = 48;
		sin_table[479] = 48;
		sin_table[480] = 48;
		sin_table[481] = 47;
		sin_table[482] = 47;
		sin_table[483] = 47;
		sin_table[484] = 47;
		sin_table[485] = 46;
		sin_table[486] = 46;
		sin_table[487] = 46;
		sin_table[488] = 46;
		sin_table[489] = 45;
		sin_table[490] = 45;
		sin_table[491] = 45;
		sin_table[492] = 45;
		sin_table[493] = 45;
		sin_table[494] = 44;
		sin_table[495] = 44;
		sin_table[496] = 44;
		sin_table[497] = 44;
		sin_table[498] = 43;
		sin_table[499] = 43;
		sin_table[500] = 43;
		sin_table[501] = 43;
		sin_table[502] = 42;
		sin_table[503] = 42;
		sin_table[504] = 42;
		sin_table[505] = 42;
		sin_table[506] = 41;
		sin_table[507] = 41;
		sin_table[508] = 41;
		sin_table[509] = 41;
		sin_table[510] = 40;
		sin_table[511] = 40;
		sin_table[512] = 40;
		sin_table[513] = 40;
		sin_table[514] = 40;
		sin_table[515] = 39;
		sin_table[516] = 39;
		sin_table[517] = 39;
		sin_table[518] = 39;
		sin_table[519] = 38;
		sin_table[520] = 38;
		sin_table[521] = 38;
		sin_table[522] = 38;
		sin_table[523] = 37;
		sin_table[524] = 37;
		sin_table[525] = 37;
		sin_table[526] = 37;
		sin_table[527] = 36;
		sin_table[528] = 36;
		sin_table[529] = 36;
		sin_table[530] = 36;
		sin_table[531] = 35;
		sin_table[532] = 35;
		sin_table[533] = 35;
		sin_table[534] = 35;
		sin_table[535] = 35;
		sin_table[536] = 34;
		sin_table[537] = 34;
		sin_table[538] = 34;
		sin_table[539] = 34;
		sin_table[540] = 33;
		sin_table[541] = 33;
		sin_table[542] = 33;
		sin_table[543] = 33;
		sin_table[544] = 32;
		sin_table[545] = 32;
		sin_table[546] = 32;
		sin_table[547] = 32;
		sin_table[548] = 31;
		sin_table[549] = 31;
		sin_table[550] = 31;
		sin_table[551] = 31;
		sin_table[552] = 31;
		sin_table[553] = 30;
		sin_table[554] = 30;
		sin_table[555] = 30;
		sin_table[556] = 30;
		sin_table[557] = 29;
		sin_table[558] = 29;
		sin_table[559] = 29;
		sin_table[560] = 29;
		sin_table[561] = 28;
		sin_table[562] = 28;
		sin_table[563] = 28;
		sin_table[564] = 28;
		sin_table[565] = 28;
		sin_table[566] = 27;
		sin_table[567] = 27;
		sin_table[568] = 27;
		sin_table[569] = 27;
		sin_table[570] = 26;
		sin_table[571] = 26;
		sin_table[572] = 26;
		sin_table[573] = 26;
		sin_table[574] = 25;
		sin_table[575] = 25;
		sin_table[576] = 25;
		sin_table[577] = 25;
		sin_table[578] = 25;
		sin_table[579] = 24;
		sin_table[580] = 24;
		sin_table[581] = 24;
		sin_table[582] = 24;
		sin_table[583] = 24;
		sin_table[584] = 23;
		sin_table[585] = 23;
		sin_table[586] = 23;
		sin_table[587] = 23;
		sin_table[588] = 22;
		sin_table[589] = 22;
		sin_table[590] = 22;
		sin_table[591] = 22;
		sin_table[592] = 22;
		sin_table[593] = 21;
		sin_table[594] = 21;
		sin_table[595] = 21;
		sin_table[596] = 21;
		sin_table[597] = 21;
		sin_table[598] = 20;
		sin_table[599] = 20;
		sin_table[600] = 20;
		sin_table[601] = 20;
		sin_table[602] = 20;
		sin_table[603] = 19;
		sin_table[604] = 19;
		sin_table[605] = 19;
		sin_table[606] = 19;
		sin_table[607] = 18;
		sin_table[608] = 18;
		sin_table[609] = 18;
		sin_table[610] = 18;
		sin_table[611] = 18;
		sin_table[612] = 18;
		sin_table[613] = 17;
		sin_table[614] = 17;
		sin_table[615] = 17;
		sin_table[616] = 17;
		sin_table[617] = 17;
		sin_table[618] = 16;
		sin_table[619] = 16;
		sin_table[620] = 16;
		sin_table[621] = 16;
		sin_table[622] = 16;
		sin_table[623] = 15;
		sin_table[624] = 15;
		sin_table[625] = 15;
		sin_table[626] = 15;
		sin_table[627] = 15;
		sin_table[628] = 14;
		sin_table[629] = 14;
		sin_table[630] = 14;
		sin_table[631] = 14;
		sin_table[632] = 14;
		sin_table[633] = 14;
		sin_table[634] = 13;
		sin_table[635] = 13;
		sin_table[636] = 13;
		sin_table[637] = 13;
		sin_table[638] = 13;
		sin_table[639] = 13;
		sin_table[640] = 12;
		sin_table[641] = 12;
		sin_table[642] = 12;
		sin_table[643] = 12;
		sin_table[644] = 12;
		sin_table[645] = 12;
		sin_table[646] = 11;
		sin_table[647] = 11;
		sin_table[648] = 11;
		sin_table[649] = 11;
		sin_table[650] = 11;
		sin_table[651] = 11;
		sin_table[652] = 10;
		sin_table[653] = 10;
		sin_table[654] = 10;
		sin_table[655] = 10;
		sin_table[656] = 10;
		sin_table[657] = 10;
		sin_table[658] = 10;
		sin_table[659] = 9;
		sin_table[660] = 9;
		sin_table[661] = 9;
		sin_table[662] = 9;
		sin_table[663] = 9;
		sin_table[664] = 9;
		sin_table[665] = 8;
		sin_table[666] = 8;
		sin_table[667] = 8;
		sin_table[668] = 8;
		sin_table[669] = 8;
		sin_table[670] = 8;
		sin_table[671] = 8;
		sin_table[672] = 8;
		sin_table[673] = 7;
		sin_table[674] = 7;
		sin_table[675] = 7;
		sin_table[676] = 7;
		sin_table[677] = 7;
		sin_table[678] = 7;
		sin_table[679] = 7;
		sin_table[680] = 6;
		sin_table[681] = 6;
		sin_table[682] = 6;
		sin_table[683] = 6;
		sin_table[684] = 6;
		sin_table[685] = 6;
		sin_table[686] = 6;
		sin_table[687] = 6;
		sin_table[688] = 6;
		sin_table[689] = 5;
		sin_table[690] = 5;
		sin_table[691] = 5;
		sin_table[692] = 5;
		sin_table[693] = 5;
		sin_table[694] = 5;
		sin_table[695] = 5;
		sin_table[696] = 5;
		sin_table[697] = 5;
		sin_table[698] = 4;
		sin_table[699] = 4;
		sin_table[700] = 4;
		sin_table[701] = 4;
		sin_table[702] = 4;
		sin_table[703] = 4;
		sin_table[704] = 4;
		sin_table[705] = 4;
		sin_table[706] = 4;
		sin_table[707] = 4;
		sin_table[708] = 4;
		sin_table[709] = 3;
		sin_table[710] = 3;
		sin_table[711] = 3;
		sin_table[712] = 3;
		sin_table[713] = 3;
		sin_table[714] = 3;
		sin_table[715] = 3;
		sin_table[716] = 3;
		sin_table[717] = 3;
		sin_table[718] = 3;
		sin_table[719] = 3;
		sin_table[720] = 3;
		sin_table[721] = 3;
		sin_table[722] = 2;
		sin_table[723] = 2;
		sin_table[724] = 2;
		sin_table[725] = 2;
		sin_table[726] = 2;
		sin_table[727] = 2;
		sin_table[728] = 2;
		sin_table[729] = 2;
		sin_table[730] = 2;
		sin_table[731] = 2;
		sin_table[732] = 2;
		sin_table[733] = 2;
		sin_table[734] = 2;
		sin_table[735] = 2;
		sin_table[736] = 2;
		sin_table[737] = 2;
		sin_table[738] = 2;
		sin_table[739] = 2;
		sin_table[740] = 2;
		sin_table[741] = 1;
		sin_table[742] = 1;
		sin_table[743] = 1;
		sin_table[744] = 1;
		sin_table[745] = 1;
		sin_table[746] = 1;
		sin_table[747] = 1;
		sin_table[748] = 1;
		sin_table[749] = 1;
		sin_table[750] = 1;
		sin_table[751] = 1;
		sin_table[752] = 1;
		sin_table[753] = 1;
		sin_table[754] = 1;
		sin_table[755] = 1;
		sin_table[756] = 1;
		sin_table[757] = 1;
		sin_table[758] = 1;
		sin_table[759] = 1;
		sin_table[760] = 1;
		sin_table[761] = 1;
		sin_table[762] = 1;
		sin_table[763] = 1;
		sin_table[764] = 1;
		sin_table[765] = 1;
		sin_table[766] = 1;
		sin_table[767] = 1;
		sin_table[768] = 1;
		sin_table[769] = 1;
		sin_table[770] = 1;
		sin_table[771] = 1;
		sin_table[772] = 1;
		sin_table[773] = 1;
		sin_table[774] = 1;
		sin_table[775] = 1;
		sin_table[776] = 1;
		sin_table[777] = 1;
		sin_table[778] = 1;
		sin_table[779] = 1;
		sin_table[780] = 1;
		sin_table[781] = 1;
		sin_table[782] = 1;
		sin_table[783] = 1;
		sin_table[784] = 1;
		sin_table[785] = 1;
		sin_table[786] = 1;
		sin_table[787] = 1;
		sin_table[788] = 1;
		sin_table[789] = 1;
		sin_table[790] = 1;
		sin_table[791] = 1;
		sin_table[792] = 1;
		sin_table[793] = 1;
		sin_table[794] = 1;
		sin_table[795] = 1;
		sin_table[796] = 2;
		sin_table[797] = 2;
		sin_table[798] = 2;
		sin_table[799] = 2;
		sin_table[800] = 2;
		sin_table[801] = 2;
		sin_table[802] = 2;
		sin_table[803] = 2;
		sin_table[804] = 2;
		sin_table[805] = 2;
		sin_table[806] = 2;
		sin_table[807] = 2;
		sin_table[808] = 2;
		sin_table[809] = 2;
		sin_table[810] = 2;
		sin_table[811] = 2;
		sin_table[812] = 2;
		sin_table[813] = 2;
		sin_table[814] = 2;
		sin_table[815] = 3;
		sin_table[816] = 3;
		sin_table[817] = 3;
		sin_table[818] = 3;
		sin_table[819] = 3;
		sin_table[820] = 3;
		sin_table[821] = 3;
		sin_table[822] = 3;
		sin_table[823] = 3;
		sin_table[824] = 3;
		sin_table[825] = 3;
		sin_table[826] = 3;
		sin_table[827] = 3;
		sin_table[828] = 4;
		sin_table[829] = 4;
		sin_table[830] = 4;
		sin_table[831] = 4;
		sin_table[832] = 4;
		sin_table[833] = 4;
		sin_table[834] = 4;
		sin_table[835] = 4;
		sin_table[836] = 4;
		sin_table[837] = 4;
		sin_table[838] = 4;
		sin_table[839] = 5;
		sin_table[840] = 5;
		sin_table[841] = 5;
		sin_table[842] = 5;
		sin_table[843] = 5;
		sin_table[844] = 5;
		sin_table[845] = 5;
		sin_table[846] = 5;
		sin_table[847] = 5;
		sin_table[848] = 6;
		sin_table[849] = 6;
		sin_table[850] = 6;
		sin_table[851] = 6;
		sin_table[852] = 6;
		sin_table[853] = 6;
		sin_table[854] = 6;
		sin_table[855] = 6;
		sin_table[856] = 6;
		sin_table[857] = 7;
		sin_table[858] = 7;
		sin_table[859] = 7;
		sin_table[860] = 7;
		sin_table[861] = 7;
		sin_table[862] = 7;
		sin_table[863] = 7;
		sin_table[864] = 8;
		sin_table[865] = 8;
		sin_table[866] = 8;
		sin_table[867] = 8;
		sin_table[868] = 8;
		sin_table[869] = 8;
		sin_table[870] = 8;
		sin_table[871] = 8;
		sin_table[872] = 9;
		sin_table[873] = 9;
		sin_table[874] = 9;
		sin_table[875] = 9;
		sin_table[876] = 9;
		sin_table[877] = 9;
		sin_table[878] = 10;
		sin_table[879] = 10;
		sin_table[880] = 10;
		sin_table[881] = 10;
		sin_table[882] = 10;
		sin_table[883] = 10;
		sin_table[884] = 10;
		sin_table[885] = 11;
		sin_table[886] = 11;
		sin_table[887] = 11;
		sin_table[888] = 11;
		sin_table[889] = 11;
		sin_table[890] = 11;
		sin_table[891] = 12;
		sin_table[892] = 12;
		sin_table[893] = 12;
		sin_table[894] = 12;
		sin_table[895] = 12;
		sin_table[896] = 12;
		sin_table[897] = 13;
		sin_table[898] = 13;
		sin_table[899] = 13;
		sin_table[900] = 13;
		sin_table[901] = 13;
		sin_table[902] = 13;
		sin_table[903] = 14;
		sin_table[904] = 14;
		sin_table[905] = 14;
		sin_table[906] = 14;
		sin_table[907] = 14;
		sin_table[908] = 14;
		sin_table[909] = 15;
		sin_table[910] = 15;
		sin_table[911] = 15;
		sin_table[912] = 15;
		sin_table[913] = 15;
		sin_table[914] = 16;
		sin_table[915] = 16;
		sin_table[916] = 16;
		sin_table[917] = 16;
		sin_table[918] = 16;
		sin_table[919] = 17;
		sin_table[920] = 17;
		sin_table[921] = 17;
		sin_table[922] = 17;
		sin_table[923] = 17;
		sin_table[924] = 18;
		sin_table[925] = 18;
		sin_table[926] = 18;
		sin_table[927] = 18;
		sin_table[928] = 18;
		sin_table[929] = 18;
		sin_table[930] = 19;
		sin_table[931] = 19;
		sin_table[932] = 19;
		sin_table[933] = 19;
		sin_table[934] = 20;
		sin_table[935] = 20;
		sin_table[936] = 20;
		sin_table[937] = 20;
		sin_table[938] = 20;
		sin_table[939] = 21;
		sin_table[940] = 21;
		sin_table[941] = 21;
		sin_table[942] = 21;
		sin_table[943] = 21;
		sin_table[944] = 22;
		sin_table[945] = 22;
		sin_table[946] = 22;
		sin_table[947] = 22;
		sin_table[948] = 22;
		sin_table[949] = 23;
		sin_table[950] = 23;
		sin_table[951] = 23;
		sin_table[952] = 23;
		sin_table[953] = 24;
		sin_table[954] = 24;
		sin_table[955] = 24;
		sin_table[956] = 24;
		sin_table[957] = 24;
		sin_table[958] = 25;
		sin_table[959] = 25;
		sin_table[960] = 25;
		sin_table[961] = 25;
		sin_table[962] = 25;
		sin_table[963] = 26;
		sin_table[964] = 26;
		sin_table[965] = 26;
		sin_table[966] = 26;
		sin_table[967] = 27;
		sin_table[968] = 27;
		sin_table[969] = 27;
		sin_table[970] = 27;
		sin_table[971] = 28;
		sin_table[972] = 28;
		sin_table[973] = 28;
		sin_table[974] = 28;
		sin_table[975] = 28;
		sin_table[976] = 29;
		sin_table[977] = 29;
		sin_table[978] = 29;
		sin_table[979] = 29;
		sin_table[980] = 30;
		sin_table[981] = 30;
		sin_table[982] = 30;
		sin_table[983] = 30;
		sin_table[984] = 31;
		sin_table[985] = 31;
		sin_table[986] = 31;
		sin_table[987] = 31;
		sin_table[988] = 31;
		sin_table[989] = 32;
		sin_table[990] = 32;
		sin_table[991] = 32;
		sin_table[992] = 32;
		sin_table[993] = 33;
		sin_table[994] = 33;
		sin_table[995] = 33;
		sin_table[996] = 33;
		sin_table[997] = 34;
		sin_table[998] = 34;
		sin_table[999] = 34;
		sin_table[1000] = 34;
		sin_table[1001] = 35;
		sin_table[1002] = 35;
		sin_table[1003] = 35;
		sin_table[1004] = 35;
		sin_table[1005] = 35;
		sin_table[1006] = 36;
		sin_table[1007] = 36;
		sin_table[1008] = 36;
		sin_table[1009] = 36;
		sin_table[1010] = 37;
		sin_table[1011] = 37;
		sin_table[1012] = 37;
		sin_table[1013] = 37;
		sin_table[1014] = 38;
		sin_table[1015] = 38;
		sin_table[1016] = 38;
		sin_table[1017] = 38;
		sin_table[1018] = 39;
		sin_table[1019] = 39;
		sin_table[1020] = 39;
		sin_table[1021] = 39;
		sin_table[1022] = 40;
		sin_table[1023] = 40;
    end
	 
	 always@ (posedge(CLOCK_50)) begin
       if ( ENABLE ) begin
		  if (tone_counter == 0) begin
			  if (tone_duration < `ONE_SEC) begin
			     tone_duration <= tone_duration + 1;
				  freq1 <= freq1 + 1;
				  if (freq1 == 74) begin 
					  freq1 <= 0;
					  data_out <= sin_table[i];
					  i <= i+ 1;
					  if(i == 1023) begin
							i <= 0;
					  end
				  end
			  end
			  else begin
			      tone_duration <= 0;
					tone_counter <= tone_counter + 1;
			  end
		  end
		  else if (tone_counter == 1) begin
			  if (tone_duration < `ONE_SEC) begin
				  tone_duration <= tone_duration + 1;
				  freq2 <= freq2 + 1;
				  if (freq2 == 99) begin 
					  freq2 <= 0;
					  data_out <= sin_table[i];
					  i <= i+ 1;
					  if(i == 1023) begin
							i <= 0;
					  end
				  end
			  end
			  else begin
			      tone_duration <= 0;
					tone_counter <= tone_counter + 1;
			  end
		  end
		  else begin
			  if (tone_duration < `ONE_SEC) begin
				  tone_duration <= tone_duration + 1;
				  freq3 <= freq3 + 1;
				  if (freq3 == 119) begin 
					  freq3 <= 0;
					  data_out <= sin_table[i];
					  i <= i+ 1;
					  if(i == 1023) begin
							i <= 0;
					  end
				  end
			  end
			  else begin
			      tone_duration <= 0;
					tone_counter <= 0;
			  end
		  end
      end
    end
	 
	 endmodule