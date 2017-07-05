/*
	Author:		Steven Bremner
	Date:		April 23, 2014

	Description:
		All of the banners used by the opthandler in main.cpp
*/

#ifndef BANNERS_H
	#define BANNERS_H

#include "includes.h"

inline void printbanner()
{
	printf("%s%-10s%s%-10s%s%-10s%s%s",
		"-------------------------\n\n",
		"Author:", "Steven Bremner\n",
		"Date:", "December 17, 2015\n",
		"\nThanks for using the Base-64 Soup tool from SteveInternals!\n"
	);
}

inline void printexamples(std::string execPath)
{
	printf("Examples:\n\n");

	printf(" Standard Base-64 encoding of data:\n");
	printf("\t%s --encode \"SteveInternals\"\n\n", execPath.c_str());

	printf(" Encoding data using a custom alphabet:\n");
	printf("\t%s --alphabet \"rHtBbUjXTyq0ARnE2h6SFN4D8WpKGius3Ll5gmIfz+ekwCPJvaMQ179OdZxYc/Vo\" --encode \"SteveInternals\"\n\n", execPath.c_str());

	printf(" Decoding data using a custom alphabet:\n");
	printf("\t%s --alphabet \"rHtBbUjXTyq0ARnE2h6SFN4D8WpKGius3Ll5gmIfz+ekwCPJvaMQ179OdZxYc/Vo\" --decode \"FOhmiINyKfhmGIZLKXA=\"\n\n", execPath.c_str());
}

inline void printusage(std::string execPath)
{
	printf("\nUsage for %s:\n\n", execPath.c_str());
	printf(" %-24s\t%s\n\n", "--encode <input data>", "Specify data to encode");
	printf(" %-24s\t%s\n\n", "--decode <input data>", "Specify data to decode");
	printf(" %-24s\t%s\n\n", "--alphabet <string:64>", "Optional: Specify the order of the base64 alphabet");
	printf(" %-24s\t%s\n\n", "--random", "Randomize the alphabet");
	printf(" %-24s\t%s\n\n", "--hide-banner", "hides the SteveInternals banner from the final output");
	printf(" %-24s\t%s\n\n", "-h,-H,--help", "prints the usage or help page");

	printexamples(execPath);

	printbanner();
}

#endif