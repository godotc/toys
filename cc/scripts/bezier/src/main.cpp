#include "opencv2/calib3d.hpp"
#include "opencv2/core.hpp"
#include "opencv2/core/matx.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <queue>
#include <vector>

using namespace cv;



std::vector<cv::Point2f> points;

int selected_index = -1;

int ControlPointSize = 20;

// Callback function for mouse events
void mouseCallback(int event, int x, int y, int flags, void *userdata)
{
    auto p = Point2f{static_cast<float>(x), static_cast<float>(y)};
    if (event == EVENT_LBUTTONDOWN) {
        selected_index = -1;
        for (int i = 0; i < points.size(); ++i) {
            auto diff = points[i] - p;
            if (norm(diff) < ControlPointSize * 1.5) {
                selected_index = i;
                break;
            }
        }
        if (selected_index == -1) {
            points.push_back(p);
        }
        std::cout << "Left mouse button clicked at (" << x << ", " << y << ")" << std::endl;
    }
    else if (event == EVENT_LBUTTONUP) {
        selected_index = -1;
    }
    else if (event == EVENT_MOUSEMOVE) {
        // std::cout << "Mouse moved at (" << x << ", " << y << ")" << std::endl;
        if (selected_index != -1) {
            points[selected_index] = p;
        }
    }
}

auto lerp(auto p1, auto p2, float t)
{
    return p1 + (p2 - p1) * t;
}

auto Points(std::vector<Point2f> points)
{
    std::vector<Point2f> last_points = points;
    std::vector<Point2f> nex_points;

    Point2f p1, p2;
    float   t = 0;
    for (int i = 2; i < last_points.size(); i += 2) {
        p1 = points[i - 2];
        p2 = points[i - 1];

        nex_points.emplace(lerp(p1, p2, t));
    }
}



float t = 0.5;

int main()
{

    // Create a black image window
    Mat image = Mat::zeros(800, 800, CV_8UC3);

    // Create a window
    auto flags = cv::WINDOW_AUTOSIZE | cv::WINDOW_AUTOSIZE;
    cv::namedWindow("image", flags);

    // Bind the mouse callback function
    setMouseCallback("image", mouseCallback);

    // Main loop to display the image window
    while (true) {
        for (auto &p : points) {
            cv::circle(image, p,
                       ControlPointSize, {255.f, 0.f, 0.f}, -1);
        }

        for (int i = 1; i < points.size(); ++i) {

            lerp(p1, p2, t);
        }



        int key = waitKey(1);
        if (key == 'q') {
            break;
        }

        imshow("image", image);

        image = Mat::zeros(800, 800, CV_8UC3);
    }

    destroyAllWindows();

    return 0;
}
