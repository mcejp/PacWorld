/*
 * Autograb
 *
 * Copyright 1997 by Lennart Steinke
 *
 * You'll need JLib 1.7 or higher to compile the source
 */

#include <jlib.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include "bmp.h"
#include "jlib_bmp.h"

#define MIN(a,b) ((a) <(b) ? (a) : (b))
#define BUFF_MEM(x_) pic->buff->buffer[(x_)]
#define AT(x,y) (x)+(y)*(IMG_MAX_X(pic)+1)
#define WAIT while(!kbhit()); while(kbhit()){getch();}
#define BEEP putchar(7);fflush(stdout);

image *pic;

UBYTE	border		= 0xff,
		raw			= 0x00,
		tile		= 0x00,
		palStore	= 0x00,
		debug		= 0x00,
		visible 	= 0x00,
		splitAll	= 0x00,
		coords		= 0x00,
		binary		= 0x00;

USHORT	numSprites	= 0x00;

USHORT	gb_width	= 0,
		gb_height	= 0;
USHORT	tile_x	= 0,
		tile_y	= 0;

char	*InFileName = NULL,
		*OutFileName= NULL;
FILE	*OutFile;


void (*SaveToFile)(buffer_rec *, int, int, int, int);


void
printUsage(void)
{
	printf("Usage:\n------\n\tautograb <infile> [-o <outfile>] [-raw] [-pal] [-v] [-b <index>] [-size Width Height]\n\n");
	printf("Options:\n");
	printf("\t-raw  : Stores only the size and data of the images\n");
	printf("\t-pal  : Stores palette (only of use in raw mode)\n");
	printf("\t-tile : Stores the images in tile (*.tle) format.\n");
	printf("\t-v    : Displays the image and shows the grabbed areas\n");
	printf("\t-b    : Speclifies which color index to use as border, default is 255\n");
	printf("\t-a    : Grab all: Splits whole picture. Needs size option, also\n");
	printf("\t-o    : Specifies the output filename, default is \"autograb.spr\"\n");
	printf("\t        respectively \"autograb.tle\" if using tile mode.\n");
	printf("\t-size Width Height : Specifies width and height\n");
	printf("\t-ct   : Stores the position and size of the images found as ASCII text.\n");
	printf("\t-cb   : Stores the position and size of the images found in a binary\n");
	printf("\t        format\n");
	printf("\n");
}

int
parseArgs(int argc, char **argv)
{
	int a,c=0, foo;

	if (argc < 2)
	{
		printUsage();
		exit(0);
	}

	for (a=1; a<argc; a++)
	{
		if ((*argv[a]=='-') || (*argv[a]=='/'))
		{
			switch (argv[a][1])
			{
				case 'h':
				case '?':
					printUsage();
					exit(0);
				break;

				/* Output file name */
				case 'o':
					OutFileName = argv[a+1];
					a++;
				break;

				/* Enable raw mode */
				case 'r':
					if (argv[a][2] == 'a' && argv[a][3] =='w')
						raw =1;
				break;

				/* Pal ? */
				case 'p':
					if (argv[a][2] == 'a' && argv[a][3] =='l')
						palStore =1;
				break;

				/* Enable tile mode */
				case 't':
					if (argv[a][2] == 'i' && argv[a][3] =='l' && argv[a][4]=='e')
					{
						tile =1;
						raw =0;
					}
				break;

				/* Visible mode */
				case 'v':
					visible=1;
				break;

				case 'd':
					debug = 1;
					visible =1;
				break;

				/* Border color */
				case 'b':
					border = atoi(argv[a+1]);
					if (!border)
					{
						printf("It's not allowed to use index %i as border color\n", border);
						exit(0);
					}
					a++;
				break;

				/* Size */
				case 's':
					if (argv[a][2] == 'i' && argv[a][3] =='z' && argv[a][4]=='e')
					{
						if (argc <= a+2)
						{
							printf("Error: Size information expected, found end of line instead.\n");
							exit(0);
						}

						gb_width  = atoi(argv[a+1]);
						gb_height = atoi(argv[a+2]);

						a+=2;
					}
				break;

				/* Split whole image */
				case 'a':
					splitAll =1;
				break;
				/* Save coords */
				case 'c':
					if (argv[a][2] == 'b')
					{
						binary =1;
					}
					else if (argv[a][2] == 't')
					{
						coords =1;
					}
				break;
			}
		}
		else
		{
			if (c == 0)
			{
				InFileName = argv[a];
				c++;
			}
			else
			{
				printf("Option expected, found %s instead.\n", argv[a]);
				exit(0);
			}
		}
	}

	if (splitAll && gb_width==0)
	{
		printf("If you specify split all (-a), you must use -size, also.\n");
		exit(0);
	}

	if (InFileName == NULL)
	{
		printf("No input file specified. Use autograb -? for help.\n");
		exit(0);
	}
	else
	{
		FILE *tmp;

		tmp=fopen(InFileName,"r");

		if (!tmp)
		{
			printf("Unable to read file \"%s\". Check the filename, and try again.\n", InFileName);
			exit(0);
		}
		else
		{
			fclose(tmp);
		}
	}

	foo = tile + raw + coords +binary;
	if (foo >1)
	{
		printf("You specified more than one output format. Program halted\n");
		exit(0);
	}

	return 1;
}

