// This Program was written by Gordian Edenhofer on 04/05/14 and is licensed under the GPLv2+
// semt - Simple Edid Manipulation Tool

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

	int i, c, x_res, y_res, checksum = 0, debug = 0;
	unsigned char edid[128];
	FILE *fp;

	while ((c = getopt(argc,argv,"dvh")) != -1) {
		switch (c) {
		case 'd':
			debug = 1;
			break;
		case 'v':
			printf("%s: Version 1.1.1 by Gordian Edenhofer\nThe last revision was made on 03/02/2016 21:35:42\n", argv[0]);
			return 0;
		case 'h':
			printf("semt - Simple EDID Manipulation Tool Version 1.1\n"
			"Processing the \"Extended display identification data\" (EDID) in\n"
			"order to apply a custom resolution. This is helpfull if the\n"
			"concerning monitor does not provide an EDID itself or its EDID iscorrupted.\n\n"
			"Usage: semt [-d] <Input File> <X-Resolution> <Y-Resolution> <Output File>\n"
			"\t-h\tDisplay this help message and quit\n"
			"\t-v\tPrint the version number and quit\n"
			"\t-d\tEnter debug mode\n\n"
			"Examples:\t# semt ~/Downloads/edid.bin 1920 1080 ./custom_edid.bin\n"
			"\t\t# semt -d ./edid.bin 1680 1050 /etc/X11/edid.bin\n\n"
			"It makes sense to use this command in conjunction with an altered\n"
			"/etc/X11/xorg.conf file, in which the new EDID is invoked.\n"
			"This can be done by adding the following line somewhere under\n"
			"the screen section:\n"
			"    Option \"CustomEDID\" \"CRT-0:/path/to/modified/edid.bin\"\n\n"
			"Copyright (c) Gordian Edenhofer <gordian.edenhofer@gmail.com>\n");
			return 0;
		default:
			printf("%s: unknown flag %c\n", argv[0], c);
			return 1;
		}
	}

	if (argc-optind != 4) {
		printf("%s: is ment to be used with the following arguments [Inputfile] [X-Resolution] [Y-Resolution] [Outputfile]\n", argv[0]);
		return 1;
	}
	if ((fp = fopen(argv[optind],"r")) == NULL) {
		printf("%s: cannot read %s\n", argv[0], argv[optind]);
		return 2;
	}
	if (fread( edid, 1, 128, fp ) != 128) {
		printf("%s: cannot read all of %s\n", argv[0], argv[optind]);
		return 3;
	}
	if (fclose(fp) != 0) {
		printf("%s: cannot close input file %s\n", argv[0], argv[optind]);
		return 4;
	}

	x_res = atoi(argv[optind+1]);
	y_res = atoi(argv[optind+2]);

	// Debugging Code
	if (debug > 0) {
		// X
		printf("Writing x_res(mod)256=  %x\t to edid[56]=%x\n", x_res%256, edid[56]);
		printf("Writing (x_res/256)*16= %x\t to edid[58]=%x\n", (x_res/256)*16, edid[58]);
		printf("Writing x_res(mod)256=  %x\t to edid[74]=%x\n", x_res%256, edid[74]);
		printf("Writing (x_res/256)*16= %x\t to edid[76]=%x\n", (x_res/256)*16, edid[76]);
		// Y
		printf("Writing y_res(mod)256=  %x\t to edid[59]=%x\n", y_res%256, edid[59]);
		printf("Writing (y_res/256)*16= %x\t to edid[61]=%x\n", (y_res/256)*16, edid[61]);
		printf("Writing y_res(mod)256=  %x\t to edid[77]=%x\n", y_res%256, edid[77]);
		printf("Writing (y_res/256)*16= %x\t to edid[79]=%x\n", (y_res/256)*16, edid[79]);
	}

	// Dumping X-Resolution to EDID
	edid[56] = x_res%256;
	edid[58] = (x_res/256)*16;
	edid[74] = x_res%256;
	edid[76] = (x_res/256)*16;

	// Dumping Y-Resolution to EDID
	edid[59] = y_res%256;
	edid[61] = (y_res/256)*16;
	edid[77] = y_res%256;
	edid[79] = (y_res/256)*16;

	// Calculating and writing the checksum
	for (i=0; i<=126; ++i)
		checksum += edid[i];
	checksum %= 256;
	edid[127] = (256-checksum)%256;


	if ((fp = fopen(argv[optind+3],"w")) == NULL) {
		printf("%s: cannot write %s\n", argv[0],argv[optind+3]);
		return 5;
	}
	if (fwrite(edid,1,128,fp) != 128) {
		printf("%s: cannot write all 128 bytes to %s\n", argv[0], argv[optind+3]);
		return 6;
	}
	if (fclose(fp) != 0) {
		printf("%s: cannot close %s\n", argv[0], argv[optind+3]);
		return 7;
	}

	printf("%s: edid altered successfully.\n", argv[0]);

	return 0;
}
