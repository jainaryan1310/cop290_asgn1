void LeftClick(int event, int x, int y, int flags, void* param);
int getCropCoordinates(Mat grayImg);
Mat warp(Mat grayImg);
Mat crop(Mat warpedImg);