void
OpenOutFile()
{
	USHORT a=0;

	if (OutFileName)
	{
		OutFile = fopen(OutFileName, "wb");
	}
	else
	{
		if (tile)
		{
			OutFile = fopen("autograb.tle","wb");
		}
		else if (coords)
		{
			if (binary)
			{
				OutFile = fopen("autograb.crd","wb");
			}
			else
			{
				OutFile = fopen("coords.txt","w");
			}
		}
		else
		{
			OutFile = fopen("autograb.spr","wb");
		}
	}

	if (!coords)
	{
		fwrite(&a, sizeof(a), 1, OutFile);
	}

	if (tile)
	{
		/* Width and Height dummys */
		fwrite(&a, sizeof(a),2, OutFile);

		/* Write image palette */
		fwrite(IMG_PALETTE(pic), sizeof(UBYTE), 768, OutFile);
	}
}

void
CloseOutFile()
{
	USHORT a=numSprites;

	if (raw && palStore)
	{
		/* Write image palette */
		fwrite(IMG_PALETTE(pic), sizeof(UBYTE), 768, OutFile);
	}


	if (!coords)
	{
		fseek(OutFile, 0, SEEK_SET);
		fwrite(&a, sizeof(a), 1, OutFile);

		if (tile)
		{
			USHORT	width  = gb_width,
					height = gb_height;
			fwrite(&width,  sizeof(width ), 1, OutFile);
			fwrite(&height, sizeof(height), 1, OutFile);
		}
	}
	fclose(OutFile);
}

void SaveDebug(buffer_rec *buff, int x, int y, int width, int height)
{
	static char ch=0;
	int a;

	for (a=0; a<height; a++, y++)
	{
		memset(&(BUFF_MEM( AT(x,y))) , ch, width);
		screen_blit_buff_to(0,0, pic->buff,0,0,IMG_MAX_X(pic),IMG_MAX_Y(pic));
	}
	if (ch==0)
		ch=250;
	else
		ch=0;
}

void SaveAsCoords(buffer_rec *buff, int x, int y, int width, int height)
{
	USHORT	w = width,
			h = height,
			posX = x,
			posY = y;

	if (binary)
	{
		fwrite(&posX, sizeof(USHORT), 1, OutFile);
		fwrite(&posY, sizeof(USHORT), 1, OutFile);
		fwrite(&w, sizeof(USHORT), 1, OutFile);
		fwrite(&h, sizeof(USHORT), 1, OutFile);
	}
	else
	{
		fprintf(OutFile, "%i, %i, %i, %i\n", posX, posY, w, h);
	}
}

void SaveAsRaw(buffer_rec *buff, int x, int y, int width, int height)
{
	int		padX	=0,
			padY	=0,
			actY	=0;
	char	*buff_	=NULL;
	USHORT	width_	= width,
			height_	= height;

    
	if (gb_width && gb_width != width)
	{
		padX=1;
		buff_ = malloc(gb_width);
		width_=gb_width;
	}

	if (gb_height && gb_height != height)
	{
		padY = gb_height - height;
		height_ =gb_height;
	}

    fwrite(&width_ , sizeof(width_ ), 1, OutFile);
	fwrite(&height_, sizeof(height_), 1, OutFile);


	for (actY=0; actY<height; actY++,y++)
	{
		if (padX)
		{
			memset(buff_, 0, gb_width);
			memcpy(buff_, &(BUFF_MEM( AT(x,y))), width);
		}
		else
		{
			buff_=&(BUFF_MEM( AT(x,y)));
		}
		fwrite(buff_, 1, width_, OutFile);
	}

	if (!padX)
	{
		buff_ = malloc(gb_width);
	}
	memset(buff_, 0, gb_width);

	for (y=0; y<padY; y++)
	{
		fwrite(buff_,1,gb_width, OutFile);
	}

	free(buff_);
}

