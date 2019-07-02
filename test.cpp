
# include <cstdlib>
# include <iomanip>
# include <iostream>
# include <mpi.h>
# include <omp.h>
# include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]){
    Mat src = imread("1080-image.jpeg");

    int width = src.cols;
    int height = src.rows;
    int GRID_SIZE = 100;

    vector<Rect> mCells;

    for (int y = 0; y < height - GRID_SIZE; y += GRID_SIZE) {
        for (int x = 0; x < width - GRID_SIZE; x += GRID_SIZE) {
            int k = x*y + x;
            Rect grid_rect(x, y, GRID_SIZE, GRID_SIZE);
            cout << grid_rect<< endl;
            mCells.push_back(grid_rect);
            rectangle(src, grid_rect, Scalar(0, 255, 0), 1);
            imshow("src", src);
            imshow(format("grid%d",k), src(grid_rect));
            waitKey();
        }
    }
    return 0;
}

