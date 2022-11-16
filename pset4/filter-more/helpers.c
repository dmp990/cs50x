#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // image[i][j] has rgbtBlue, rgbtGreen, rgbtRed
            int average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // get the centre of image floored to previous integer
    // if the image has odd width, it will not touch the central column of pixels, which is what we want
    int centre = width / 2;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < centre; j++) // reflect first row, and then the second and so on
        {
            RGBTRIPLE temp;
            temp.rgbtBlue = image[i][j].rgbtBlue;
            temp.rgbtGreen = image[i][j].rgbtGreen;
            temp.rgbtRed = image[i][j].rgbtRed;

            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;

            image[i][width - 1 - j].rgbtBlue = temp.rgbtBlue;
            image[i][width - 1 - j].rgbtGreen = temp.rgbtGreen;
            image[i][width - 1 - j].rgbtRed = temp.rgbtRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // temperory holder of blurred rgb values
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) // row by row apply blur
        {
            int sum_blue = 0;
            int sum_green = 0;
            int sum_red = 0;
            float count = 0.0;

            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int currentX = i + x;
                    int currentY = j + y;

                    if (currentX < 0 || currentX > (height - 1) || currentY < 0 || currentY > (width - 1)) // pixels do not exist
                    {
                        continue;
                    }

                    sum_blue += image[currentX][currentY].rgbtBlue;
                    sum_green += image[currentX][currentY].rgbtGreen;
                    sum_red += image[currentX][currentY].rgbtRed;

                    count++;
                }

                temp[i][j].rgbtBlue = round(sum_blue / count);
                temp[i][j].rgbtGreen = round(sum_green / count);
                temp[i][j].rgbtRed = round(sum_red / count);
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width]; // temperory array to store edge data

    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gxBlue = 0;
            int gyBlue = 0;
            int gxGreen = 0;
            int gyGreen = 0;
            int gxRed = 0;
            int gyRed = 0;

            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++) // row by row calculate Gx, Gy and apply sobel's algo
                {
                    int currentX = i + x;
                    int currentY = j + y;

                    if (currentX < 0 || currentX > (height - 1) || currentY < 0 || currentY > (width - 1)) // pixels do not exist
                    {
                        continue;
                    }

                    gxBlue += image[currentX][currentY].rgbtBlue * gx[x + 1][y + 1];
                    gyBlue += image[currentX][currentY].rgbtBlue * gy[x + 1][y + 1];
                    gxGreen += image[currentX][currentY].rgbtGreen * gx[x + 1][y + 1];
                    gyGreen += image[currentX][currentY].rgbtGreen * gy[x + 1][y + 1];
                    gxRed += image[currentX][currentY].rgbtRed * gx[x + 1][y + 1];
                    gyRed += image[currentX][currentY].rgbtRed * gy[x + 1][y + 1];
                }
            }

            int blue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));
            int green = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int red = round(sqrt(gxRed * gxRed + gyRed * gyRed));

            // make sure that the max value is 255
            if (blue > 255)
            {
                blue = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (red > 255)
            {
                red = 255;
            }

            temp[i][j].rgbtBlue = blue;
            temp[i][j].rgbtGreen = green;
            temp[i][j].rgbtRed = red;
        }
    }

    // copy data from temp back to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }

    return;
}
