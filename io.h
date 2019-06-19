ScvImage *LoadFile(char *, FILE *);
void GrayscaleProc(ScvImage *);
void saveImageToBmp(ScvImage *, FILE *);
void printBmpHead(BITMAPFILEHEADER *);
void printBmpInfoHead(BITMAPINFOHEADER *);
void printRgbQuan(RGBQUAD *);