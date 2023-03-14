#include <opencv2/opencv.hpp>
#include <iostream>
#include <direct.h>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap("Video.mp4");
    if (!cap.isOpened())
        return -1;

    int i = 0;
    string str, dir_name, video_name;
    Mat frame, prev_frame, diff_frame, bgr_frame;
    Scalar frameDiff;    

    cap >> prev_frame;
    cvtColor(prev_frame, prev_frame, COLOR_BGR2GRAY);
    dir_name = "frames";
    int status = _mkdir(dir_name.c_str());

    double threshold = 10;
    
    while (cap.read(frame))
    {
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        absdiff(frame, prev_frame, diff_frame);
        frameDiff = mean(diff_frame);
        string str = dir_name + "\\frame" + to_string(i) + ".jpg";

        if (frameDiff[0] > threshold)
        {
            cout << "Shot detected at frame " << cap.get(CAP_PROP_POS_FRAMES) << endl;
            if (!imwrite(str, prev_frame)) {
                cerr << "Error writing image file: " << str << endl;
                return 1;
            }
            i++;
        }
        waitKey(1);
        prev_frame = frame;
    }


    // This part for make video from captured frames
    video_name = "my_video.avi";
    VideoWriter video_writer(video_name, VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, prev_frame.size());

    if (!video_writer.isOpened()) {
        cerr << "Error opening video writer." << endl;
        return 1;
    }

    for (int j = 0; j < i; j++) {
        string filename = dir_name + "\\frame" + to_string(j) + ".jpg";
        Mat frame = imread(filename);

        if (frame.empty()) {
            cerr << "Error reading image file: " << filename << endl;
            return 1;
        }
        video_writer.write(frame);

    }
    cap.release();
    video_writer.release();

    cout << "Video created successfully." << endl;

    return 0;
}