void SaveAsJlib(buffer_rec *buff, int x, int y, int width, int height)
{
	FILE 	*fp;
	int		i,j, w,h, padX=0, padY=0;
	UBYTE	*start=B_BUFF_PTR(buff);
	int		x1	= x,
			y1	= y,
			x2	= x+ width -1,
			y2	= y+ height-1;


	fp = OutFile;

    w = MIN(width , 255);
	h = MIN(height, 255);

	if (gb_width && gb_height)
	{
		w = gb_width;
		h = gb_height;
		if (gb_width > w)
			padX = gb_width -w;

		if (gb_height > h)
			padY = gb_height -h;
	}


	/* The following lines are taken from sprgrab.c
	 * (c) by J. Griffiths
	 */
	
	/* write out width & height */
	fputc(w, fp);
	fputc(h, fp);

	/* write out data */
	for(i=y1;i <= y2;i++)
	{
		for(j=x1;j <= x2;j++)
		{
			fputc(start[i*B_X_SIZE(buff)+j],fp);
		}

		for (j=0; j<padX; j++)
		{
			fputc(0,fp);
		}
	}
	for (i=0; i < padY; i++)
	{
		for (i=0; i< w; i++)
			fputc(0, fp);
	}

	/* assume a bounding rectangle of the whole sprite */
	fputc(1,fp); /* one rect */

	fputc(0,fp);
	fputc(0,fp);      /* 0,0 to w,h */

	fputc(x2-x1+1,fp);
	fputc(y2-y1+1,fp);

}

void SaveAsTile(buffer_rec *buff, int x, int y, int width, int height)
{
	int		padX	=0,
			padY	=0,
			actY	=0;
	char	*buff_	=NULL;

	if (gb_width && gb_width != width)
	{
		padX=1;
		buff_ = malloc(gb_width);
	}

	if (gb_height && gb_height != height)
	{
		padY = gb_height - height;
	}

	for (actY=0; actY<height; actY++, y++)
	{
		if (padX)
		{
			memset(buff_, 0, gb_width);
			memcpy(buff_, &(BUFF_MEM( AT(x,y))), width);
		}
		else
		{
			buff_=&(BUFF_MEM( AT(x,y)));
		}
		fwrite(buff_, 1, gb_width, OutFile);
		memset(buff_, 255, gb_width);
		if (visible)
			screen_blit_buff_to(0,0, pic->buff,0,0,IMG_MAX_X(pic),IMG_MAX_Y(pic));
	}

	if (!padX)
	{
		buff_ = malloc(gb_width);
	}
	memset(buff_, 0, gb_width);

	for (y=0; y<padY; y++)
	{
		fwrite(buff_,1,gb_width, OutFile);
	}

	free(buff_);
}


void SaveImages(buffer_rec *buff, int x, int y, int width, int height)
{
	if (gb_width || gb_height)
	{
		if (gb_width != width || gb_height != height)
		{
			int curWidth,
				curHeight = height;
			int curX,
				curY=y;

			while (curHeight >0)
			{
				curWidth	= width;
				curX		= x;

				while (curWidth >0)
				{
					SaveToFile( buff, curX, curY, MIN(curWidth,gb_width), MIN(curHeight,gb_height));
					curWidth	-= gb_width;
					curX		+= gb_width;
					numSprites++;
				}
				curHeight	-= gb_height;
				curY	 	+= gb_height;
			}
		}
		else
		{
			SaveToFile(buff, x,y,width,height);
			numSprites++;
		}
	}
	else
	{
		SaveToFile(buff, x,y,width,height);
		numSprites++;
	}
}


