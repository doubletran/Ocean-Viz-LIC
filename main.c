#include <stdio.h>
#include <stdlib.h>
int readFile(char* filename){
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
      perror("Error opening file");
      return 1;
  }

    // Read dimensions
    int nums, numt;
    fread(&nums, sizeof(int), 1, fp);
    fread(&numt, sizeof(int), 1, fp);

    printf("Texture Size: %d x %d\n", nums, numt);

    // Allocate memory for texture data
    float *textureData = (float *)malloc(nums * numt * 4 * sizeof(float));
    if (textureData == NULL) {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
    }

    // Read RGBA data
    fread(textureData, sizeof(float), nums * numt * 4, fp);
    fclose(fp);

    // Print texture data (optional)
    printf("Texture Data (RGBA values):\n");
    for (int t = 0; t < numt; t++) {
        for (int s = 0; s < nums; s++) {
            float red   = textureData[(t * nums + s) * 4 + 0];
            float green = textureData[(t * nums + s) * 4 + 1];
            float blue  = textureData[(t * nums + s) * 4 + 2];
            float alpha = textureData[(t * nums + s) * 4 + 3];

            printf("Pixel[%d, %d]: R=%.2f, G=%.2f, B=%.2f, A=%.2f\n", s, t, red, green, blue, alpha);
        }
    }

    // Free allocated memory
    free(textureData);
    return 0;
}

int writeFile(){
  int nums = 10; // Example width
    int numt = 10; // Example height

    FILE *fp = fopen("texture.tex", "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Write dimensions
    fwrite(&nums, sizeof(int), 1, fp);
    fwrite(&numt, sizeof(int), 1, fp);

    for (int t = 0; t < numt; t++) {
      for (int s = 0; s < nums; s++) {
          float red   = (float)(nums - s) / nums;  // More red on the left
          float green = (float)s / nums;           // More green in the middle
          float blue  = (float)(s % nums) / nums;  // More blue on the right
          float alpha = 1.0f;                      // Fully opaque

          fwrite(&red, sizeof(float), 1, fp);
          fwrite(&green, sizeof(float), 1, fp);
          fwrite(&blue, sizeof(float), 1, fp);
          fwrite(&alpha, sizeof(float), 1, fp);
      }
  }


    fclose(fp);
    return 0;
}
int main() {
  readFile("vis2dtexture.tex");
  
}