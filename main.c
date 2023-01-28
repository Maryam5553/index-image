#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pngio.h"
#include "node.h"
#include "palette.h"

int main(int argc, char **argv)
{
	struct image *image;
	struct pal_image *new_image;
	int rc;

	int option;
	int nb_options = 0;
	int palette_choice = -1;

	// options courtes
	while ((option = getopt(argc, argv, "sbg4h")) != -1) // à chaque appel getopt() renvoie la prochaine option trouvée
	{													 // renvoie -1 quand ne trouve plus d'options
		switch (option)
		{
		case 's': // 's' or 'b' or...
		case 'b':
		case 'g':
		case '4':
			palette_choice = option;
			nb_options++;
			break;
		case 'h': // help
			printf("***Help\nThis program indexes a png image.\nRun with ./index image.png new_image.png where image.png is the name of your image file.\nEnter one of the following options to choose a color palette:\n -s : saturated\n -b : black and white (binary)\n -g : grayscale\n -4 : 4-4-4\n");
			return EXIT_SUCCESS;
		default:
			fprintf(stderr, "Unknown option.\n");
			return 1;
		}
	}

	if (nb_options > 1)
	{
		fprintf(stderr, "Enter only one option.\n");
		return 1;
	}

	if (argc != optind + 2) // une fois que getopt() renvoie -1, optind prend l'index
	{						// du premier argument qui n'est pas une option (il met les arguments à la fin après les options)
		fprintf(stderr, "Requires 2 parameters.\n");
		return 1;
	}

	// lire l'image
	image = read_png(argv[optind]);
	if (image == NULL)
	{
		fprintf(stderr, "Couldn't read %s\n", argv[optind]);
		return 1;
	}

	// initialisation de new_image
	new_image = calloc(1, sizeof(struct pal_image));
	if (new_image == NULL)
	{
		fprintf(stderr, "Couldn't allocate new_image.\n");
		free_image(image);
		return 1;
	}

	new_image->width = image->width;
	new_image->height = image->height;
	new_image->data = malloc(image->height * sizeof(unsigned char *));
	if (new_image->data == NULL)
	{
		fprintf(stderr, "Couldn't allocate data of new_image.\n");
		free_image(image);
		free_pal_image(new_image);
		return 1;
	}

	for (int i = 0; i < image->height; i++)
	{
		new_image->data[i] = malloc(image->width);
		if (new_image->data[i] == NULL)
		{
			fprintf(stderr, "Couldn't allocate of data new_image.\n");
			free_image(image);
			free_pal_image(new_image);
			return 1;
		}
	}

	// création de l'arbre répertoriant les couleurs
	struct node *colors = NULL;

	unsigned char RGB[3];
	int pos[2] = {0, 0};
	RGB[0] = image->data[0][0];
	RGB[1] = image->data[0][1];
	RGB[2] = image->data[0][2];
	colors = insert(colors, RGB, pos);

	for (int i = 0; i < image->height; i++)
		for (int j = 0; j < image->width; j++)
		{
			RGB[0] = image->data[i][j * 4];
			RGB[1] = image->data[i][j * 4 + 1];
			RGB[2] = image->data[i][j * 4 + 2];
			pos[0] = i;
			pos[1] = j;
			insert(colors, RGB, pos);
		}

	int nb_coul = 0;
	nb_colors(colors, &nb_coul);

	// création de la palette
	// s'il y a moins de 256, on réutilise simplement des couleurs de l'image
	if (nb_coul < 256)
	{
		new_image->pal_len = nb_coul;
		new_image->pal = malloc(3 * new_image->pal_len * sizeof(unsigned char));

		if (new_image->pal == NULL)
		{
			fprintf(stderr, "Couldn't allocate pal.\n");
			free_node(colors);
			free_image(image);
			free_pal_image(new_image);
			return 1;
		}

		nb_coul = 0;
		new_image->pal = create_palette(colors, new_image->pal, &nb_coul);

		for (int i = 0; i < image->height; i++)
			for (int j = 0; j < image->width; j++)
				for (int k = 0; k < new_image->pal_len; k++)
					if (image->data[i][j * 4] == new_image->pal[k * 3] &&
						image->data[i][j * 4 + 1] == new_image->pal[k * 3 + 1] &&
						image->data[i][j * 4 + 2] == new_image->pal[k * 3 + 2])
					{
						new_image->data[i][j] = k;
						break;
					}
	}

	// plus de 256 : on crée une palette
	else
	{
		switch (palette_choice)
		{
		case 's': // PALETTE SATURÉE
			new_image->pal_len = 8;
			break;
		case '4': // PALETTE 4-4-4
			new_image->pal_len = 64;
			break;
		case 'g': // PALETTE GRIS
			new_image->pal_len = 256;
			break;
		case 'b': // PALETTE NOIR BLANC
			new_image->pal_len = 2;
			break;
		default:
			new_image->pal_len = 64;
			break;
		}

		// allocation palette
		new_image->pal = malloc(3 * new_image->pal_len * sizeof(unsigned char));
		if (new_image->pal == NULL)
		{
			fprintf(stderr, "Couldn't allocate pal.\n");
			free_node(colors);
			free_image(image);
			free_pal_image(new_image);
			return 1;
		}

		switch (palette_choice)
		{
		case 's':
			pal_SATURATED(new_image->pal);
			break;
		case '4':
			pal_444(new_image->pal);
			break;
		case 'g':
			pal_GRAY(new_image->pal);
			break;
		case 'b':
			pal_BLACKANDWHITE(new_image->pal);
			break;
		default:
			pal_444(new_image->pal);
			break;
		}
	}

	// index l'image à partir nouvelle palette
	index_image(colors, image, new_image);

	// écriture de l'image
	rc = write_pal_png(argv[optind + 1], new_image);
	if (rc < 0)
	{
		fprintf(stderr, "Couldn't write %s.\n", argv[optind + 1]);
		free_node(colors);
		free_image(image);
		free_pal_image(new_image);
		return 1;
	}

	free_node(colors);
	free_image(image);
	free_pal_image(new_image);

	return 0;
}