int traceBorder(image *pic, USHORT x, USHORT y)
{
	USHORT	width	= 0,
			height	= 0,
			act_x	= x+1,
			act_y	= y+1;

	if ( (BUFF_MEM( AT(x,y)  ) != border) ||
		 (BUFF_MEM( AT(x+1,y)) != border) ||
		 (BUFF_MEM( AT(x,y+1)) != border))
	{
		return 1;
	}

	while ( BUFF_MEM( AT(act_x, act_y)) != border )
	{
		width++;
		act_x++;
	}

	if (width == 0)
	{
		return 1;
	}

	act_x = x+1;
	while ( BUFF_MEM( AT(act_x, act_y)) != border )
	{
		height++;
		act_y++;
	}

	if (height >0)
	{
		if ( tile )
		{
			if ( gb_width==0 && gb_height==0)
			{
				gb_width  = width;
				gb_height = height;
			}
		}
		SaveImages(pic->buff, x+1, y+1, width, height);

		return width;
	}
	else
	{
		return 1;
	}
}

int checkBMP(const char* filename)
{
	char *ext = "    ";
	int len = strlen(filename);
	int a;
	if (len > 4)
	{
		len -=4;
		for (a=0; a<4; a++)
		{
			ext[a] = toupper( *(filename+len+a));
		}
		if (!strcmp(ext, ".BMP"))
		{
			return 1;
		}
		else return 0;
	}
	else
	{
		return 0;
	}
}


void
main(int argc, char **argv)
{
	USHORT	width, height, x,y;

	printf("\nautograb (c) 1997 Lennart Steinke ");
	printf("<lennart_steinke@bigfoot.com>\n\n");
	printf("Visit The Game Programming Galaxy:\n");
	printf("http://www.geocities.com/SiliconValley/Vista/6774/\n\n\n");

	parseArgs(argc, argv);

	if (debug)
	{
		SaveToFile = SaveDebug;
	}
	else if (tile)
	{
		SaveToFile = SaveAsTile;
	}
	else if (raw)
	{
		SaveToFile = SaveAsRaw;
	}
	else if (coords)
	{
		SaveToFile = SaveAsCoords;
	}
	else
	{
		SaveToFile = SaveAsJlib;
	}



	
	if (checkBMP(InFileName))
	{
		pic = image_load_bmp(InFileName);
		if (!pic)
		{
			printf("Error reading the bmp. At the moment, only 8 bit files are supported.\n");
			exit(0);
		}
	}
	else
	{
		pic = image_load_pcx(InFileName);
		if (!pic)
		{
			printf("Unable to read the pcx.\n");
			exit(0);
		}
	}

    if (visible)
	{
		screen_set_video_mode();
	}



	OpenOutFile();

	if (visible)
	{
		screen_block_set_pal(IMG_PALETTE(pic));
		screen_blit_buff_to(0,0, pic->buff,0,0,IMG_MAX_X(pic),IMG_MAX_Y(pic));
	}


	width  = IMG_MAX_X(pic)+1;
	height = IMG_MAX_Y(pic)+1;

	if (splitAll)
	{
		SaveImages(pic->buff, 0, 0, width, height);
	}
	else
	{
		for (y =0; y < height-2; y++)
		{
			for (x=0; x < width-2; )
			{
				x+=traceBorder(pic, x,y);
			}
		}
	}

	CloseOutFile();

	image_free(pic);

	if (visible)
	{
		screen_blit_buff_to(0,0, pic->buff,0,0,B_X_SIZE(pic->buff),B_Y_SIZE(pic->buff));
		BEEP;
		WAIT;
		screen_restore_video_mode();
		printf("autograb (c) 1997 Lennart Steinke ");
		printf("<lennart_steinke@bigfoot.com>\n\n");
		printf("Visit The Game Programming Galaxy:\n");
		printf("http://www.geocities.com/SiliconValley/Vista/6774/\n\n\n");
	}


	if (!coords)
	{
		switch (numSprites)
		{
			case 0:
				printf("No bounding rects found.\n");
			break;

			case 1:
				printf("Stored 1 %s %s %s\n",
					tile ? "tile" : (coords || binary) ? "coordinate set" : "sprite",
					raw ? "using raw mode" : binary ? "as binary data" : coords ? "as ascii text" : tile ? "" : "using JLib mode",
					(raw && palStore) ? "with palette info" : "");
			break;

			default:
				printf("Stored %i %s %s %s\n", numSprites, tile ? "tiles" : "sprites", raw ? "using raw mode" : tile ? "" : "using JLib mode", (raw && palStore) ? "with palette info" : "");
		}
	}
}
