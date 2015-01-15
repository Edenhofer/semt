// This Program was written by Gordian Edenhofer on 04/05/14
// It is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
// Simple Edid Manipulation Tool
// How To:	semt [Inputfile] [X-Resolution] [Y-Resolution] [Outputfile] [Options]

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
			printf("%s: Version 1.1	by Gordian Edenhofer\nThe last revision was made on 25/12/2014 15:50:42\n", argv[0]);
			return 0;
		case 'h':
			printf("NAME\n"
			"\tsemt - Simple EDID Manipulation Tool Version 1.1\n\n"
			"SYNOPSIS\n"
			"\tsemt [Inputfile] [X-Resolution] [Y-Resolution] [Outputfile] [Options]\n\n"
			"EXAMPLE\n"
			"\tsemt ./PATH/TO/TEMPLATE/EDID 1920 1080 ./edid.bin\n\n"
			"DESCRIPTION\n"
			"\tThe \"semt\" command is an easy way to manipulate the resolution\n"
			"\tin the EDID.\n\n"
			"\tThe following options are available:\n\n"
			"\t-d\n"
			"\t\tenter debug mode\n\n"
			"\t-v\n"
			"\t\tprint the version number and quit\n\n"
			"\t-h\n"
			"\t\tdisplay this help message and quit\n\n"
			"\tIt makes sense to use this command in conjunction with an altered\n"
			"\t/etc/X11/xorg.conf file, in which the new EDID is invoked.\n"
			"\tThis can be done by adding the following line somewhere under\n"
			"\tthe screen section:\n"
			"\t'Option \"CustomEDID" "DFP:/path/to/modified/edid.bin\"'\n\n"
			"AUTHOR\n"
			"\tThe program was written and is maintained by Gordian Edenhofer.\n"
			"\tFor further question concerning the program please contact the author\n"
			"\tvia <gordian.edenhofer@yahoo.de>\n\n"
			"\tsemt - Simple EDID Manipulation Tool by Gordian Edenhofer\n"
			"\tis licensed under the Creative Commons Attribution-ShareAlike 3.0\n"
			"\tUnported License.\n");
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
