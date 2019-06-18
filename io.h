ScvImage *LoadFile(char *);
void GrayscaleProc(ScvImage *);
void saveImageToBmp(ScvImage *, char *);
void printBmpHead(BITMAPFILEHEADER *);
void printBmpInfoHead(BITMAPINFOHEADER *);
void printRgbQuan(RGBQUAD *);